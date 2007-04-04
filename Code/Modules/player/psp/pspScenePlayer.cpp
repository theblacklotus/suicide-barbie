#include "pspScenePlayer.h"
#include "../ScenePlayer.h"

#include <memory>
#include <effects/all.h>
#include <effects/Library.h>
#include <effects/BaseEffect.h>

extern "C" {
	#include <Base/Std/Std.h>
	#include <Base/Math/Lin.h>
}

namespace mutalisk
{
	using namespace effects;
	bool gDelayedTextureLoading = false;

////////////////////////////////////////////////
std::auto_ptr<RenderableTexture> prepare(RenderContext& rc, mutalisk::data::texture const& data)
{
	std::auto_ptr<RenderableTexture> texture(new RenderableTexture(data));
	return texture;
}
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
	if (!gDelayedTextureLoading)
	{
		scene->mResources.textures.resize(data.textureIds.size());
		for(size_t q = 0; q < data.textureIds.size(); ++q)
		{
			scene->mResources.textures[q].blueprint = loadResource<mutalisk::data::texture>(pathPrefix + data.textureIds[q]);
			scene->mResources.textures[q].renderable = prepare(rc, *scene->mResources.textures[q].blueprint);
		}
	}
	for(size_t q = 0; q < data.textureIds.size(); ++q)
	{
		printf("¤¤ texture = %x\n", (unsigned)scene->mResources.textures[q].blueprint.get());
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
		return 0U;
	}
	size_t vertexElementInfo(unsigned int vertexDecl, unsigned int elementBits, size_t* elementSize = 0)
	{
		//MUTALISK_NOT_IMPLEMENTED("vertexElementInfo");
		return 0;
	}
}

		struct Data
		{
			Vec3 pos;
			float area;
		};
		struct Ball
		{
			u32 color;
			Vec3 normal;
			Vec3 pos;
			float area;
			bool operator < (const Ball& other) const 
			{
				if (pos.z > other.pos.z)
					return false;
				else
					return true;
			}
		};
