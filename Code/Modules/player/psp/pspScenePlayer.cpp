#include "pspScenePlayer.h"
#include "../ScenePlayer.h"

#include <memory>
#include <effects/BaseEffect.h>
#include <effects/all.h>

using namespace mutalisk;
using namespace mutalisk::effects;

////////////////////////////////////////////////
std::auto_ptr<RenderableScene> prepare(RenderContext& rc, mutalisk::data::scene const& data, std::string const& pathPrefix)
{
	std::auto_ptr<RenderableScene> scene(new RenderableScene(data));

	// load shared resources
	scene->mResources.meshes.resize(data.meshIds.size());
	for(size_t q = 0; q < data.meshIds.size(); ++q)
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
	size_t singleWeightSize(unsigned int vertexDecl)
	{
		unsigned int weightBits = vertexDecl & GU_WEIGHT_BITS;
		if(weightBits == GU_WEIGHT_8BIT)
			return 1U;
		else if(weightBits == GU_WEIGHT_16BIT)
			return 2U;
		else if(weightBits == GU_WEIGHT_32BITF)
			return 4U;
		ASSERT(!"no weights found");
		return 0U;
	}
}

std::auto_ptr<RenderableMesh> prepare(RenderContext& rc, mutalisk::data::mesh const& data)
{
	std::auto_ptr<RenderableMesh> mesh(new RenderableMesh(data));
	if(data.skinInfo)
	{
		;;printf("has skinInfo \n");
		// for CPU skinning remove weights from the vertex buffer

		size_t skipOffset = singleWeightSize(data.vertexDecl) * data.skinInfo->weightsPerVertex;
		size_t newVertexStride = data.vertexStride - skipOffset;
		mesh->mAmplifiedVertexData = new unsigned char[newVertexStride * data.vertexCount];
		mesh->mAmplifiedVertexDecl = data.vertexDecl & (~(GU_WEIGHTS_BITS|GU_WEIGHT_BITS));

		unsigned char* dst = mesh->mAmplifiedVertexData;
		unsigned char const* src = data.vertexData + skipOffset;
		for(size_t q = 0; q < data.vertexCount; ++q)
		{
			// copy 1 vertex without weights
			memcpy(dst, src, newVertexStride);
			dst += newVertexStride;
			src += data.vertexStride;
		}
		;;printf("skinInfo processed\n");
	}

	return mesh;
}

namespace {
	struct MatrixState
	{
		void applyWorldMatrix(RenderContext const& rc, ScePspFMatrix4 const& inWorldMatrix, BaseEffect::Input& dst)
		{
			world = inWorldMatrix;

			gumFastInverse(&invWorld, &world);
			gumMultMatrix(&worldViewProj, &rc.viewProjMatrix, &world);

			dst.matrices[BaseEffect::WorldMatrix] = &world;
			dst.matrices[BaseEffect::ViewMatrix] = &rc.viewMatrix;
			dst.matrices[BaseEffect::ViewProjMatrix] = &rc.viewProjMatrix;
			dst.matrices[BaseEffect::WorldViewProjMatrix] = &worldViewProj;
			dst.matrices[BaseEffect::InvWorldMatrix] = &invWorld;
		}

		ScePspFMatrix4 world;
		ScePspFMatrix4 worldViewProj;
		ScePspFMatrix4 invWorld;
	};

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

