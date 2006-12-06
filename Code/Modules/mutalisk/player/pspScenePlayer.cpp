#include "pspScenePlayer.h"
#include "ScenePlayer.h"

#include <memory>

using namespace std;

////////////////////////////////////////////////
auto_ptr<RenderableScene> prepare(RenderContext& rc, mutalisk::data::scene const& data, std::string const& pathPrefix)
{
	auto_ptr<RenderableScene> scene(new RenderableScene(data));

	// load shared resources
	scene->mResources.meshes = new RenderableScene::SharedResources::Mesh[data.meshCount];
	for(size_t q = 0; q < data.meshCount; ++q)
	{
		scene->mResources.meshes[q].blueprint = loadResource<mutalisk::data::mesh>(pathPrefix + data.meshIds[q]);
		scene->mResources.meshes[q].renderable = prepare(rc, *scene->mResources.meshes[q].blueprint);
	}
	scene->mResources.animCharSet = loadResource<mutant::anim_character_set>(pathPrefix + data.animCharId);

	// setup scene
	scene->setClip(data.defaultClipIndex);

	return scene;
}

namespace {
	size_t weightSize(unsigned int vertexDecl)
	{
		if(vertexDecl & GU_WEIGHT_8BIT)
			return 1U;
		else if(vertexDecl & GU_WEIGHT_16BIT)
			return 2U;
		else if(vertexDecl & GU_WEIGHT_32BITF)
			return 4U;
		return 0;
	}
}

auto_ptr<RenderableMesh> prepare(RenderContext& rc, mutalisk::data::mesh const& data)
{
	auto_ptr<RenderableMesh> mesh(new RenderableMesh(data));
/*	if(data.skinInfo)
	{
		// for CPU skinning remove weights from the vertex buffer

		size_t skipOffset = weightSize(data.vertexDecl) * data.skinInfo->weightsPerVertex;
		size_t newVertexStride = data.vertexStride - skipOffset;
		mesh->mAmplifiedVertexData = new unsigned char[newVertexStride * data.vertexCount];

		unsigned char* dst = mesh->mAmplifiedVertexData;
		unsigned char const* src = data.vertexData + skipOffset;
		for(size_t q = 0; q < data.vertexCount; ++q)
		{
			// copy 1 vertex without weights
			memcpy( &dst, &src, newVertexStride);
			dst += newVertexStride;
			src += data.vertexStride;
		}
	}*/

	return mesh;
}

namespace {
	void toNative(CTransform::t_matrix const& src, ScePspFMatrix4& dst)
	{
		float static scale = 1.0f;
		dst.x.x = src.Rot.Row[0].x * scale;
		dst.x.y = src.Rot.Row[1].x * scale;
		dst.x.z = src.Rot.Row[2].x * scale;
		dst.x.w = 0.0f;
		dst.y.x = src.Rot.Row[0].y * scale;
		dst.y.y = src.Rot.Row[1].y * scale;
		dst.y.z = src.Rot.Row[2].y * scale;
		dst.y.w = 0.0f;
		dst.z.x = src.Rot.Row[0].z * scale;
		dst.z.y = src.Rot.Row[1].z * scale;
		dst.z.z = src.Rot.Row[2].z * scale;
		dst.z.w = 0.0f;
		dst.w.x = src.Move.x;
		dst.w.y = src.Move.y;
		dst.w.z = src.Move.z;
		dst.w.w = 1.0f;
	}

	void toNative(float const* srcMatrixData, ScePspFMatrix4& dst)	
	{
		memcpy(&dst, srcMatrixData, sizeof(float)*16);
	}

	unsigned int toNative(mutalisk::data::Color const& srcColor)
	{
		unsigned int 
		dstColor = (unsigned int)(srcColor.r * 255.0f);
		dstColor |= (unsigned int)(srcColor.g * 255.0f) << 8;
		dstColor |= (unsigned int)(srcColor.b * 255.0f) << 16;
		dstColor |= (unsigned int)(srcColor.a * 255.0f) << 24;
		return dstColor;
	}

	void setCameraMatrix(RenderContext& rc, ScePspFMatrix4 const& cameraMatrix)
	{
		sceGumMatrixMode(GU_VIEW);

		ScePspFMatrix4 viewMatrix;
		gumFastInverse(&viewMatrix, &cameraMatrix);

		static bool overrideCamera = false;
		if(overrideCamera)
			sceGumLoadMatrix(&viewMatrix);
		else
			sceGumMultMatrix(&viewMatrix);
	}

	void setWorldMatrix(RenderContext& rc, ScePspFMatrix4 const& worldMatrix)
	{
		sceGumMatrixMode(GU_MODEL);
//		sceGumLoadIdentity();
		sceGumLoadMatrix(&worldMatrix);
	}


