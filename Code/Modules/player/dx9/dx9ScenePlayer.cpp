#include "dx9ScenePlayer.h"
#include "../ScenePlayer.h"

#include <memory>
#include <d3d9types.h>
#include <effects/all.h>
#include <effects/Library.h>
#include <effects/BaseEffect.h>

using namespace mutalisk;
using namespace mutalisk::effects;

namespace {
	unsigned int indices2Faces(unsigned int indexCount, D3DPRIMITIVETYPE primitiveType)
	{
		switch(primitiveType)
		{
		case D3DPT_POINTLIST:
			return indexCount;
		case D3DPT_LINELIST:
			return indexCount / 2;
		case D3DPT_LINESTRIP:
			return indexCount - 1;
		case D3DPT_TRIANGLELIST:
			return indexCount / 3;
		case D3DPT_TRIANGLESTRIP:
		case D3DPT_TRIANGLEFAN:
			return indexCount - 2;
		default:
			ASSERT(!"Unknown primitive type");
			return 0;
		}
	}

	static unsigned int faces2Indices(unsigned int  faceCount, D3DPRIMITIVETYPE primitiveType)
	{
		switch(primitiveType)
		{
		case D3DPT_POINTLIST:
			return faceCount;
		case D3DPT_LINELIST:
			return faceCount * 2;
		case D3DPT_LINESTRIP:
			return faceCount + 1;
		case D3DPT_TRIANGLELIST:
			return faceCount * 3;
		case D3DPT_TRIANGLESTRIP:
		case D3DPT_TRIANGLEFAN:
			return faceCount + 2;
		default:
			ASSERT(!"Unknown primitive type");
			return 0;
		}
	}
} // namespace

////////////////////////////////////////////////
//template <>
static com_ptr<IDirect3DTexture9> prepare(RenderContext& rc, std::string fileName)
{
	com_ptr<IDirect3DTexture9> resource;
	DX_MSG("load texture") = 
		D3DXCreateTextureFromFileA(rc.device, fileName.c_str(), &resource);
	return resource;
}

////////////////////////////////////////////////
struct Dx9RenderableScene : public RenderableScene
{
	Dx9RenderableScene(mutalisk::data::scene const& blueprint) : RenderableScene(blueprint) {}

	struct NativeSharedResources {
		mutalisk::array<com_ptr<IDirect3DTexture9> >	textures;
	};

	NativeSharedResources	mNativeResources;
};
////////////////////////////////////////////////
std::auto_ptr<Dx9RenderableScene> prepare(RenderContext& rc, mutalisk::data::scene const& data)
{
	std::auto_ptr<Dx9RenderableScene> scene(new Dx9RenderableScene(data));

	// load shared resources
	scene->mResources.meshes.resize(data.meshIds.size());
	for(size_t q = 0; q < data.meshIds.size(); ++q)
	{
		scene->mResources.meshes[q].blueprint = loadResource<mutalisk::data::mesh>(data.meshIds[q]);
		scene->mResources.meshes[q].renderable = prepare(rc, *scene->mResources.meshes[q].blueprint);
	}
	scene->mNativeResources.textures.resize(data.textureIds.size());
	for(size_t q = 0; q < data.textureIds.size(); ++q)
	{
		scene->mNativeResources.textures[q] = prepare(rc, data.textureIds[q]); //loadResource<IDirect3DTexture9>(rc, data.textureIds[q]);
	}

	scene->mResources.animCharSet = loadResource<mutant::anim_character_set>(data.animCharId);

	// setup scene
	scene->setClip(data.defaultClipIndex);

	return scene;
}