std::auto_ptr<RenderableMesh> prepare(RenderContext& rc, mutalisk::data::mesh const& data)
{
	std::auto_ptr<RenderableMesh> mesh(new RenderableMesh(data));
	if(data.skinInfo)
	{
		;;printf("has skinInfo \n");
		// for CPU skinning remove weights from the vertex buffer

		size_t skipOffset = 0;//singleWeightSize(data.vertexDecl) * data.skinInfo->weightsPerVertex;
		size_t newVertexStride = data.vertexStride - skipOffset;
		mesh->mAmplifiedVertexData[0] = new unsigned char[newVertexStride * data.vertexCount];
		mesh->mAmplifiedVertexDecl = data.vertexDecl & (~(GU_WEIGHTS_BITS|GU_WEIGHT_BITS));
		mesh->mAmplifiedVertexStride = newVertexStride;

		unsigned char* dst = mesh->mAmplifiedVertexData[0];
		unsigned char const* src = data.vertexData + skipOffset;
		for(size_t q = 0; q < data.vertexCount; ++q)
		{
			// copy 1 vertex without weights
			memcpy(dst, src, newVertexStride);
			dst += newVertexStride;
			src += data.vertexStride;
		}

		mesh->mAmplifiedVertexData[1] = new unsigned char[newVertexStride * data.vertexCount];
		memcpy(mesh->mAmplifiedVertexData[1], mesh->mAmplifiedVertexData[0], newVertexStride * data.vertexCount);
		mesh->mAmplifiedBufferIndex = 0;

		;;printf("skinInfo processed\n");
	}
	else if(!false)		// "ball render"(tm) technique
	{
		;;printf("using \"ball render\"(tm) technique\n");

		if ( (data.vertexDecl & GU_COLOR_8888) == 0 ) 
		{
			printf("ASSERT: need vertex color for ball render!\n");
			assert(false);
		}

		// this is what the input must look like, and what the output will look like.. simple, huh?
		struct Vertex
		{
			float u, v;
			u32 color;
			Vec3 normal;
			Vec3 pos;
		};

		if (data.primitiveType != GU_TRIANGLES) 
		{
			printf("ASSERT: input needs to be triangle list!\n");
			assert(false);
		}

		if (data.vertexStride != sizeof(Vertex))
		{
			printf("ASSERT: vertex does not match ball render vertex!\n");
			assert(false);
		}

		if(data.indexData == 0 /*|| (data.vertexDecl & GU_INDEX_16BIT) == 0*/)
		{
			printf("ASSERT: source is not indexed or not 16bits!\n");
			assert(false);
		}

		Vertex* src = (Vertex*)data.vertexData;
		u16* index = (u16*)data.indexData;

		u32 skipped = 0;
		float scale = 1000.f;
		float areaThreshold = 0.f;

		std::vector<Ball> balls;
		balls.reserve(data.indexCount / 3);

		for(size_t i = 0; i < data.indexCount; i+=3)
		{
			u32 i0 = *index++;
			u32 i1 = *index++;
			u32 i2 = *index++;

			Vertex v0 = src[i0];
			Vertex v1 = src[i1];
			Vertex v2 = src[i2];

			Vec3& p0 = v0.pos;
			Vec3& p1 = v1.pos;
			Vec3& p2 = v2.pos;

			Vec3_scale(&p0, &p0, scale);
			Vec3_scale(&p1, &p1, scale);
			Vec3_scale(&p2, &p2, scale);

			// calc area

			Vec3 d0, d1, c;
			Vec3_sub(&d0, &p1, &p0);
			Vec3_sub(&d1, &p2, &p0);
			Vec3_cross(&c, &d0, &d1);
			float area = Vec3_length(&c) * 0.5f;

			// calc midpoint (barycentric)

			Vec3 p;
			Vec3_scale(&d0, &d0, 0.5f);
			Vec3_scale(&d1, &d1, 0.5f);
			Vec3_add(&p, &d0, &d1);
			Vec3_add(&p, &p0, &p);

			Vec3_scale(&p, &p, 1.f / scale);

			if (area < areaThreshold)
			{
				skipped++;
				continue;
			}

			Ball ball;
			ball.color = v0.color;
			ball.normal = v0.normal;
			ball.pos = p;
			ball.area = sqrtf(area) / 1300.f;
			ball.area += ball.area * 0.25f;

			balls.push_back(ball);
		}
		printf("%i out of %i triangles skipped\n", skipped, data.indexCount/3);

		std::sort(balls.begin(), balls.end());

		size_t vertexCount = balls.size() * 2 /*one sprite equals 2 verts*/;
		size_t vertexStride = sizeof(Vertex);
		mesh->mAmplifiedVertexData[0] = new unsigned char[vertexStride * vertexCount];
		mesh->mAmplifiedVertexDecl = GU_VERTEX_32BITF | GU_NORMAL_32BITF | GU_TEXTURE_32BITF | GU_COLOR_8888;
		mesh->mAmplifiedVertexStride = vertexStride;
		mesh->mUserData = new unsigned char[sizeof(Data) * vertexCount];

		Vertex* dst = (Vertex*)mesh->mAmplifiedVertexData[0];
		Data* out   = (Data*)mesh->mUserData;
		Vec3 zeroVec = {0,0,0};
		for(std::vector<Ball>::const_iterator it = balls.begin(); it != balls.end(); ++it)
		{
			const Ball& ball = *it;

			Vertex v0, v1;
			v0.u = v0.v = 0.f;
			v1.u = v1.v = 1.f;
			v0.color = v1.color = ball.color;
			v0.normal = v1.normal = ball.normal;
			v0.pos = v1.pos = zeroVec;
			Data data;
			data.pos = ball.pos;
			data.area = ball.area;
			*dst++ = v0;
			*dst++ = v1;
			*out++ = data;
		}

		mutalisk::data::mesh& dataHack = const_cast<mutalisk::data::mesh&>(data);
		delete dataHack.indexData; dataHack.indexData = 0;				// $HACK ; will force raw vertex data when rendering
		dataHack.primitiveType = GU_SPRITES;							// $HACK ; primtype is now sprites
		dataHack.vertexCount = ((data.indexCount/3) - skipped) * 2;		// $HACK ; adjust vertex count

		mesh->mAmplifiedVertexData[1] = new unsigned char[vertexStride * vertexCount];
		memcpy(mesh->mAmplifiedVertexData[1], mesh->mAmplifiedVertexData[0], vertexStride * vertexCount);
		mesh->mAmplifiedBufferIndex = 0;

		;;printf("done processing \"ball render\"(tm) technique\n");
	}

	return mesh;
}