	struct LightState
	{
		LightState() : currentLightId(0) { totalAmbient.r = totalAmbient.g = totalAmbient.b = totalAmbient.a = 0.0f; }
		int currentLightId;
		mutalisk::data::Color totalAmbient;
	};

	void setLight(LightState& state, mutalisk::data::scene::Light const& light, ScePspFMatrix4 const& worldMatrix)
	{
		// $TBD: support specular
		// $TBD: support point/spot lights
		ASSERT(state.currentLightId < 4);

		int i = state.currentLightId;
		sceGuLightColor(i, GU_DIFFUSE, toNative(light.diffuse));
		sceGuLightAtt(i, light.attenuation[0], light.attenuation[1], light.attenuation[2]);

		state.totalAmbient.r += light.ambient.r;
		state.totalAmbient.g += light.ambient.g;
		state.totalAmbient.b += light.ambient.b;
		state.totalAmbient.a += light.ambient.a;
		sceGuAmbient(toNative(state.totalAmbient));

//		ScePspFVector3 dir = { worldMatrix.z.x, worldMatrix.z.y, worldMatrix.z.z };
		ScePspFVector3 dir = { 0, 1, 0 };
//		ScePspFVector3 pos = { worldMatrix.w.x, worldMatrix.w.y, worldMatrix.w.z };

		switch(light.type)
		{
		case mutalisk::data::scene::Light::Directional:
			sceGuLight(i, GU_DIRECTIONAL, GU_DIFFUSE, &dir);
			break;
		case mutalisk::data::scene::Light::Spot:
		case mutalisk::data::scene::Light::Point:
			ASSERT(!"Not implemented");
			break;
		}

		sceGuEnable(GU_LIGHT0 + i);
		if(i == 0)
			sceGuEnable(GU_LIGHTING);

		state.currentLightId++;
	}
	void setDefaultLight(LightState& state, ScePspFMatrix4 const& worldMatrix)
	{
		mutalisk::data::scene::Light l;
		l.type = mutalisk::data::scene::Light::Directional;
		l.ambient.r = l.ambient.g = l.ambient.b = l.ambient.a = 0.0f;
		l.diffuse.r = l.diffuse.g = l.diffuse.b = l.diffuse.a = 1.0f;//0.75f;
		l.specular.r = l.specular.g = l.specular.b = l.specular.a = 0.0f;
		l.attenuation[0] = 1.0f;
		l.attenuation[1] = 0.0f;
		l.attenuation[2] = 0.0f;
		l.theta = 0.0f;
		l.phi = 0.0f;

		setLight(state, l, worldMatrix);
	}
	void setDefaultLight(LightState& state)
	{
		ScePspFMatrix4 identityMatrix;
		gumLoadIdentity(&identityMatrix);
		setDefaultLight(state, identityMatrix);
	}

	void render(RenderContext& rc, RenderableMesh const& mesh)
	{
		int flag = 0;
		int count = 0;
		if(mesh.mBlueprint.indexData)
		{
			count = mesh.mBlueprint.indexCount;
			flag = GU_INDEX_16BIT;
		}
		else
			count = mesh.mBlueprint.vertexCount;

		sceGumDrawArray(
			mesh.mBlueprint.primitiveType, mesh.mBlueprint.vertexDecl|GU_TRANSFORM_3D|flag,
			count,
			mesh.mBlueprint.indexData, mesh.mBlueprint.vertexData);
	}
} // namespace


