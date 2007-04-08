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

RenderContext::RenderContext()
:	znear(1.0f)
,	zfar(50.0f)
{
	gumLoadIdentity(&viewMatrix);
	gumLoadIdentity(&projMatrix);
	gumLoadIdentity(&viewProjMatrix);
}

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
		for(size_t q = 0; q < data.textureIds.size(); ++q)
		{
			printf("¤¤ texture = %x\n", (unsigned)scene->mResources.textures[q].blueprint.get());
		}
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
	return mesh;
}

namespace {
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
	/*void toNative(CTransform::t_matrix& worldMatrix, float const* worldMatrixData)
	{
		worldMatrix = CTransform::t_matrix(
			worldMatrixData[8], worldMatrixData[4], worldMatrixData[0],
			worldMatrixData[9], worldMatrixData[5], worldMatrixData[1],
			worldMatrixData[10], worldMatrixData[6], worldMatrixData[2],
			worldMatrixData[14], worldMatrixData[13], worldMatrixData[12]);
	}*/
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
		dst.uScale = src.uScale;
		dst.vScale = src.vScale;
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
		switch(src.xTexWrapOp)
		{
		case Op::twoClamp:
			dst.xTexWrap = GU_CLAMP;
			break;
		case Op::twoRepeat:
			dst.xTexWrap = GU_REPEAT;
			break;
		}
		switch(src.yTexWrapOp)
		{
		case Op::twoClamp:
			dst.yTexWrap = GU_CLAMP;
			break;
		case Op::twoRepeat:
			dst.yTexWrap = GU_REPEAT;
			break;
		}

		toNative(dst.aux0, src.aux0);
//;;printf("!blastSurfaceInputs -- toNative\n");
	}

	void setProjection(RenderContext& rc, float fovy, float aspect)
	{
  		float const zn = rc.znear;//1.0f;
  		float const zf = rc.zfar;//50.0f;


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

	void render(RenderContext& rc, RenderableMesh const& mesh, unsigned subset = 0)
	{
		unsigned char* vertexData = mesh.mBlueprint.vertexData;
		int vertexFlag = mesh.mBlueprint.vertexDecl;

		if(mesh.mAmplifiedVertexData[mesh.mAmplifiedBufferIndex])
		{
			vertexData = mesh.mAmplifiedVertexData[mesh.mAmplifiedBufferIndex];
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
		/*MatrixT nativeMatrix;
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
		}*/


		if(scene.mState.activeCameraIndex != ~0U)
		{
			MatrixT nativeMatrix;
			toNative(nativeMatrix, scene.mState.cameraMatrix);

			ASSERT(scene.mState.activeCameraIndex >= 0 && scene.mState.activeCameraIndex < scene.mBlueprint.cameras.size());
			setProjection(rc,
				scene.mBlueprint.cameras[scene.mState.activeCameraIndex].fov,
				scene.mBlueprint.cameras[scene.mState.activeCameraIndex].aspect);
			setCameraMatrix(rc, nativeMatrix);

			cameraPos = scene.mState.cameraMatrix.Move;
		}
	}
//;;printf(" render -- 1\n");

	/*CTransform::t_matrix cameraMatrix(CTransform::identityMatrix());
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
	}*/

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