std::auto_ptr<RenderableMesh> prepare(RenderContext& rc, mutalisk::data::mesh const& data)
{
	std::auto_ptr<RenderableMesh> mesh(new RenderableMesh(data));
	
	unsigned int bitsPerIndex = (data.indexSize == 4)? D3DXMESH_32BIT: 0;
	DX_MSG("create mesh") = D3DXCreateMeshFVF(
		(DWORD)indices2Faces(data.indexCount, data.primitiveType),
		data.vertexCount,
		D3DXMESH_MANAGED | bitsPerIndex,
		data.fvfVertexDecl,
		rc.device,
		&mesh->mNative);

	{
		unsigned char* data = 0;
		DX_MSG("lock vertex buffer for writing") = mesh->mNative->LockVertexBuffer(0, (void**)&data);
		memcpy(data, mesh->mBlueprint.vertexData, mesh->mBlueprint.vertexDataSize);
		DX_MSG("unlock vertex buffer") = mesh->mNative->UnlockVertexBuffer();
	}

	{
		unsigned char* data = 0;
		DX_MSG("lock index buffer for writing") = mesh->mNative->LockIndexBuffer(0, (void**)&data);
		memcpy(data, mesh->mBlueprint.indexData, mesh->mBlueprint.indexCount * mesh->mBlueprint.indexSize);
		DX_MSG("unlock index buffer") = mesh->mNative->UnlockIndexBuffer();
	}

	if(!data.subsets.empty())
	{
		// fill attribute table according to subsets
		DWORD* faceAttribute = 0;
		DX_MSG("lock attribute buffer for writing") = mesh->mNative->LockAttributeBuffer(0, &faceAttribute);	
		for(size_t q = 0; q < data.subsets.size(); ++q)
		{
			size_t faceStart = indices2Faces(data.subsets[q].offset, data.primitiveType);
			size_t faceCount = indices2Faces(data.subsets[q].count, data.primitiveType);

			for(size_t w = faceStart; w < faceStart + faceCount; ++w)
				faceAttribute[w] = (DWORD)q;
		}
		DX_MSG("unlock attribute buffer") = mesh->mNative->UnlockAttributeBuffer();
	}
	else
	{
		DWORD* adjacency = new DWORD[mesh->mNative->GetNumFaces() * 3 * sizeof(DWORD)];
		DX_MSG("generate adjacency") = 
			mesh->mNative->GenerateAdjacency(std::numeric_limits<float>::epsilon(), adjacency);
		DX_MSG("inplace optimize for vertex cache") = 
			mesh->mNative->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE, adjacency, 0, 0, 0);
		delete[] adjacency;
	}

	{
		unsigned char* data = 0;
		DX_MSG("lock vertex buffer for reading") = mesh->mNative->LockVertexBuffer(0, (void**)&data);
		memcpy(mesh->mBlueprint.vertexData, data, mesh->mBlueprint.vertexDataSize);
		DX_MSG("unlock vertex buffer") = mesh->mNative->UnlockVertexBuffer();
	}

	{
		unsigned char* data = 0;
		DX_MSG("lock index buffer for reading") = mesh->mNative->LockIndexBuffer(0, (void**)&data);
		memcpy(mesh->mBlueprint.indexData, data, mesh->mBlueprint.indexCount * mesh->mBlueprint.indexSize);
		DX_MSG("unlock index buffer") = mesh->mNative->UnlockIndexBuffer();
	}


	return mesh;
}

namespace {
	void toNative(CTransform::t_matrix const& src, D3DXMATRIX& worldMatrix, bool swap = true)
	{
		static int method = 1;
		if(method == 0)
		{
			worldMatrix = D3DXMATRIX(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				src.Move.x, src.Move.y, src.Move.z, 1.0f
			);
		}
		else if(method == 1)
		{	
			worldMatrix = D3DXMATRIX(
				src.Rot.Row[0].x, src.Rot.Row[1].x, src.Rot.Row[2].x, 0.0f,
				src.Rot.Row[0].y, src.Rot.Row[1].y, src.Rot.Row[2].y, 0.0f,
				src.Rot.Row[0].z, src.Rot.Row[1].z, src.Rot.Row[2].z, 0.0f,
				src.Move.x, src.Move.y, src.Move.z, 1.0f
			);
		}
		else if(method == 2)
		{
			worldMatrix = D3DXMATRIX(
				src.Rot.Row[0].x, src.Rot.Row[0].y, src.Rot.Row[0].z, 0.0f,
				src.Rot.Row[1].x, src.Rot.Row[1].y, src.Rot.Row[1].z, 0.0f,
				src.Rot.Row[2].x, src.Rot.Row[2].y, src.Rot.Row[2].z, 0.0f,
				src.Move.x, src.Move.y, src.Move.z, 1.0f
			);
		}

		if(swap)
		{
			std::swap(worldMatrix._41, worldMatrix._43);
			std::swap(worldMatrix._11, worldMatrix._13);
			std::swap(worldMatrix._21, worldMatrix._23);
			std::swap(worldMatrix._31, worldMatrix._33);
		}
	}