namespace {
	/*struct MatrixState
	{
		void applyWorldMatrix(RenderContext const& rc, ScePspFMatrix4 const& inWorldMatrix, BaseEffect::Input& dst)
		{
			world = inWorldMatrix;

			gumFastInverse(&invWorld, &world);
			gumMultMatrix(&worldViewProj, &rc.viewProjMatrix, &world);

			dst.matrices[BaseEffect::WorldMatrix] = &world;
			dst.matrices[BaseEffect::ViewMatrix] = &rc.viewMatrix;
			dst.matrices[BaseEffect::ProjMatrix] = &rc.projMatrix;
			dst.matrices[BaseEffect::ViewProjMatrix] = &rc.viewProjMatrix;
			dst.matrices[BaseEffect::WorldViewProjMatrix] = &worldViewProj;
			dst.matrices[BaseEffect::InvWorldMatrix] = &invWorld;
		}

		ScePspFMatrix4 world;
		ScePspFMatrix4 worldViewProj;
		ScePspFMatrix4 invWorld;
	};*/

	void toNative(ScePspFMatrix4& dst, CTransform::t_matrix const& src)
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
	void toNative(CTransform::t_matrix& worldMatrix, float const* worldMatrixData)
	{
		worldMatrix = CTransform::t_matrix(
			worldMatrixData[8], worldMatrixData[4], worldMatrixData[0],
			worldMatrixData[9], worldMatrixData[5], worldMatrixData[1],
			worldMatrixData[10], worldMatrixData[6], worldMatrixData[2],
			worldMatrixData[14], worldMatrixData[13], worldMatrixData[12]);
	}
	void toNative(ScePspFMatrix4& dst, float const* srcMatrixData)	
	{
		memcpy(&dst, srcMatrixData, sizeof(float)*16);
	}
	Vec3 const& getTranslation(Vec3& translation, ScePspFMatrix4 const& nativeMatrix)
	{
		translation.x = nativeMatrix.w.x;
		translation.y = nativeMatrix.w.y;
		translation.z = nativeMatrix.w.z;
		return translation;
	}

	void toNative(ScePspFVector4& dst, mutalisk::data::Vec4 const& src)
	{
		dst.x = src[0]; dst.y = src[1]; dst.z = src[2]; dst.w = src[3];
	}
	void toNative(unsigned int& dst, mutalisk::data::Color const& src)
	{
//;;printf("$toNative(%f, %f, %f, %f)\n", src.r, src.g, src.b, src.a);
		dst = (unsigned int)(src.r * 255.0f);
//;;printf(" toNative -- 1\n");
		dst |= (unsigned int)(src.g * 255.0f) << 8;
//;;printf(" toNative -- 2\n");
		dst |= (unsigned int)(src.b * 255.0f) << 16;
//;;printf(" toNative -- 3\n");
		dst |= (unsigned int)(src.a * 255.0f) << 24;
//;;printf("!toNative\n");
	}

