#include "pspScenePlayer.h"
#include "../ScenePlayer.h"

#include <memory>
#include <effects/all.h>
#include <effects/Library.h>
#include <effects/BaseEffect.h>

#include "pspvfpu.h"

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
//;;printf(" render -- 1\n");

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
namespace {
struct pspvfpu_context *sp_vfpucontext;
void xformPosNormal(ScePspFVector4* outPos, ScePspFVector4* outNormal, 
	ScePspFVector3 const* inPos, ScePspFVector3 const* inNormal, ScePspFMatrix4 const* boneMatrix, float weight)
{
//	pspvfpu_use_matrices(sp_vfpucontext, 0, VMAT0 | VMAT1 | VMAT3); 

//	vm0
//		[m]
//	vm1
//		[outPos	outNormal	inPos	inNormal]
//	vm2
//		[tmpV.x	w	0	0]
//		[tmpV.y	0	0	0]
//		[tmpV.z	0	0	0]
//		[0		0	0	0]


	__asm__ volatile (
       "lv.q   C100, %0\n"
       "lv.q   C110, %1\n"

	   "ulv.q   C120, %2\n"
       "ulv.q   C130, %3\n"
       "lv.q   C000, 0  + %4\n"
       "lv.q   C010, 16 + %4\n"
       "lv.q   C020, 32 + %4\n"
       "lv.q   C030, 48 + %4\n"
       "mtv    %5,   S210\n"


       "vdot.t S200, R000, C120\n"
       "vdot.t S201, R001, C120\n"
       "vdot.t S202, R002, C120\n"
	   "vadd.t C200, C030, C200\n" 

	   "vscl.t C200, C200, S210\n"
	   "vadd.t C100, C100, C200\n"


       "vdot.t S200, R000, C130\n"
       "vdot.t S201, R001, C130\n"
       "vdot.t S202, R002, C130\n"

	   "vscl.t C200, C200, S210\n"
	   "vadd.t C110, C110, C200\n"


       "sv.q   C100, %0\n"
       "sv.q   C110, %1\n"
	   : "+m"(*outPos), "+m"(*outNormal): "m"(*inPos), "m"(*inNormal), "m"(*boneMatrix), "r"(weight));

}

inline void xformPosNormal_begin(/*ScePspFVector4 const* pos, ScePspFVector4 const* normal,*/
	ScePspFVector3 const* inPos, ScePspFVector3 const* inNormal)
{
	__asm__ volatile (
	   "vzero.q  C100\n"
	   "vzero.q  C110\n"
	   "ulv.q   C120, %0\n"
       "ulv.q   C130, %1\n"
	   :: "m"(*inPos), "m"(*inNormal));
/*	__asm__ volatile (
	   "vmov.q  C100, C000[ 0, 0, 0, 0]\n"
	   "vmov.q  C110, C000[ 0, 0, 0, 0]\n"
	   "ulv.q   C120, %0\n"
       "ulv.q   C130, %1\n"
	   :: "m"(*inPos), "m"(*inNormal));*/
}
inline void xformPosNormal_begin2(ScePspFVector4 const* pos, ScePspFVector4 const* normal,
	ScePspFVector3 const* inPos, ScePspFVector3 const* inNormal)
{
	__asm__ volatile (
       "lv.q   C100, %0\n"
       "lv.q   C110, %1\n"
	   "ulv.q   C120, %2\n"
       "ulv.q   C130, %3\n"
	   :: "m"(*pos), "m"(*normal), "m"(*inPos), "m"(*inNormal));
}
inline void xformPosNormal_fn(ScePspFMatrix4 const* boneMatrix, float weight)
{
	__asm__ volatile (
       "lv.q   C000, 0  + %0\n"
       "lv.q   C010, 16 + %0\n"
       "lv.q   C020, 32 + %0\n"
       "lv.q   C030, 48 + %0\n"
       "mtv    %1,   S210\n"


       "vdot.t S200, R000, C120\n"
       "vdot.t S201, R001, C120\n"
       "vdot.t S202, R002, C120\n"
	   "vadd.t C200, C030, C200\n" 

	   "vscl.t C200, C200, S210\n"
	   "vadd.t C100, C100, C200\n"


       "vdot.t S200, R000, C130\n"
       "vdot.t S201, R001, C130\n"
       "vdot.t S202, R002, C130\n"

	   "vscl.t C200, C200, S210\n"
	   "vadd.t C110, C110, C200\n"
	   :: "m"(*boneMatrix), "r"(weight));
}
inline void xformPosNormal_end(ScePspFVector4* pos, ScePspFVector4* normal)
{
	__asm__ volatile (
       "sv.q   C100, %0\n"
       "sv.q   C110, %1\n"

	   : "+m"(*pos), "+m"(*normal));
}

void nativeProcessSkinMesh(Vec3 const* srcPositions, Vec3 const* srcNormals, float const* srcWeights, unsigned char const* srcBoneIndices,
	Vec3 *dstPositions, Vec3* dstNormals, size_t srcVertexStride, size_t srcWeightStride, size_t srcBoneIndexStride,
	size_t dstVertexStride, size_t vertexCount,
	mutalisk::data::skin_info const& skinInfo, CSkinnedAlgos::BoneMapT const& boneMap, CTransform::t_matrix const* matrices)
{
	if (sp_vfpucontext == NULL)
		sp_vfpucontext = pspvfpu_initcontext();

	static std::vector<ScePspFMatrix4> worldMatrices;
	worldMatrices.resize(boneMap.size());

	unsigned i = 0;
	CSkinnedAlgos::BoneMapT::const_iterator bIdIt = boneMap.begin();
	for( ; (i < boneMap.size()) && (bIdIt != boneMap.end()); ++bIdIt, ++i )
	{
		CTransform::t_matrix tm;
		float const* mat16 = skinInfo.bones[ bIdIt->first ].matrix.data;
		setMatrix(tm, mat16);

		CTransform::t_matrix itm;
		Mat34_invertOrthogonal(&itm, &tm);

		toNative(worldMatrices[i], (matrices[ bIdIt->second ] * itm));
	}

	CTransform::t_matrix identityM;
	Mat34_setIdentity( &identityM );
	while( i < worldMatrices.size() )
		toNative(worldMatrices[i++], identityM);

	unsigned char const* srcPositionsRaw = reinterpret_cast<unsigned char const*>(srcPositions);
	unsigned char const* srcNormalsRaw = reinterpret_cast<unsigned char const*>(srcNormals);
	unsigned char const* srcWeightsRaw = reinterpret_cast<unsigned char const*>(srcWeights);
	unsigned char const* srcBoneIndicesRaw = reinterpret_cast<unsigned char const*>(srcBoneIndices);

	unsigned char* dstPositionsRaw = reinterpret_cast<unsigned char*>(dstPositions);
	unsigned char* dstNormalsRaw = reinterpret_cast<unsigned char*>(dstNormals);
	for( size_t q = 0; q < vertexCount; ++q )
	{
		ScePspFVector3 const* pos3 = reinterpret_cast<ScePspFVector3 const*>(srcPositionsRaw);
		ScePspFVector3 const* nrm3 = reinterpret_cast<ScePspFVector3 const*>(srcNormalsRaw);

		float const* weights = reinterpret_cast<float const*>(srcWeightsRaw);
		unsigned char const* boneIndices = srcBoneIndicesRaw;
		ScePspFVector4 accumP3 = {0.0f, 0.0f, 0.0f, 0.0f};
		ScePspFVector4 accumN3 = {0.0f, 0.0f, 0.0f, 0.0f};
		
		pspvfpu_use_matrices(0, 0, 0);
		xformPosNormal_begin(pos3, nrm3);
		for( size_t w = 0; w < skinInfo.weightsPerVertex; ++w )
		{
			unsigned char boneId = boneIndices[w];
			ASSERT(boneId < worldMatrices.size());
			float boneWeight = weights[w];

			if(boneWeight < 0.001f)
				continue;

			xformPosNormal_fn(
				&worldMatrices[boneId], boneWeight);

//			xformPosNormal(
//				&accumP3, &accumN3,
//				pos3, nrm3,
//				&worldMatrices[boneId], boneWeight);
		}
		pspvfpu_use_matrices(0, 0, 0);
		xformPosNormal_end(&accumP3, &accumN3);

		*(reinterpret_cast<float*>(dstPositionsRaw) +0) = accumP3.x;
		*(reinterpret_cast<float*>(dstPositionsRaw) +1) = accumP3.y;
		*(reinterpret_cast<float*>(dstPositionsRaw) +2) = accumP3.z;
		*(reinterpret_cast<float*>(dstNormalsRaw) +0) = accumN3.x;
		*(reinterpret_cast<float*>(dstNormalsRaw) +1) = accumN3.y;
		*(reinterpret_cast<float*>(dstNormalsRaw) +2) = accumN3.z;

		srcPositionsRaw += srcVertexStride;
		srcNormalsRaw += srcVertexStride;
		srcWeightsRaw += srcWeightStride;
		srcBoneIndicesRaw += srcBoneIndexStride;

		dstPositionsRaw += dstVertexStride;
		dstNormalsRaw += dstVertexStride;
	}
}
}


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

//	processSkinMesh(
	nativeProcessSkinMesh(
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