	void toNative(float const* worldMatrixData, D3DXMATRIX& worldMatrix)	
	{
		worldMatrix = D3DXMATRIX(worldMatrixData);
	}

	void toNative(float const* worldMatrixData, CTransform::t_matrix& worldMatrix)
	{
		worldMatrix = CTransform::t_matrix(
			worldMatrixData[8], worldMatrixData[4], worldMatrixData[0],
			worldMatrixData[9], worldMatrixData[5], worldMatrixData[1],
			worldMatrixData[10], worldMatrixData[6], worldMatrixData[2],
			worldMatrixData[14], worldMatrixData[13], worldMatrixData[12]);
	}


	void toNative(mutalisk::data::Vec4 const& src, D3DXVECTOR4& dst)
	{
		dst.x = src[0]; dst.y = src[1]; dst.z = src[2]; dst.w = src[3];
	}
	void toNative(mutalisk::data::Color const& color, D3DXCOLOR& c)
	{
		c.r = color.r; c.g = color.g; c.b = color.b; c.a = color.a;
	}

	void toNative(Dx9RenderableScene const& scene, mutalisk::data::shader_fixed const& src, BaseEffect::Input::Surface& dst)
	{
		toNative(src.ambient, dst.ambient);
		toNative(src.diffuse, dst.diffuse);
		toNative(src.specular, dst.specular);
		toNative(src.emissive, dst.emissive);

		dst.diffuseTexture = (src.diffuseTexture != ~0U)? scene.mNativeResources.textures[src.diffuseTexture] : 0;
		dst.envmapTexture = (src.envmapTexture != ~0U)? scene.mNativeResources.textures[src.envmapTexture] : 0;

		dst.uOffset = src.uOffset;
		dst.vOffset = src.vOffset;
		dst.transparency = src.transparency;
		dst.dummy = 0;

		toNative(src.aux0, dst.aux0);
	}

	void setCameraMatrix(RenderContext& rc, D3DXMATRIX const& cameraMatrix)
	{
		D3DXMATRIX viewMatrix = cameraMatrix;

		D3DXMATRIX sm;
		D3DXMatrixScaling(&sm, 1.0f, 1.0f, -1.0f);
		D3DXMatrixMultiply(&viewMatrix, &sm, &viewMatrix);

		static int overrideCameraMethod = 1;
		D3DXMatrixInverse(&viewMatrix, 0, &viewMatrix);
		if(overrideCameraMethod == 0)
			D3DXMatrixMultiply(&rc.viewMatrix, &viewMatrix, &rc.viewMatrix);
		else if(overrideCameraMethod == 1)
			rc.viewMatrix = viewMatrix;
		D3DXMatrixMultiply(&rc.viewProjMatrix, &rc.viewMatrix, &rc.projMatrix);
	}

	/*void setWorldMatrix(RenderContext& rc, ID3DXEffect& effect, D3DXMATRIX const& worldMatrix)
	{
		D3DXMATRIX invWorld;
		D3DXMatrixInverse(&invWorld, 0, &worldMatrix);

		D3DXMATRIX worldViewProj;
		D3DXMatrixMultiply(&worldViewProj, &worldMatrix, &rc.viewProjMatrix);

		DX_MSG("set matrix") = 
			effect.SetMatrix("mWorld", &worldMatrix);
		DX_MSG("set matrix") = 
			effect.SetMatrix("mInvWorld", &invWorld);
		DX_MSG("set matrix") = 
			effect.SetMatrix("mView", &rc.viewMatrix);
		DX_MSG("set matrix") = 
			effect.SetMatrix("mViewProjection", &rc.viewProjMatrix);
		DX_MSG("set matrix") = 
			effect.SetMatrix("mWorldViewProjection", &worldViewProj);
	}

	void setDiffuseTexture(RenderContext& rc, ID3DXEffect& effect, IDirect3DTexture9* texture,
		D3DTEXTUREADDRESS addressU = D3DTADDRESS_WRAP, D3DTEXTUREADDRESS addressV = D3DTADDRESS_WRAP)
	{
		DX_MSG("set flag") = 
			effect.SetBool("bDiffuseTextureEnabled", (texture != 0));
		DX_MSG("set int") =
			effect.SetInt("nAddressU", addressU);
		DX_MSG("set int") =
			effect.SetInt("nAddressV", addressV);
		DX_MSG("set texture") = 
			effect.SetTexture("tDiffuse", texture);
	}*/