	void toNative(BaseEffect::Input::Surface& dst, RenderableScene const& scene, mutalisk::data::shader_fixed const& src)
	{
//;;printf("$blastSurfaceInputs -- toNative(%d, %d, %d)\n", (int)&dst, (int)&scene, (int)&src);

		toNative(dst.ambient, src.ambient);
		toNative(dst.diffuse, src.diffuse);
		toNative(dst.specular, src.specular);
		toNative(dst.emissive, src.emissive);

//;;printf(" blastSurfaceInputs -- toNativeColors\n");

//		printf("¤¤ src.diffuseTexture = %x\n", src.diffuseTexture);

		dst.diffuseTexture = (src.diffuseTexture != ~0U) ? scene.mResources.textures[src.diffuseTexture].blueprint.get() : 0;
		dst.envmapTexture = (src.envmapTexture != ~0U) ? scene.mResources.textures[src.envmapTexture].blueprint.get() : 0;
//		printf("¤¤ dst.diffuseTexture = %x\n", dst.diffuseTexture);

//		dst.diffuseTexture = (src.diffuseTexture != ~0U)? scene.mResources.textures[src.diffuseTexture] : 0;
//		dst.envmapTexture = 0(src.envmapTexture != ~0U)? scene.mNativeResources.textures[src.envmapTexture] : 0;

		dst.uOffset = src.uOffset;
		dst.vOffset = src.vOffset;
		dst.transparency = src.transparency;
		dst.dummy = 0;

		typedef mutalisk::data::shader_fixed	Op;
		switch(src.frameBufferOp)
		{
		case Op::fboReplace:
			dst.srcBlend = GU_FIX; dst.srcFix = ~0U;
			dst.dstBlend = GU_FIX; dst.dstFix =  0U;
			break;
		case Op::fboAdd:
			dst.srcBlend = GU_FIX; dst.srcFix = ~0U;
			dst.dstBlend = GU_FIX; dst.dstFix = ~0U;
			break;
		case Op::fboSub:
			ASSERT("Not supported");
			// not supported, do closest match instead (mul)
			dst.srcBlend = GU_DST_COLOR;
			dst.dstBlend = GU_FIX; dst.dstFix = 0U;
			break;
		case Op::fboLerp:
			dst.srcBlend = GU_SRC_ALPHA;
			dst.dstBlend = GU_ONE_MINUS_SRC_ALPHA;
			break;
		case Op::fboMul:
			dst.srcBlend = GU_DST_COLOR;
			dst.dstBlend = GU_FIX; dst.dstFix = 0U;
			break;
		case Op::fboMadd:
			dst.srcBlend = GU_SRC_ALPHA;
			dst.dstBlend = GU_FIX; dst.dstFix = ~0U;
			break;
		case Op::fboReject:
			dst.srcBlend = GU_FIX; dst.srcFix =  0U;
			dst.dstBlend = GU_FIX; dst.dstFix = ~0U;
			break;
		}

		toNative(dst.aux0, src.aux0);
//;;printf("!blastSurfaceInputs -- toNative\n");
	}

	void setProjection(RenderContext& rc, float fovy, float aspect)
	{
		float const zn = 1.0f;
		float const zf = 50.0f;

		// 
		float angle = (fovy / 2) * (GU_PI/180.0f);
		float cotangent = cosf(angle) / sinf(angle);

		ScePspFMatrix4 t;
		gumLoadIdentity(&t);
		t.x.x = cotangent / aspect;
		t.y.y = cotangent;
		t.z.z = zf/(zn-zf);// (far + near) / delta_z; // -(far + near) / delta_z
		t.w.z = zn*zf/(zn-zf);//2.0f * (far * near) / delta_z; // -2 * (far * near) / delta_z
		t.z.w = -1.0f;
		t.w.w = 0.0f;

		rc.projMatrix = t;
		//gumMultMatrix(&rc.projMatrix, &rc.projMatrix, &t);
	}

	void setCameraMatrix(RenderContext& rc, ScePspFMatrix4 const& camera)
	{
		ScePspFMatrix4 view = camera;

		static bool overrideCamera = true;
		gumFastInverse(&view, &view);
		if(overrideCamera)
			rc.viewMatrix = view;
		else
			gumMultMatrix(&rc.viewMatrix, &rc.viewMatrix, &view);
		gumMultMatrix(&rc.viewProjMatrix, &rc.projMatrix, &view);
	}
	
	void setWorldMatrix(ScePspFMatrix4* dst, RenderContext const& rc, ScePspFMatrix4 const& world)
	{
		ScePspFMatrix4	invWorld;
		ScePspFMatrix4	worldViewProj;

		gumFastInverse(&invWorld, &world);
		gumMultMatrix(&worldViewProj, &rc.viewProjMatrix, &world);

		dst[BaseEffect::WorldMatrix] = world;
		dst[BaseEffect::ViewMatrix] = rc.viewMatrix;
		dst[BaseEffect::ProjMatrix] = rc.projMatrix;
		dst[BaseEffect::ViewProjMatrix] = rc.viewProjMatrix;
		dst[BaseEffect::WorldViewProjMatrix] = worldViewProj;
		dst[BaseEffect::InvWorldMatrix] = invWorld;
	}

/*
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
	}*/