	void toNative(mutalisk::data::Color const& color, ScePspFVector4& vec)
	{
		vec.x = color.r;
		vec.y = color.g;
		vec.z = color.b;
		vec.w = color.a;
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

	void render(RenderContext& rc, RenderableMesh const& mesh, unsigned subset = 0)
	{
		unsigned char* vertexData = mesh.mBlueprint.vertexData;
		int vertexFlag = mesh.mBlueprint.vertexDecl;

		if(mesh.mAmplifiedVertexData)
		{
			vertexData = mesh.mAmplifiedVertexData;
			vertexFlag = mesh.mAmplifiedVertexDecl;
		}

		int indexCount = 0;
		int indexDataOffset = 0;
		if(mesh.mBlueprint.indexData)
		{
			indexCount = mesh.mBlueprint.subsets[subset].count;//mesh.mBlueprint.indexCount;
			indexDataOffset = mesh.mBlueprint.subsets[subset].offset * 2;	// $TBD: GU_INDEX_8BIT support
			vertexFlag |= GU_INDEX_16BIT;
		}
		else
			indexCount = mesh.mBlueprint.vertexCount;

		sceGumDrawArray(
			mesh.mBlueprint.primitiveType, vertexFlag | GU_TRANSFORM_3D,
			indexCount,
			mesh.mBlueprint.indexData + indexDataOffset, vertexData);
	}
} // namespace

/*
void render(RenderContext& rc, RenderableScene const& scene, bool animatedActors, bool animatedCamera, int maxActors, int maxLights)
{
	ScePspFMatrix4 nativeMatrix;
	ASSERT(rc.defaultEffect);

	LightState lightState;
	for(size_t q = 0; q < ((maxLights>0)?maxLights: scene.mBlueprint.lights.size()); ++q)
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
	if(animatedCamera && !scene.mBlueprint.cameras.empty())
	{
		unsigned int cameraIndex = scene.mBlueprint.defaultCameraIndex;
		ASSERT(cameraIndex >= 0 && cameraIndex < scene.mState.camera2XformIndex.size());
		toNative(scene.mState.matrices[scene.mState.camera2XformIndex[cameraIndex]], nativeMatrix);
		setCameraMatrix(rc, nativeMatrix);

		if(scene.mBlueprint.lights.empty() && maxLights != 0)
			setDefaultLight(lightState, nativeMatrix);	
	}
	else
		if(scene.mBlueprint.lights.empty() && maxLights != 0)
			setDefaultLight(lightState);	


	unsigned int passCount = 1;
//	DX_MSG("begin effect") = rc.defaultEffect->Begin(&passCount, 0);
    for(unsigned int iPass = 0; iPass < passCount; iPass++)
    {
//		DX_MSG("begin pass") = rc.defaultEffect->BeginPass(iPass);
		for(size_t q = 0; q < ((maxActors>0)?maxActors: scene.mBlueprint.actors.size()); ++q)
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

}*/

void render(RenderContext& rc, RenderableScene const& scene, bool animatedActors, bool animatedCamera, int maxActors)
{
	ScePspFMatrix4 nativeMatrix;

	if(scene.mBlueprint.defaultClipIndex == ~0U)
	{
		animatedActors = false;
		animatedCamera = false;
	}

	if(scene.mBlueprint.defaultCameraIndex != ~0U)
	{
		unsigned int cameraIndex = scene.mBlueprint.defaultCameraIndex;
		if(animatedCamera)
		{
			ASSERT(cameraIndex >= 0 && cameraIndex < scene.mState.camera2XformIndex.size());
			toNative(scene.mState.matrices[scene.mState.camera2XformIndex[cameraIndex]], nativeMatrix);
		}
		else
		{
			ASSERT(cameraIndex >= 0 && cameraIndex < scene.mBlueprint.cameras.size());
			toNative(scene.mBlueprint.cameras[cameraIndex].worldMatrix.data, nativeMatrix);
		}

		setCameraMatrix(rc, nativeMatrix);
	}

	MatrixState matrixState;

	static Lambert fx;
	BaseEffect::Input& fxInput = fx.allocInput();

	fx.captureState();
	unsigned passCount = fx.begin();

	size_t nextLightIndex = 0;
	for(size_t q = 0; q < scene.mBlueprint.lights.size(); ++q, ++nextLightIndex)
	{
		mutalisk::data::scene::Light const& light = scene.mBlueprint.lights[q];

		ASSERT(q >= 0 && q < scene.mState.light2XformIndex.size());
		toNative(scene.mState.matrices[scene.mState.light2XformIndex[q]], nativeMatrix);

		ASSERT(q < fxInput.lights.size());
		fxInput.lights[q].first = &light;
		fxInput.lights[q].second = nativeMatrix;
	}
	if(nextLightIndex < fxInput.lights.size())
		fxInput.lights[nextLightIndex].first = 0;

	for(unsigned pass = 0; pass < passCount; ++pass)
	{
		for(size_t q = 0; q < ((maxActors>0)?maxActors: scene.mBlueprint.actors.size()); ++q)
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
				matrixState.applyWorldMatrix(rc, nativeMatrix, fxInput);

			RenderableMesh const& mesh = *scene.mResources.meshes[actor.meshIndex].renderable;
			if(actor.materials.empty())
			{
				ScePspFVector4 const black = {0,0,0,1};
				ScePspFVector4 const white = {0,0,0,1};

				fxInput.vecs[BaseEffect::AmbientColor] = black;
				fxInput.vecs[BaseEffect::DiffuseColor] = white;
				fxInput.vecs[BaseEffect::SpecularColor] = white;

				fx.pass(pass);
				render(rc, mesh);
			}
			else
			for(unsigned int materialIt = 0; materialIt < actor.materials.size(); ++materialIt)
			{
				unsigned int textureIndex = actor.materials[materialIt].textureIndex;
				// $TBD: texture support
				//fxInput.textures[BaseEffect::DiffuseTexture] = 
				//	((textureIndex != ~0U)? scene.mNativeResources.textures[textureIndex]: 0);

				toNative(actor.materials[materialIt].ambient, fxInput.vecs[BaseEffect::AmbientColor]);
				toNative(actor.materials[materialIt].diffuse, fxInput.vecs[BaseEffect::DiffuseColor]);
				toNative(actor.materials[materialIt].specular, fxInput.vecs[BaseEffect::SpecularColor]);

				fx.pass(pass);
				render(rc, mesh, materialIt);
			}
		}
	}
	fx.end();
}

////////////////////////////////////////////////

void CSkinnedAlgos::processSkinMesh(RenderableMesh& mesh, BoneMapT const& boneMap, CTransform::t_matrix const* matrices)
{
	assert(mesh.mBlueprint.skinInfo);
	assert(!boneMap.empty());

	// $HACK: hardcoded offsets
	// $TBD: calc offsets using vertexDecl
	size_t weightsOffset = 0U;
	size_t normalsOffset = weightsOffset + singleWeightSize(mesh.mBlueprint.vertexDecl)*mesh.mBlueprint.skinInfo->weightsPerVertex;
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

		reinterpret_cast<Vec3*>(dstRaw + sizeof(Vec3)),							// dstPositions
		reinterpret_cast<Vec3*>(dstRaw ),										// dstNormals

		mesh.mBlueprint.vertexStride,											// srcVertexStride
		mesh.mBlueprint.vertexStride,											// srcWeightStride
		mesh.mBlueprint.boneIndexStride,										// srcBoneIndexStride
		sizeof(Vec3)*2,															// dstVertexStride
		mesh.mBlueprint.vertexCount,

		*mesh.mBlueprint.skinInfo, boneMap, matrices);
}