	void render(RenderContext& rc, RenderableMesh const& mesh, unsigned subset = 0)
	{
		// @TBD: support for 'undefined' (~0) subset, whole mesh should be rendered
		rc.device->SetFVF(mesh.mBlueprint.fvfVertexDecl);
		DX_MSG("draw all subsets") =
			mesh.mNative->DrawSubset(subset);
	}
} // namespace


void update(Dx9RenderableScene& scene, float deltaTime)
{
	scene.update(deltaTime);
}

void process(Dx9RenderableScene& scene)
{
	scene.process();
}


struct InstanceInput
{
	enum { MaxLights = 8 };
	enum { RequiredMatrices = BaseEffect::MaxCount_nMatrix };

//	LightT const*	lights[MaxLights];
//	MatrixT const*	lightMatrices[MaxLights];
//	unsigned		lightCount;
	MatrixT			geometryMatrices[RequiredMatrices];
};

struct RenderBlock
{
	unsigned				instanceIndex;
	unsigned				surfaceIndex;
	BaseEffect*				fx;
	RenderableMesh const*	mesh;
	unsigned				subset;

	float					cameraDistanceSq;
};

struct findVisibleActors
{
	RenderContext& rc;
	findVisibleActors(RenderContext& rc_, int) : rc(rc_) {}

	template <typename Container, typename Out>
	void operator()(Container& c, Out& o) { operator()(c.begin(), c.end(), o); }

	template <typename In, typename Out>
	void operator()(In first, In last, Out& visibleActors)
	{
		visibleActors.reserve(std::distance(first, last));
		for(; first != last; ++first)
			visibleActors.push_back(first);
//		std::copy(first, last, visibleActors.begin());
	}
};

struct blastInstanceInputs
{
	Dx9RenderableScene const& scene;
	RenderContext const& rc;

	blastInstanceInputs(Dx9RenderableScene const& scene_, RenderContext const& rc_) : rc(rc_), scene(scene_) {}

	template <typename Container, typename Out>
	void operator()(Container& c, Out& o) { operator()(c.begin(), c.end(), o); }

	template <typename In, typename Out>
	void operator()(In first, In last, Out& instanceInputs)
	{
		gatherSceneLights();

		D3DXMATRIX nativeMatrix;
		instanceInputs.resize(std::distance(first, last));
		for(unsigned block = 0; first != last; ++first, ++block)
		{
			ASSERT(*first);
			mutalisk::data::scene::Actor const& actor = **first;

			InstanceInput& instanceInput = instanceInputs[block];
			gatherInstanceLights();

			ASSERT(actor.id >= 0 && actor.id < this->scene.mState.actor2XformIndex.size());
			toNative(this->scene.mState.matrices[this->scene.mState.actor2XformIndex[actor.id]], nativeMatrix);

			applyWorldMatrix(nativeMatrix, instanceInput.geometryMatrices);
		}
	}

	void gatherSceneLights()
	{
		// @TBD:
	}

	void gatherInstanceLights()
	{
		// @TBD:
	}

	void applyWorldMatrix(D3DXMATRIX const& world, MatrixT* dst)
	{
		D3DXMATRIX invWorld;
		D3DXMATRIX worldViewProj;

		D3DXMatrixInverse(&invWorld, 0, &world);
		D3DXMatrixMultiply(&worldViewProj, &world, &rc.viewProjMatrix);

		dst[BaseEffect::WorldMatrix] = world;
		dst[BaseEffect::ViewMatrix] = rc.viewMatrix;
		dst[BaseEffect::ProjMatrix] = rc.projMatrix;
		dst[BaseEffect::ViewProjMatrix] = rc.viewProjMatrix;
		dst[BaseEffect::WorldViewProjMatrix] = worldViewProj;
		dst[BaseEffect::InvWorldMatrix] = invWorld;
	}
};