	void render(RenderContext& rc, RenderableMesh const& mesh, unsigned subset = 0)
	{
		unsigned char* vertexData = mesh.mBlueprint.vertexData;
		int vertexFlag = mesh.mBlueprint.vertexDecl;

		if(mesh.mAmplifiedVertexData[1-mesh.mAmplifiedBufferIndex])
		{
			vertexData = mesh.mAmplifiedVertexData[1-mesh.mAmplifiedBufferIndex];
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
/*
void render(RenderContext& rc, RenderableScene const& scene, bool animatedActors, bool animatedCamera, int maxActors, int maxLights)
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

				fxInput.textures[BaseEffect::DiffuseTexture] = (textureIndex != ~0U) ? scene.mResources.textures[textureIndex].blueprint.get() : 0;

				toNative(actor.materials[materialIt].ambient, fxInput.vecs[BaseEffect::AmbientColor]);
				toNative(actor.materials[materialIt].diffuse, fxInput.vecs[BaseEffect::DiffuseColor]);
				toNative(actor.materials[materialIt].specular, fxInput.vecs[BaseEffect::SpecularColor]);

				fx.pass(pass);
				render(rc, mesh, materialIt);
			}
		}
	}
	fx.end();
}*/


typedef RenderableScene	RenderableSceneT;
typedef RenderContext		RenderContextT;
#include "../Renderer.h"

void render(RenderContext& rc, RenderableScene const& scene, int maxActors)
{
	static bool animatedActors = true;//gSettings.forceAnimatedActors;
	static bool animatedCamera = true;//gSettings.forceAnimatedCamera;

//;;printf("$render\n");
	Vec3 cameraPos; cameraPos.x = cameraPos.y = cameraPos.z = 0.0f;
	{
		MatrixT nativeMatrix;
		if(scene.mBlueprint.defaultClipIndex == ~0U)
		{
			animatedActors = false;
			animatedCamera = false;
		}

		if(!animatedActors)
			for(size_t q = 0; q < scene.mBlueprint.actors.size(); ++q)
			{
				ASSERT(q >= 0 && q < scene.mState.actor2XformIndex.size());
				toNative(
					const_cast<RenderableScene&>(scene).mState.matrices[scene.mState.actor2XformIndex[q]],
					scene.mBlueprint.actors[q].worldMatrix.data);
			}

		if(!animatedCamera)
			for(size_t q = 0; q < scene.mBlueprint.cameras.size(); ++q)
			{
				ASSERT(q >= 0 && q < scene.mState.camera2XformIndex.size());
				toNative(
					const_cast<RenderableScene&>(scene).mState.matrices[scene.mState.camera2XformIndex[q]],
					scene.mBlueprint.cameras[q].worldMatrix.data);
			}

		if(scene.mBlueprint.defaultCameraIndex != ~0U)
		{
			unsigned int cameraIndex = scene.mBlueprint.defaultCameraIndex;
			if(animatedCamera)
			{
				ASSERT(cameraIndex >= 0 && cameraIndex < scene.mState.camera2XformIndex.size());
				toNative(nativeMatrix, scene.mState.matrices[scene.mState.camera2XformIndex[cameraIndex]]);
			}
			else
			{
				ASSERT(cameraIndex >= 0 && cameraIndex < scene.mBlueprint.cameras.size());
				toNative(nativeMatrix, scene.mBlueprint.cameras[cameraIndex].worldMatrix.data);
			}

			setProjection(rc, scene.mBlueprint.cameras[cameraIndex].fov, scene.mBlueprint.cameras[cameraIndex].aspect);
			setCameraMatrix(rc, nativeMatrix);
			getTranslation(cameraPos, nativeMatrix);
		}
	}
//;;printf(" render -- 1\n");

	CTransform::t_matrix cameraMatrix(CTransform::identityMatrix());
	if(scene.mBlueprint.defaultCameraIndex != ~0U)
	{
		unsigned int cameraIndex = scene.mBlueprint.defaultCameraIndex;
		if(animatedCamera)
		{
			ASSERT(cameraIndex >= 0 && cameraIndex < scene.mState.camera2XformIndex.size());
			cameraMatrix = scene.mState.matrices[scene.mState.camera2XformIndex[cameraIndex]];
		}
		else
		{
			ASSERT(cameraIndex >= 0 && cameraIndex < scene.mBlueprint.cameras.size());
			toNative(cameraMatrix, scene.mBlueprint.cameras[cameraIndex].worldMatrix.data);
		}
	}
	const array<data::scene::Actor>& actors = scene.mBlueprint.actors;
	mutalisk::array<RenderableScene::SharedResources::Mesh>& meshes = const_cast<RenderableScene&>(scene).mResources.meshes;
	for(size_t q = 0; q < actors.size(); ++q)
	{
		unsigned meshIndex = actors[q].meshIndex;
		ASSERT(meshIndex >= 0 && meshIndex < meshes.size());
		RenderableMesh& mesh = *meshes[meshIndex].renderable;
		if (mesh.mUserData == 0)
			continue;
		const CTransform::t_matrix& worldMatrix = scene.mState.matrices[scene.mState.actor2XformIndex[q]];
		Mat33 wc,cw;
		Mat33_transpose(&wc, const_cast<Mat33*>(&worldMatrix.Rot));
		Mat33_mul(&cw, &wc, &cameraMatrix.Rot);
		Vec3 unit2d = {1.f, 1.f, 0.f};
		Vec3 vec;
		Vec3_setMat33MulVec3(&vec, &cw, &unit2d);
		struct Vertex
		{
			float u, v;
			u32 color;
			Vec3 normal;
			Vec3 pos;
		};
		struct Data
		{
			Vec3 pos;
			float area;
		};
		mesh.mAmplifiedBufferIndex = 1 - mesh.mAmplifiedBufferIndex;
		Vertex* vertexData = (Vertex*)mesh.mAmplifiedVertexData[mesh.mAmplifiedBufferIndex];
		Data* data = (Data*)mesh.mUserData;
		size_t primCount = mesh.mBlueprint.vertexCount / 2;
		Vec3 p0, p1, v;
		static size_t count = 0;
		if (count < primCount)
			count+= 1; //primCount / (60 * 3);
		static float t = 0;
		const static float t1 = 1.f / 520.f;
		static bool fadeOut = false;
		if (t > 5.f)
			fadeOut = true;
		if (fadeOut)
			t -= 1.f / 60.f;
		else
			t += 1.f / 60.f;
		float t0 = t1;
		for (size_t j = 0; j < primCount; ++j)
		{
			Data& d = *data++;
			Vec3_scale(&v, &vec, d.area);

			t0 += t1;
			float td = (t - t0) * 2.5f;
			if (td <= 0.f)
				v.x = v.y = v.z = 0.f;
			else if (0.f < td && td <= 1.f)
			{
				float const up = 1.2f;
				float a = 3.142592f - asinf(1.f/up);
				float s = up * sinf(td*a);
				Vec3_scale(&v, &v, s);
			}



			Vec3_sub(&p0, &d.pos, &v);
			Vec3_add(&p1, &d.pos, &v);
			vertexData->pos = p0; vertexData++;
			vertexData->pos = p1; vertexData++;
		}
	}

	static std::vector<InstanceInput> instanceInputs;
	static std::vector<BaseEffect::Input::Surface> surfaceInputs;
	static std::vector<RenderBlock> bgRenderBlocks, opaqueRenderBlocks, transparentRenderBlocks, fgRenderBlocks;

	instanceInputs.resize(0);
	surfaceInputs.resize(0); 
	bgRenderBlocks.resize(0); fgRenderBlocks.resize(0); 
	opaqueRenderBlocks.resize(0); transparentRenderBlocks.resize(0); 

	static std::vector<mutalisk::data::scene::Actor const*> visibleActors;
	visibleActors.resize(0);

	RenderContext& camera = rc; // @TBD:
	findVisibleActors(camera, 0) (scene.mBlueprint.actors, visibleActors);
//;;printf(" render -- findVisibleActors\n");
	blastInstanceInputs(scene, camera) (visibleActors, instanceInputs);
//;;printf(" render -- blastInstanceInputs\n");
	blastSurfaceInputs(scene, 0) (visibleActors, surfaceInputs);
//;;printf(" render -- blastSurfaceInputs\n");
	blastRenderBlocks(scene, cameraPos) (visibleActors, bgRenderBlocks, opaqueRenderBlocks, transparentRenderBlocks, fgRenderBlocks);
//;;printf(" render -- blastRenderBlocks\n");
	sortRenderBlocks()(transparentRenderBlocks);
//;;printf(" render -- sortRenderBlocks\n");
	if(instanceInputs.empty() || surfaceInputs.empty())
	{
		ASSERT(visibleActors.empty());
	}
	else
	{
		BaseEffect::Input::BufferControl background;
		background.colorWriteEnable = true;
		background.zWriteEnable = false;
		background.zReadEnable = true;
		background.zEqual = false;

		BaseEffect::Input::BufferControl opaque[2];
		opaque[0].colorWriteEnable = true;
		opaque[0].zWriteEnable = true;
		opaque[0].zReadEnable = true;
		opaque[0].zEqual = false;
		// zpass
		opaque[1] = opaque[0];
		opaque[1].colorWriteEnable = false;

		BaseEffect::Input::BufferControl transparent[2];
		transparent[0].colorWriteEnable = true;
		transparent[0].zWriteEnable = false;
		transparent[0].zReadEnable = true;
		transparent[0].zEqual = false;
		// zpass
		transparent[1] = transparent[0];
		transparent[1].zEqual = true;

		BaseEffect::Input::BufferControl foreground;
		foreground.colorWriteEnable = true;
		foreground.zWriteEnable = false;
		foreground.zReadEnable = false;
		foreground.zEqual = false;

		drawRenderBlocks draw(rc, scene, 
			&instanceInputs[0], instanceInputs.size(), &surfaceInputs[0], surfaceInputs.size());
		
//;;printf(" render -- drawRenderBlocks\n");
		draw(opaqueRenderBlocks,		opaque[0], opaque[1]);
//;;printf(" render -- draw 1\n");
		draw(bgRenderBlocks,			background, background);
//;;printf(" render -- draw 2\n");
		draw(transparentRenderBlocks,	transparent[0], transparent[1]);
//;;printf(" render -- draw 3\n");
//		draw(fgRenderBlocks,			foreground, foreground);
//;;printf(" render -- draw 4\n");
	}
//;;printf("!render\n");
}

////////////////////////////////////////////////
void CSkinnedAlgos::processSkinMesh(RenderableMesh& mesh, BoneMapT const& boneMap, CTransform::t_matrix const* matrices)
{
	assert(mesh.mBlueprint.skinInfo);
	assert(!boneMap.empty());

	// $HACK: hardcoded offsets
	// $TBD: calc offsets using vertexDecl
	
	size_t offset = 
		(((mesh.mBlueprint.vertexDecl & GU_TEXTURE_32BITF) == GU_TEXTURE_32BITF)? 8U: 0U) +
		(((mesh.mBlueprint.vertexDecl & GU_COLOR_8888) == GU_COLOR_8888)? 4U: 0U);
	size_t normalsOffset = offset;
	size_t positionsOffset = normalsOffset + sizeof(Vec3);
	size_t weightsOffset = 0U;
	size_t boneIndicesOffset = 0U;

	mesh.mAmplifiedBufferIndex = 1 - mesh.mAmplifiedBufferIndex;
	unsigned char* dstRaw = mesh.mAmplifiedVertexData[mesh.mAmplifiedBufferIndex];
	unsigned char const* srcRaw = mesh.mBlueprint.vertexData;
	unsigned char const* srcBoneWeights = mesh.mBlueprint.weightData;
	unsigned char const* srcBoneIndices = mesh.mBlueprint.boneIndexData;

	processSkinMesh(
		reinterpret_cast<Vec3 const*>(srcRaw + positionsOffset),
		reinterpret_cast<Vec3 const*>(srcRaw + normalsOffset),
		reinterpret_cast<float const*>(srcBoneWeights + weightsOffset),
		reinterpret_cast<unsigned char const*>(srcBoneIndices + boneIndicesOffset),

		reinterpret_cast<Vec3*>(dstRaw + positionsOffset),						// dstPositions
		reinterpret_cast<Vec3*>(dstRaw + normalsOffset),						// dstNormals

		mesh.mBlueprint.vertexStride,											// srcVertexStride
		mesh.mBlueprint.weightStride,											// srcWeightStride
		mesh.mBlueprint.boneIndexStride,										// srcBoneIndexStride

		mesh.mAmplifiedVertexStride,											// dstVertexStride
		mesh.mBlueprint.vertexCount,

		*mesh.mBlueprint.skinInfo, boneMap, matrices);
}

} // namespace mutalisk