void render(RenderContext& rc, RenderableScene const& scene, bool animatedActors, bool animatedCamera, int maxActors, int maxLights)
{
	ScePspFMatrix4 nativeMatrix;
	ASSERT(rc.defaultEffect);

	LightState lightState;
	for(size_t q = 0; q < ((maxLights>0)?maxLights: scene.mBlueprint.lightCount); ++q)
	{
		mutalisk::data::scene::Light const& light = scene.mBlueprint.lights[q];
		if(animatedActors)
		{
			ASSERT(q >= 0 && q < scene.mState.light2XformIndex.size());
			toNative(scene.mState.matrices[scene.mState.light2XformIndex[q]], nativeMatrix);
		}
		else
			toNative(light.worldMatrix.data, nativeMatrix);
		setLight(lightState, scene.mBlueprint.lights[q], nativeMatrix);
	}

//	animatedCamera = false;
	if(animatedCamera && scene.mBlueprint.cameraCount > 0)
	{
		unsigned int cameraIndex = scene.mBlueprint.defaultCameraIndex;
		ASSERT(cameraIndex >= 0 && cameraIndex < scene.mState.camera2XformIndex.size());
		toNative(scene.mState.matrices[scene.mState.camera2XformIndex[cameraIndex]], nativeMatrix);
		setCameraMatrix(rc, nativeMatrix);

		if(scene.mBlueprint.lightCount == 0 && maxLights != 0)
			setDefaultLight(lightState, nativeMatrix);	
	}
	else
		if(scene.mBlueprint.lightCount == 0 && maxLights != 0)
			setDefaultLight(lightState);	


	unsigned int passCount = 1;
//	DX_MSG("begin effect") = rc.defaultEffect->Begin(&passCount, 0);
    for(unsigned int iPass = 0; iPass < passCount; iPass++)
    {
//		DX_MSG("begin pass") = rc.defaultEffect->BeginPass(iPass);
		for(size_t q = 0; q < ((maxActors>0)?maxActors: scene.mBlueprint.actorCount); ++q)
		{
			mutalisk::data::scene::Actor const& actor = scene.mBlueprint.actors[q];

			if(animatedActors)
			{
				ASSERT(q >= 0 && q < scene.mState.actor2XformIndex.size());
				toNative(scene.mState.matrices[scene.mState.actor2XformIndex[q]], nativeMatrix);
			}
			else
				toNative(actor.worldMatrix.data, nativeMatrix);

//			if(!scene.mResources.meshes[actor.meshIndex].blueprint->skinInfo)
				setWorldMatrix(rc, nativeMatrix);

//			DX_MSG("commit changes") = rc.defaultEffect->CommitChanges();
			render(rc, *scene.mResources.meshes[actor.meshIndex].renderable);
		}
//		DX_MSG("end pass") = rc.defaultEffect->EndPass();
	}
//	DX_MSG("end effect") = rc.defaultEffect->End();

}

////////////////////////////////////////////////

void CSkinnedAlgos::processSkinMesh(RenderableMesh& mesh, BoneMapT const& boneMap, CTransform::t_matrix const* matrices)
{		
	assert(mesh.mBlueprint.skinInfo);
	assert(!boneMap.empty());

/*	size_t positionsOffset = ~0U;
	size_t normalsOffset = sizeof(float)*mesh.mBlueprint.skinInfo->weightsPerVertex;//~0U;
	size_t weightsOffset = 0U;//~0U;
	size_t boneIndicesOffset = 0U;//~0U;

	for(size_t q = 0; q < MAX_FVF_DECL_SIZE && dx9Declaration[q].Stream != 0xff; ++q)
	{
		size_t offset = dx9Declaration[q].Offset;
		switch(dx9Declaration[q].Usage)
		{
		case D3DDECLUSAGE_POSITION:
			positionsOffset = offset;
			break;
		case D3DDECLUSAGE_BLENDWEIGHT:
			weightsOffset = offset;
			break;
		case D3DDECLUSAGE_BLENDINDICES:
			boneIndicesOffset = offset;
			break;
		case D3DDECLUSAGE_NORMAL:
			normalsOffset = offset;
			break;
		}
	}

	assert(positionsOffset != ~0);
	assert(normalsOffset != ~0);
	assert(weightsOffset != ~0);
	assert(boneIndicesOffset != ~0);
	*/

	// $HACK: hardcoded offsets
	// $TBD: calc offsets using vertexDecl
	size_t weightsOffset = 0U;
	size_t normalsOffset = weightsOffset + weightSize(mesh.mBlueprint.vertexDecl)*mesh.mBlueprint.skinInfo->weightsPerVertex;
	size_t positionsOffset = normalsOffset + sizeof(Vec3);
	size_t boneIndicesOffset = 0U;

	unsigned char* dstRaw = mesh.mAmplifiedVertexData;
	unsigned char const* srcRaw = mesh.mBlueprint.vertexData;
	unsigned char const* srcBoneIndices = mesh.mBlueprint.boneIndexData;

	processSkinMesh(
		reinterpret_cast<Vec3 const*>(srcRaw + positionsOffset),
		reinterpret_cast<Vec3 const*>(srcRaw + normalsOffset),
		reinterpret_cast<float const*>(srcRaw + weightsOffset),
		reinterpret_cast<unsigned char const*>(srcBoneIndices + boneIndicesOffset),

		reinterpret_cast<Vec3*>(dstRaw + positionsOffset),
		reinterpret_cast<Vec3*>(dstRaw + normalsOffset),

		mesh.mBlueprint.vertexStride,											// srcVertexStride
		mesh.mBlueprint.vertexStride,											// srcWeightStride
		mesh.mBlueprint.boneIndexStride,										// srcBoneIndexStride
		sizeof(Vec3)*2,															// dstVertexStride
		mesh.mBlueprint.vertexCount,

		*mesh.mBlueprint.skinInfo, boneMap, matrices);
}