struct blastSurfaceInputs
{
	Dx9RenderableScene const& scene;
	blastSurfaceInputs(Dx9RenderableScene const& scene_, int) : scene(scene_) {}

	template <typename Container, typename Out>
	void operator()(Container& c, Out& o) { operator()(c.begin(), c.end(), o); }

	template <typename In, typename Out>
	void operator()(In first, In last, Out& surfaceInputs)
	{
		surfaceInputs.reserve(std::distance(first, last));
		
		for(unsigned block = 0; first != last; ++first)
		{
			ASSERT(*first);
			mutalisk::data::scene::Actor const& actor = **first;

			ASSERT(!actor.materials.empty());
			surfaceInputs.resize(surfaceInputs.size() + actor.materials.size());
			for(unsigned q = 0; q < actor.materials.size(); ++q, ++block)
			{
				BaseEffect::Input::Surface& surfaceInput = surfaceInputs[block];
				toNative(scene, actor.materials[q].shaderInput, surfaceInput);
			}
		}
	}
};

struct blastRenderBlocks
{
	Dx9RenderableScene const& scene;
	RenderContext& rc;

	blastRenderBlocks(Dx9RenderableScene const& scene_, RenderContext& rc_) : rc(rc_), scene(scene_) {}

	template <typename Container, typename Out>
	void operator()(Container& c, Out& o) { operator()(c.begin(), c.end(), o); }

	template <typename In, typename Out>
	void operator()(In first, In last, Out& renderBlocks)
	{
		renderBlocks.reserve(std::distance(first, last));
		
		unsigned actorIt = 0;
		unsigned blockIt = 0;
		for(; first != last; ++first, ++actorIt)
		{
			ASSERT(*first);
			mutalisk::data::scene::Actor const& actor = **first;

			RenderableMesh const* mesh = scene.mResources.meshes[actor.meshIndex].renderable.get();
			float cameraDistanceSq = 0.0f; // @TBD: calc distance to camera

			ASSERT(!actor.materials.empty());
			renderBlocks.resize(renderBlocks.size() + actor.materials.size());
			for(unsigned q = 0; q < actor.materials.size(); ++q, ++blockIt)
			{
				RenderBlock& renderBlock = renderBlocks[blockIt];
				renderBlock.instanceIndex = actorIt;
				renderBlock.surfaceIndex = blockIt;
				renderBlock.fx = mutalisk::effects::getByIndex(actor.materials[q].shaderIndex);
				renderBlock.mesh = mesh;
				renderBlock.subset = q;
				renderBlock.cameraDistanceSq = cameraDistanceSq;
			}
		}
	}
};

struct sortRenderBlocks
{
	template <typename Container, typename Out>
	void operator()(Container& c, Out& o) { operator()(c.begin(), c.end(), o); }

	template <typename In, typename Out>
	void operator()(In first, In last, Out& sortedRenderBlocks)
	{
		// @TBD:
	}
};

struct drawRenderBlocks
{
	RenderContext& rc;
	Dx9RenderableScene const& scene;
	InstanceInput const* instanceInputs; size_t instanceInputCount;
	BaseEffect::Input::Surface const* surfaceInputs; size_t surfaceInputCount;

	drawRenderBlocks(RenderContext& rc_, Dx9RenderableScene const& scene_, 
		InstanceInput const* instanceInputs_, size_t instanceInputCount_,
		BaseEffect::Input::Surface const* surfaceInputs_, size_t surfaceInputCount_)
		: rc(rc_), scene(scene_)
		, instanceInputs(instanceInputs_), instanceInputCount(instanceInputCount_)
		, surfaceInputs(surfaceInputs_), surfaceInputCount(surfaceInputCount_) {}

	template <typename Container>
	void operator()(Container& c) { operator()(c.begin(), c.end()); }

