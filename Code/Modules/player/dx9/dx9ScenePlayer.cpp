#include "dx9ScenePlayer.h"
#include "../ScenePlayer.h"

#include <memory>
#include <d3d9types.h>
#include <effects/BaseEffect.h>
#include <effects/all.h>

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

	void setCameraMatrix(RenderContext& rc, D3DXMATRIX const& cameraMatrix)
	{
		D3DXMATRIX viewMatrix = cameraMatrix;

		D3DXMATRIX sm;
		D3DXMatrixScaling(&sm, 1.0f, 1.0f, -1.0f);
		D3DXMatrixMultiply(&viewMatrix, &sm, &viewMatrix);

		static bool overrideCamera = false;
		D3DXMatrixInverse(&viewMatrix, 0, &viewMatrix);
		if(!overrideCamera)
			D3DXMatrixMultiply(&rc.viewMatrix, &viewMatrix, &rc.viewMatrix);

		D3DXMatrixMultiply(&rc.viewProjMatrix, &viewMatrix, &rc.projMatrix);
	}

	void setWorldMatrix(RenderContext& rc, ID3DXEffect& effect, D3DXMATRIX const& worldMatrix)
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
	}

	void render(RenderContext& rc, RenderableMesh const& mesh, unsigned int subset = 0)
	{
		// $TBD: support for 'undefined' (~0) subset, whole mesh should be rendered
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
/*
void render(RenderContext& rc, Dx9RenderableScene const& scene, bool animatedActors, bool animatedCamera, int maxActors)
{
	D3DXMATRIX nativeMatrix;
	ASSERT(rc.defaultEffect);

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

	unsigned int passCount = 0;
	DX_MSG("begin effect") = rc.defaultEffect->Begin(&passCount, 0);
    for(unsigned int iPass = 0; iPass < passCount; iPass++)
    {
		DX_MSG("begin pass") = rc.defaultEffect->BeginPass(iPass);	
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
				setWorldMatrix(rc, *rc.defaultEffect, nativeMatrix);

			RenderableMesh const& mesh = *scene.mResources.meshes[actor.meshIndex].renderable;
			if(actor.materials.empty())
			{
				DX_MSG("commit changes") = rc.defaultEffect->CommitChanges();
				render(rc, mesh);
			} else
			for(unsigned int materialIt = 0; materialIt < actor.materials.size(); ++materialIt)
			{
				unsigned int textureIndex = actor.materials[materialIt].textureIndex;
				setDiffuseTexture(rc, *rc.defaultEffect, 
					(textureIndex != ~0U)? scene.mNativeResources.textures[textureIndex]: 0);

				DX_MSG("commit changes") = rc.defaultEffect->CommitChanges();
				render(rc, mesh, materialIt);
			}
		}
		DX_MSG("end pass") = rc.defaultEffect->EndPass();
	}
	DX_MSG("end effect") = rc.defaultEffect->End();
}*/

namespace {

	struct MatrixState
	{
		void applyWorldMatrix(RenderContext const& rc, D3DXMATRIX const& inWorldMatrix, BaseEffect::Input& dst)
		{
			world = inWorldMatrix;

			D3DXMatrixInverse(&invWorld, 0, &world);
			D3DXMatrixMultiply(&worldViewProj, &world, &rc.viewProjMatrix);

			dst.matrices[BaseEffect::WorldMatrix] = &world;
			dst.matrices[BaseEffect::ViewMatrix] = &rc.viewMatrix;
			dst.matrices[BaseEffect::ViewProjMatrix] = &rc.viewProjMatrix;
			dst.matrices[BaseEffect::WorldViewProjMatrix] = &worldViewProj;
			dst.matrices[BaseEffect::InvWorldMatrix] = &invWorld;
		}

		D3DXMATRIX world;
		D3DXMATRIX worldViewProj;
		D3DXMATRIX invWorld;
	};
}


void render(RenderContext& rc, Dx9RenderableScene const& scene, bool animatedActors, bool animatedCamera, int maxActors)
{
	D3DXMATRIX nativeMatrix;
	ASSERT(rc.defaultEffect);

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

//	BaseEffect& effect;
	gContext.device = rc.device;
	gContext.uberShader = rc.defaultEffect;

	gContext.device = rc.device;

	static Lambert fx;
	BaseEffect::Input& fxInput = fx.allocInput();

	fx.captureState();
	unsigned passCount = fx.begin();
	for(unsigned pass = 0; pass < passCount; ++pass)
	{
//		fx.beginPass(pass);

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

			fxInput.vecs[BaseEffect::AmbientColor] = D3DXVECTOR4(0,0,0,1);
			fxInput.vecs[BaseEffect::DiffuseColor] = D3DXVECTOR4(1,1,1,1);
			fxInput.vecs[BaseEffect::SpecularColor] = D3DXVECTOR4(1,1,1,1);

			RenderableMesh const& mesh = *scene.mResources.meshes[actor.meshIndex].renderable;
			if(actor.materials.empty())
			{
				fx.pass(pass);
				render(rc, mesh);
			}
			else
			for(unsigned int materialIt = 0; materialIt < actor.materials.size(); ++materialIt)
			{
				unsigned int textureIndex = actor.materials[materialIt].textureIndex;
				fxInput.textures[BaseEffect::DiffuseTexture] = 
					((textureIndex != ~0U)? scene.mNativeResources.textures[textureIndex]: 0);

				fx.pass(pass);
//				fx.commitChanges();
				render(rc, mesh, materialIt);
			}
		}
	}
	fx.end();
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