	template <typename In>
	void operator()(In first, In last)
	{
		BaseEffect* currFx = 0;
		BaseEffect::Input fxInput;
		BaseEffect::clearInput(fxInput);

		static std::vector<LightT> sceneLights; sceneLights.resize(0);
		static std::vector<MatrixT> sceneLightMatrices; sceneLightMatrices.resize(0);
		fxInput.lights = gatherSceneLights(sceneLights, sceneLightMatrices);

		for(; first != last; ++first)
		{
			RenderBlock const& block = *first;

			if(currFx != block.fx)
			{
				if(currFx)
					currFx->end();
				currFx = block.fx;

				currFx->captureState();
				currFx->begin();
			}

			fxInput.surface = &surfaceInputs[block.surfaceIndex];
			fxInput.matrices = instanceInputs[block.instanceIndex].geometryMatrices;
	
			unsigned passCount = currFx->passCount(fxInput);
			for(unsigned pass = 0; pass < passCount; ++pass)
			{
				//currFx->passInfo();
				currFx->pass(fxInput, pass);

				ASSERT(block.mesh);
				render(rc, *block.mesh, block.subset);
			}
		}

		if(currFx)
			currFx->end();
	}

	BaseEffect::Input::Lights gatherSceneLights(std::vector<LightT>& lights, std::vector<MatrixT>& lightMatrices)
	{
		BaseEffect::Input::Lights fxLights;
		fxLights.count = 0;

		if(scene.mBlueprint.lights.empty())
			return fxLights;

		lights.resize(scene.mBlueprint.lights.size());
		lightMatrices.resize(scene.mBlueprint.lights.size());

		MatrixT nativeMatrix;
		for(size_t q = 0; q < scene.mBlueprint.lights.size(); ++q)
		{
			mutalisk::data::scene::Light const& light = scene.mBlueprint.lights[q];

			ASSERT(q >= 0 && q < scene.mState.light2XformIndex.size());
			toNative(scene.mState.matrices[scene.mState.light2XformIndex[q]], nativeMatrix);

			lights[q] = light;
			lightMatrices[q] = nativeMatrix;
		}

		fxLights.data = &lights[0];
		fxLights.matrices = &lightMatrices[0];
		fxLights.count = scene.mBlueprint.lights.size();
		return fxLights;
	}
};

void render(RenderContext& rc, Dx9RenderableScene const& scene, bool animatedActors, bool animatedCamera, int maxActors)
{
	D3DXMATRIX nativeMatrix;
	ASSERT(rc.defaultEffect);

	{
		if(scene.mBlueprint.defaultClipIndex == ~0U)
		{
			animatedActors = false;
			animatedCamera = false;
		}

		if(!animatedActors)
			for(size_t q = 0; q < scene.mBlueprint.actors.size(); ++q)
			{
				ASSERT(q >= 0 && q < scene.mState.actor2XformIndex.size());
				toNative(scene.mBlueprint.actors[q].worldMatrix.data,
					const_cast<Dx9RenderableScene&>(scene).mState.matrices[scene.mState.actor2XformIndex[q]]);
			}

		if(!animatedCamera)
			for(size_t q = 0; q < scene.mBlueprint.cameras.size(); ++q)
			{
				ASSERT(q >= 0 && q < scene.mState.camera2XformIndex.size());
				toNative(scene.mBlueprint.cameras[q].worldMatrix.data,
					const_cast<Dx9RenderableScene&>(scene).mState.matrices[scene.mState.camera2XformIndex[q]]);
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

		gContext.device = rc.device;
		gContext.uberShader = rc.defaultEffect;
	}


	static std::vector<InstanceInput> instanceInputs;
	static std::vector<BaseEffect::Input::Surface> surfaceInputs;
	static std::vector<RenderBlock> renderBlocks;

	instanceInputs.resize(0);
	surfaceInputs.resize(0);
	renderBlocks.resize(0);

	static std::vector<mutalisk::data::scene::Actor const*> visibleActors;
	visibleActors.resize(0);

	RenderContext& camera = rc; // @TBD:
	findVisibleActors(camera, 0) (scene.mBlueprint.actors, visibleActors);
	blastInstanceInputs(scene, camera) (visibleActors, instanceInputs);
	blastSurfaceInputs(scene, 0) (visibleActors, surfaceInputs);
	blastRenderBlocks(scene, camera) (visibleActors, renderBlocks);
	std::vector<RenderBlock>& sortedRenderBlocks = renderBlocks; //sortRenderBlocks()(renderBlocks, sortedRenderBlocks);
	if(instanceInputs.empty() || surfaceInputs.empty())
	{
		ASSERT(visibleActors.empty());
	}
	else
		drawRenderBlocks(rc, scene, 
			&instanceInputs[0], instanceInputs.size(), &surfaceInputs[0], surfaceInputs.size()) (sortedRenderBlocks);




/*

//	static Lambert fx;
//	static Shiny fx;
//	BaseEffect::Input& fxInput = fx.allocInput();

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

//	for(unsigned pass = 0; pass < passCount; ++pass)
//	{
		unsigned shaderIndex = ~0;
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
				fxInput.vecs[BaseEffect::AmbientColor] = D3DXVECTOR4(1,1,1,1);
				fxInput.vecs[BaseEffect::DiffuseColor] = D3DXVECTOR4(1,1,1,1);
				fxInput.vecs[BaseEffect::SpecularColor] = D3DXVECTOR4(1,1,1,1);

				fx.pass(pass);
				render(rc, mesh);
			}
			else
			for(unsigned int materialIt = 0; materialIt < actor.materials.size(); ++materialIt)
			{
				unsigned int colorTextureIndex = actor.materials[materialIt].colorTextureIndex;
				fxInput.textures[BaseEffect::DiffuseTexture] = 
					((colorTextureIndex != ~0U)? scene.mNativeResources.textures[colorTextureIndex]: 0);

				unsigned int envmapTextureIndex = actor.materials[materialIt].envmapTextureIndex;
				fxInput.textures[BaseEffect::EnvmapTexture] = 
					((envmapTextureIndex != ~0U)? scene.mNativeResources.textures[envmapTextureIndex]: 0);

				fxInput.vecs[BaseEffect::AmbientColor] = D3DXVECTOR4(1,1,1,1);
//				toNative(actor.materials[materialIt].ambient, fxInput.vecs[BaseEffect::AmbientColor]);
				toNative(actor.materials[materialIt].diffuse, fxInput.vecs[BaseEffect::DiffuseColor]);
				toNative(actor.materials[materialIt].specular, fxInput.vecs[BaseEffect::SpecularColor]);

				fx.pass(pass);
				render(rc, mesh, materialIt);
			}
		}
	}
	fx.end();
	*/
}
////////////////////////////////////////////////

void CSkinnedAlgos::processSkinMesh(RenderableMesh& mesh, BoneMapT const& boneMap, CTransform::t_matrix const* matrices)
{		
	ASSERT(mesh.mBlueprint.skinInfo);
	ASSERT(!boneMap.empty());

	D3DVERTEXELEMENT9 dx9Declaration[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(mesh.mBlueprint.fvfVertexDecl, dx9Declaration);

	size_t positionsOffset = ~0U;
	size_t normalsOffset = ~0U;
	size_t weightsOffset = ~0U;
	size_t boneIndicesOffset = ~0U;

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

	ASSERT(positionsOffset != ~0);
	ASSERT(normalsOffset != ~0);
	ASSERT(weightsOffset != ~0);
	ASSERT(boneIndicesOffset != ~0);

	unsigned char* dstRaw = 0;
	DX_MSG("lock vertex buffer for writing") = mesh.mNative->LockVertexBuffer(0, (void**)&dstRaw);
	unsigned char const* srcRaw = mesh.mBlueprint.vertexData;
	memcpy(dstRaw, srcRaw, mesh.mBlueprint.vertexDataSize);

	processSkinMesh(
		reinterpret_cast<Vec3 const*>(srcRaw + positionsOffset),
		reinterpret_cast<Vec3 const*>(srcRaw + normalsOffset),
		reinterpret_cast<float const*>(srcRaw + weightsOffset),
		reinterpret_cast<unsigned char const*>(srcRaw + boneIndicesOffset),

		reinterpret_cast<Vec3*>(dstRaw + positionsOffset),
		reinterpret_cast<Vec3*>(dstRaw + normalsOffset),

		mesh.mBlueprint.vertexStride,											// srcVertexStride
		mesh.mBlueprint.vertexStride,											// srcWeightStride
		mesh.mBlueprint.vertexStride,											// srcBoneIndexStride
		mesh.mBlueprint.vertexStride,											// dstVertexStride
		mesh.mBlueprint.vertexCount,

		*mesh.mBlueprint.skinInfo, boneMap, matrices);

	DX_MSG("unlock vertex buffer") = mesh.mNative->UnlockVertexBuffer();

}