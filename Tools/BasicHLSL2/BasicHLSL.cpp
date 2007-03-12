//--------------------------------------------------------------------------------------
// File: BasicHLSL.cpp
//
// This sample shows a simple example of the Microsoft Direct3D's High-Level 
// Shader Language (HLSL) using the Effect interface. 
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "dxstdafx.h"
#include "resource.h"

#include "cfg.h"
#define bool_ bool

//#define DEBUG_VS   // Uncomment this line to debug vertex shaders 
//#define DEBUG_PS   // Uncomment this line to debug pixel shaders 



#include <d3d9.h>


#include <memory>
#include <mutant/mutant.h>
#include <mutant/reader.h>
#include <mutant/io_factory.h>

#include <mutalisk/dx9/dx9Platform.h>
#include <mutalisk/mutalisk.h>
#include <player/ScenePlayer.h>
#include <player/dx9/dx9ScenePlayer.h>

//#include "Transform.h"
//#include "animator/Animators.h"
//#include "animator/AnimatorAlgos.h"

//#include "ScenePlayer.h"


struct ScenePlayerApp
{
	ScenePlayerApp(std::string const& sceneName, IDirect3DDevice9& device, ID3DXEffect& defaultEffect)
	{
		renderContext.device = &device;	
		renderContext.defaultEffect = &defaultEffect;
//		renderContext.viewProjMatrix = CTransform::identity().matrix();
		D3DXMatrixIdentity(&renderContext.viewProjMatrix);
		D3DXMatrixIdentity(&renderContext.projMatrix);

		scene.blueprint = loadResource<mutalisk::data::scene>(sceneName);
		scene.renderable = prepare(renderContext, *scene.blueprint);
	}

	void setViewMatrix(D3DXMATRIX const& viewMatrix)
	{
		renderContext.viewMatrix = viewMatrix;
	}

	void setProjMatrix(D3DXMATRIX const& projMatrix)
	{
		renderContext.projMatrix = projMatrix;
	}


	void update(float deltaTime) { ::update(*scene.renderable, deltaTime); }
	void process() { ::process(*scene.renderable); }
	void render(int maxActors = -1) { ::render(renderContext, *scene.renderable, maxActors); }

	struct Scene
	{
		std::auto_ptr<mutalisk::data::scene> blueprint;
		std::auto_ptr<Dx9RenderableScene> renderable;
	};
	
	RenderContext	renderContext;
	Scene			scene;
};
std::auto_ptr<ScenePlayerApp> scenePlayerApp;


static bool gLoadSkin = true;
static bool gRenderSkin = true;
static bool gEnableAnimations = true;
static bool gRenderDebugSkeleton = true;


std::string gSceneFileName = "test_baked.msk";
//std::string gSceneFileName = "test_texture.msk";

//*
std::string gAnimFileName = "guffy.man";
std::string gCharName = "character";
std::string gClipName = "LW Take 001";
std::string gSkinFileName = "Model__Guffy.msh";

/*/
std::string gAnimFileName = "mutant1.man";
std::string gCharName = "Mutant";
std::string gClipName = "attack1";
std::string gSkinFileName = "mutant1.msh";

//*/


std::auto_ptr<mutant::anim_character_set> loadAnimation(std::string const& fileName)
{
//	std::string fileName = "mutant1.man";
//	std::string fileName = "guffy.man";

	std::auto_ptr<mutant::binary_input> input = mutant::reader_factory::createInput( fileName );
	mutant::mutant_reader mutReader( input );
	mutReader.enableLog( false );

	std::auto_ptr<mutant::anim_character_set> mutCharSet( new mutant::anim_character_set );
	mutReader.read( *mutCharSet );

	return mutCharSet;
}

/*
std::auto_ptr<mutant::simple_skinned> loadSkin(std::string fileName)
{
//	std::string fileName = "mutant1.msh";
//	std::string fileName = "Model__Guffy.msh";
//	std::string fileName = "Model__Gun_(1).msh";


	std::auto_ptr<mutant::binary_input> input = mutant::reader_factory::createInput( fileName );
	mutant::mutant_reader mutReader( input );
	mutReader.enableLog( false );

	std::auto_ptr<mutant::simple_skinned> mutSkinned( new mutant::simple_skinned );
	mutReader.read( *mutSkinned );

	return mutSkinned;
}


void skinToBuffers( mutant::simple_skinned& skin, void** vbuffer, void** ibuffer )
{
	*vbuffer = malloc( skin.vertexCount * sizeof(mutant::simple_skinned::Vec3) * 2 );
	u8* vdst = reinterpret_cast<u8*>(*vbuffer);
	for(size_t q = 0; q < skin.vertexCount; ++q)
	{
		memcpy( vdst, &skin.positions[q], sizeof(mutant::simple_skinned::Vec3) );
		vdst += sizeof(mutant::simple_skinned::Vec3);
		memcpy( vdst, &skin.normals[q], sizeof(mutant::simple_skinned::Vec3) );
		vdst += sizeof(mutant::simple_skinned::Vec3);
	}
	memcpy( *vbuffer, skin.positions, skin.vertexCount * sizeof(mutant::simple_skinned::Vec3) );
//	*vbuffer = (void*)skin.positions; // copy
	*ibuffer = (void*)skin.indices;
}

typedef std::vector<std::pair<int, int> > BoneMapT;
BoneMapT& mapSkinnedBonesToHierarchy(
	mutant::simple_skinned::Bone* bones, size_t boneCount,
	mutant::anim_hierarchy const& hierarchy,
	BoneMapT& remapedBones )
{
	remapedBones.reserve( remapedBones.size() + boneCount );

	for( size_t q = 0; q < boneCount; ++q )
	{
		unsigned boneId = 0;
		std::string const& boneName = bones[q].name;

		while( boneId < hierarchy.size() && hierarchy[ boneId ].name != boneName )
			++boneId;

		if( boneId == hierarchy.size() )
			printf( "Couldn't find '%s' in hierarchy", boneName.c_str() );
		else
			remapedBones.push_back( std::make_pair( q, boneId ) );
	}

	return remapedBones;
}
*/
/*	
void updateSkinMesh( mutant::simple_skinned& skin, BoneMapT& boneMap, CTransform::t_matrix const* data, float* vbuffer, size_t stride )
{
//	CMeshSegment::TBoneMap const& boneIds = mSlotBuffer[segment]->bonesInCombination();
	static std::vector<D3DXMATRIX> worldMatrices;
	worldMatrices.resize( boneMap.size() );

	unsigned i = 0;
	BoneMapT::iterator bIdIt = boneMap.begin();
	for( ; (i < boneMap.size()) && (bIdIt != boneMap.end()); ++bIdIt, ++i )
	{
		float* mat16 = skin.bones[ bIdIt->first ].matrix.data;
		D3DXMATRIX m(
			mat16[0], mat16[1], mat16[2], mat16[3],
			mat16[4], mat16[5], mat16[6], mat16[7],
			mat16[8], mat16[9], mat16[10], mat16[11],
			mat16[12], mat16[13], mat16[14], mat16[15]
		);

		CTransform::t_matrix const& src = data[ bIdIt->second ];
		D3DXMATRIX animM(
			src.Rot.Row[0].x, src.Rot.Row[1].x, src.Rot.Row[2].x, 0.0f,
			src.Rot.Row[0].y, src.Rot.Row[1].y, src.Rot.Row[2].y, 0.0f,
			src.Rot.Row[0].z, src.Rot.Row[1].z, src.Rot.Row[2].z, 0.0f,
			src.Move.x, src.Move.y, src.Move.z, 1.0f
		); 

		worldMatrices[i] = m * animM;
//		worldMatrices[i] = animM * m;
	}

//	i = 0;

	while( i < worldMatrices.size() )
		D3DXMatrixIdentity( &worldMatrices[i++] );
//		Mat34_setIdentity( &worldMatrices[i++] );

	for( size_t q = 0; q < skin.vertexCount; ++q )
	{
//		CTransform::t_vector pos( skin.positions[q].data[0], skin.positions[q].data[1], skin.positions[q].data[2] );
//		CTransform::t_vector accum( 0.0f, 0.0f, 0.0f );
		D3DXVECTOR3 pos( skin.positions[q].data[0], skin.positions[q].data[1], skin.positions[q].data[2] );
		D3DXVECTOR3 accum( 0.0f, 0.0f, 0.0f );
		float accumWeight = 0.0f;
		const size_t weightsPerVertex = skin.weightsPerVertex;
		for( size_t w = 0; w < weightsPerVertex; ++w )
		{
			unsigned short boneId = skin.boneIndices[q * skin.weightsPerVertex + w];
			float boneWeight = skin.weights[q * skin.weightsPerVertex + w];
			if( w != weightsPerVertex - 1 )
				accumWeight += boneWeight;
			else
				boneWeight = 1.0f - accumWeight;

			D3DXVECTOR3 v;
			D3DXVec3TransformCoord( &v, &pos, &worldMatrices[boneId] );
			accum += v * boneWeight;

//			Vec3 v;
//			Vec3_setMat34MulVec3( &accum, &worldMatrices[boneId], &pos );
//			Vec3_scale( &v, &v, boneWeight );
//			Vec3_add( &accum, &accum, &v );
			
		}

		vbuffer[0] = accum.x;
		vbuffer[1] = accum.y;
		vbuffer[2] = accum.z;
		vbuffer = (float*)((u8*)vbuffer + stride);
	}
}

void updateSkinMesh2( mutant::simple_skinned& skin, BoneMapT& boneMap, CTransform::t_matrix const* data, float* vbuffer, size_t stride )
{
	static std::vector<CTransform::t_matrix> worldMatrices;
	worldMatrices.resize( boneMap.size() );

	unsigned i = 0;
	BoneMapT::iterator bIdIt = boneMap.begin();
	for( ; (i < boneMap.size()) && (bIdIt != boneMap.end()); ++bIdIt, ++i )
	{
		float* mat16 = skin.bones[ bIdIt->first ].matrix.data;
		CTransform::t_matrix m(
			mat16[0], mat16[1], mat16[2], //mat16[3],
			mat16[4], mat16[5], mat16[6], //mat16[7],
			mat16[8], mat16[9], mat16[10], //mat16[11],
			mat16[12], mat16[13], mat16[14] //mat16[15]
		);
		CTransform::t_matrix const& src = data[ bIdIt->second ];
		CTransform::t_matrix animM(
			src.Rot.Row[0].x, src.Rot.Row[1].x, src.Rot.Row[2].x,
			src.Rot.Row[0].y, src.Rot.Row[1].y, src.Rot.Row[2].y,
			src.Rot.Row[0].z, src.Rot.Row[1].z, src.Rot.Row[2].z,
			src.Move.x, src.Move.y, src.Move.z
			);
//		worldMatrices[i] = m * animM;
		Mat34_mul( &worldMatrices[i], &m, (Mat34*)&src );
//		Mat34_mul( &worldMatrices[i], &m, (Mat34*)&animM );
	}

//	i = 0;

	while( i < worldMatrices.size() )
		Mat34_setIdentity( &worldMatrices[i++] );

	for( size_t q = 0; q < skin.vertexCount; ++q )
	{
		CTransform::t_vector pos( skin.positions[q].data[0], skin.positions[q].data[1], skin.positions[q].data[2] );
		CTransform::t_vector accum( 0.0f, 0.0f, 0.0f );
		float accumWeight = 0.0f;
		const size_t weightsPerVertex = skin.weightsPerVertex;
		for( size_t w = 0; w < weightsPerVertex; ++w )
		{
			unsigned short boneId = skin.boneIndices[q * skin.weightsPerVertex + w];
			float boneWeight = skin.weights[q * skin.weightsPerVertex + w];
			if( w != weightsPerVertex - 1 )
				accumWeight += boneWeight;
			else
				boneWeight = 1.0f - accumWeight;

			CTransform::t_matrix& src = worldMatrices[boneId];
			CTransform::t_matrix animTM(
				src.Rot.Row[0].x, src.Rot.Row[1].x, src.Rot.Row[2].x,
				src.Rot.Row[0].y, src.Rot.Row[1].y, src.Rot.Row[2].y,
				src.Rot.Row[0].z, src.Rot.Row[1].z, src.Rot.Row[2].z,
				src.Move.x, src.Move.y, src.Move.z
				);

			Vec3 v;
			Vec3_setMat34MulVec3( &v, &animTM, &pos );//worldMatrices[boneId], &pos3 );
//			Vec3_setMat34MulVec3( &v3, &worldMatrices[boneId], &pos3 );
			Vec3_scale( &v, &v, boneWeight );
			Vec3_add( &accum, &accum, &v );

		}

		vbuffer[0] = accum.x;
		vbuffer[1] = accum.y;
		vbuffer[2] = accum.z;
		vbuffer = (float*)((u8*)vbuffer + stride);
	}
}

void updateSkinMesh3( mutant::simple_skinned& skin, BoneMapT& boneMap, CTransform::t_matrix const* data, float* vbuffer, size_t stride )
{
	static std::vector<CTransform::t_matrix> worldMatrices;
	worldMatrices.resize( boneMap.size() );

	unsigned i = 0;
	BoneMapT::iterator bIdIt = boneMap.begin();
	for( ; (i < boneMap.size()) && (bIdIt != boneMap.end()); ++bIdIt, ++i )
	{
		float* mat16 = skin.bones[ bIdIt->first ].matrix.data;
		D3DXMATRIX m(
			mat16[0], mat16[1], mat16[2], mat16[3],
			mat16[4], mat16[5], mat16[6], mat16[7],
			mat16[8], mat16[9], mat16[10], mat16[11],
			mat16[12], mat16[13], mat16[14], mat16[15]
		);

		CTransform::t_matrix const& src = data[ bIdIt->second ];
		D3DXMATRIX animM(
			src.Rot.Row[0].x, src.Rot.Row[1].x, src.Rot.Row[2].x, 0.0f,
			src.Rot.Row[0].y, src.Rot.Row[1].y, src.Rot.Row[2].y, 0.0f,
			src.Rot.Row[0].z, src.Rot.Row[1].z, src.Rot.Row[2].z, 0.0f,
			src.Move.x, src.Move.y, src.Move.z, 1.0f
		);
		D3DXMATRIX t = m * animM;
//		worldMatrices[i] = m * animM;

		worldMatrices[i] = CTransform::t_matrix(
			t._11, t._12, t._13, 
			t._21, t._22, t._23, 
			t._31, t._32, t._33, 
			t._41, t._42, t._43
		);
	}

//	i = 0;

	while( i < worldMatrices.size() )
		Mat34_setIdentity( &worldMatrices[i++] );

	for( size_t q = 0; q < skin.vertexCount; ++q )
	{
		CTransform::t_vector pos3( skin.positions[q].data[0], skin.positions[q].data[1], skin.positions[q].data[2] );
		CTransform::t_vector accum3( 0.0f, 0.0f, 0.0f );
		D3DXVECTOR3 pos( skin.positions[q].data[0], skin.positions[q].data[1], skin.positions[q].data[2] );
		D3DXVECTOR3 accum( 0.0f, 0.0f, 0.0f );
		float accumWeight = 0.0f;
		const size_t weightsPerVertex = skin.weightsPerVertex;
		for( size_t w = 0; w < weightsPerVertex; ++w )
		{
			unsigned short boneId = skin.boneIndices[q * skin.weightsPerVertex + w];
			float boneWeight = skin.weights[q * skin.weightsPerVertex + w];
			if( w != weightsPerVertex - 1 )
				accumWeight += boneWeight;
			else
				boneWeight = 1.0f - accumWeight;

			CTransform::t_matrix& src = worldMatrices[boneId];
			D3DXMATRIX animM(
				src.Rot.Row[0].x, src.Rot.Row[0].y, src.Rot.Row[0].z, 0.0f,
				src.Rot.Row[1].x, src.Rot.Row[1].y, src.Rot.Row[1].z, 0.0f,
				src.Rot.Row[2].x, src.Rot.Row[2].y, src.Rot.Row[2].z, 0.0f,
				src.Move.x, src.Move.y, src.Move.z, 1.0f
			); 
			CTransform::t_matrix animTM(
				src.Rot.Row[0].x, src.Rot.Row[1].x, src.Rot.Row[2].x,
				src.Rot.Row[0].y, src.Rot.Row[1].y, src.Rot.Row[2].y,
				src.Rot.Row[0].z, src.Rot.Row[1].z, src.Rot.Row[2].z,
				src.Move.x, src.Move.y, src.Move.z
				);

			Vec3 v3;
			Vec3_setMat34MulVec3( &v3, &animTM, &pos3 );//worldMatrices[boneId], &pos3 );
//			Vec3_setMat34MulVec3( &v3, &worldMatrices[boneId], &pos3 );
			Vec3_scale( &v3, &v3, boneWeight );
			Vec3_add( &accum3, &accum3, &v3 );

			D3DXVECTOR3 v;
			D3DXVec3TransformCoord( &v, &pos, &animM );
			accum += v * boneWeight;			
		}

		vbuffer[0] = accum3.x;
		vbuffer[1] = accum3.y;
		vbuffer[2] = accum3.z;
		vbuffer = (float*)((u8*)vbuffer + stride);
	}
}


void updateSkinMesh4( mutant::simple_skinned& skin, BoneMapT& boneMap, CTransform::t_matrix const* data, float* vbuffer, size_t stride )
{
	static std::vector<CTransform::t_matrix> worldMatrices;
	worldMatrices.resize( boneMap.size() );

	unsigned i = 0;
	BoneMapT::iterator bIdIt = boneMap.begin();
	for( ; (i < boneMap.size()) && (bIdIt != boneMap.end()); ++bIdIt, ++i )
	{
		float* mat16 = skin.bones[ bIdIt->first ].matrix.data;
		CTransform::t_matrix tm(
			mat16[0], mat16[4], mat16[8], //mat16[3],
			mat16[1], mat16[5], mat16[9], //mat16[7],
			mat16[2], mat16[6], mat16[10], //mat16[11],

			mat16[12], mat16[13], mat16[14] //mat16[15]
		);
		D3DXMATRIX m(
			mat16[0], mat16[1], mat16[2], mat16[3],
			mat16[4], mat16[5], mat16[6], mat16[7],
			mat16[8], mat16[9], mat16[10], mat16[11],
			mat16[12], mat16[13], mat16[14], mat16[15]
		);

		CTransform::t_matrix const& src = data[ bIdIt->second ];
		CTransform::t_matrix animTM(
			src.Rot.Row[0].x, src.Rot.Row[0].y, src.Rot.Row[0].z,
			src.Rot.Row[1].x, src.Rot.Row[1].y, src.Rot.Row[1].z,
			src.Rot.Row[2].x, src.Rot.Row[2].y, src.Rot.Row[2].z,
			src.Move.x, src.Move.y, src.Move.z
			);
		D3DXMATRIX animM(
			src.Rot.Row[0].x, src.Rot.Row[1].x, src.Rot.Row[2].x, 0.0f,
			src.Rot.Row[0].y, src.Rot.Row[1].y, src.Rot.Row[2].y, 0.0f,
			src.Rot.Row[0].z, src.Rot.Row[1].z, src.Rot.Row[2].z, 0.0f,
			src.Move.x, src.Move.y, src.Move.z, 1.0f
		);

		D3DXMATRIX t = m * animM;
		worldMatrices[i] = CTransform::t_matrix(
			t._11, t._12, t._13, 
			t._21, t._22, t._23, 
			t._31, t._32, t._33, 
			t._41, t._42, t._43
		);
		Vec3 t0, t1;
		Vec3_setMat34MulVec3(&t0, &tm, &animTM.Move);
		Vec3_setMat34MulVec3(&t1, &animTM, &tm.Move);


		float a41 =
			m._41 * animM._11 + 
			m._42 * animM._21 +
			m._43 * animM._31 +
			m._44 * animM._41;
		float a42 =
			m._41 * animM._12 + 
			m._42 * animM._22 +
			m._43 * animM._32 +
			m._44 * animM._42;
		float a43 =
			m._41 * animM._13 + 
			m._42 * animM._23 +
			m._43 * animM._33 +
			m._44 * animM._43;

		float a41_ =
			tm.Move.x * animTM.Rot.Row[0].x + 
			tm.Move.y * animTM.Rot.Row[1].x +
			tm.Move.z * animTM.Rot.Row[2].x +
			1.0f      * animTM.Move.x;
		float a42_ =
			tm.Move.x * animTM.Rot.Row[0].y + 
			tm.Move.y * animTM.Rot.Row[1].y +
			tm.Move.z * animTM.Rot.Row[2].y +
			1.0f      * animTM.Move.y;
		float a43_ =
			tm.Move.x * animTM.Rot.Row[0].z + 
			tm.Move.y * animTM.Rot.Row[1].z +
			tm.Move.z * animTM.Rot.Row[2].z +
			1.0f      * animTM.Move.z;

//		Vec3_setMat34MulVec3(&t->Move, tm, &animTM->Move);
//		Mat33_mul(&t->Rot, &t0->Rot, &t1->Rot);



//void Mat34_mul(__reg("a2") Mat34* t, __reg("a0") Mat34* tm, __reg("a1") Mat34* animTM)
//{
//  Vec3_setMat34MulVec3(&t->Move, tm, &animTM->Move);
//  Mat33_mul(&t->Rot, &t0->Rot, &t1->Rot);
//
//
//}

//		Mat34_mul( &worldMatrices[i], &tm, (Mat34*)&animTM );
		Mat34_mul( &worldMatrices[i], (Mat34*)&src, &tm );
		worldMatrices[i] = worldMatrices[i];
	}

//	i = 0;

	while( i < worldMatrices.size() )
		Mat34_setIdentity( &worldMatrices[i++] );

	for( size_t q = 0; q < skin.vertexCount; ++q )
	{
		CTransform::t_vector pos3( skin.positions[q].data[0], skin.positions[q].data[1], skin.positions[q].data[2] );
		CTransform::t_vector accum3( 0.0f, 0.0f, 0.0f );
		D3DXVECTOR3 pos( skin.positions[q].data[0], skin.positions[q].data[1], skin.positions[q].data[2] );
		D3DXVECTOR3 accum( 0.0f, 0.0f, 0.0f );
		float accumWeight = 0.0f;
		const size_t weightsPerVertex = skin.weightsPerVertex;
		for( size_t w = 0; w < weightsPerVertex; ++w )
		{
			unsigned short boneId = skin.boneIndices[q * skin.weightsPerVertex + w];
			float boneWeight = skin.weights[q * skin.weightsPerVertex + w];
			if( w != weightsPerVertex - 1 )
				accumWeight += boneWeight;
			else
				boneWeight = 1.0f - accumWeight;

			CTransform::t_matrix& src = worldMatrices[boneId];
			D3DXMATRIX animM(
				src.Rot.Row[0].x, src.Rot.Row[0].y, src.Rot.Row[0].z, 0.0f,
				src.Rot.Row[1].x, src.Rot.Row[1].y, src.Rot.Row[1].z, 0.0f,
				src.Rot.Row[2].x, src.Rot.Row[2].y, src.Rot.Row[2].z, 0.0f,
				src.Move.x, src.Move.y, src.Move.z, 1.0f
			); 
			CTransform::t_matrix animTM(
				src.Rot.Row[0].x, src.Rot.Row[1].x, src.Rot.Row[2].x,
				src.Rot.Row[0].y, src.Rot.Row[1].y, src.Rot.Row[2].y,
				src.Rot.Row[0].z, src.Rot.Row[1].z, src.Rot.Row[2].z,
				src.Move.x, src.Move.y, src.Move.z
				);

			Vec3 v3;
//			Vec3_setMat34MulVec3( &v3, &animTM, &pos3 );//worldMatrices[boneId], &pos3 );
			Vec3_setMat34MulVec3( &v3, &worldMatrices[boneId], &pos3 );
			Vec3_scale( &v3, &v3, boneWeight );
			Vec3_add( &accum3, &accum3, &v3 );

			D3DXVECTOR3 v;
			D3DXVec3TransformCoord( &v, &pos, &animM );
			accum += v * boneWeight;			
		}

		vbuffer[0] = accum3.x;
		vbuffer[1] = accum3.y;
		vbuffer[2] = accum3.z;
		vbuffer = (float*)((u8*)vbuffer + stride);
	}
}*/

/*void updateSkinMesh5( mutant::simple_skinned& skin, BoneMapT& boneMap, CTransform::t_matrix const* data, float* vbuffer, size_t stride )
{
	static std::vector<CTransform::t_matrix> worldMatrices;
	worldMatrices.resize( boneMap.size() );

	unsigned i = 0;
	BoneMapT::iterator bIdIt = boneMap.begin();
	for( ; (i < boneMap.size()) && (bIdIt != boneMap.end()); ++bIdIt, ++i )
	{
		float* mat16 = skin.bones[ bIdIt->first ].matrix.data;
		CTransform::t_matrix tm( // new-age wants transposed matrix
			mat16[0], mat16[4], mat16[8],
			mat16[1], mat16[5], mat16[9],
			mat16[2], mat16[6], mat16[10],
			mat16[12], mat16[13], mat16[14]
		);
		worldMatrices[i] = tm;
		Mat34_mul( &worldMatrices[i], (Mat34*)&data[ bIdIt->second ], &tm );
		worldMatrices[i] = worldMatrices[i];
	}

	while( i < worldMatrices.size() )
		Mat34_setIdentity( &worldMatrices[i++] );

//	i = 0;
//	static unsigned x = 16;
//	bIdIt = boneMap.begin();
//	for( ; (i < boneMap.size()) && (bIdIt != boneMap.end()); ++bIdIt, ++i )
//		if(bIdIt->first != x)
//			Mat34_setIdentity( &worldMatrices[i] );

	for( size_t q = 0; q < skin.vertexCount; ++q )
	{
		CTransform::t_vector pos3( skin.positions[q].data[0], skin.positions[q].data[1], skin.positions[q].data[2] );
		CTransform::t_vector nrm3( skin.normals[q].data[0], skin.normals[q].data[1], skin.normals[q].data[2] );
		CTransform::t_vector accumP3( 0.0f, 0.0f, 0.0f ), accumN3( 0.0f, 0.0f, 0.0f );
		float accumWeight = 0.0f;
		const size_t weightsPerVertex = skin.weightsPerVertex;
		for( size_t w = 0; w < weightsPerVertex; ++w )
		{
			unsigned short boneId = skin.boneIndices[q * skin.weightsPerVertex + w];
			float boneWeight = skin.weights[q * skin.weightsPerVertex + w];
			if( w != weightsPerVertex - 1 )
				accumWeight += boneWeight;
			else
				boneWeight = 1.0f - accumWeight;

			Vec3 v3;
			Vec3_setMat34MulVec3( &v3, &worldMatrices[boneId], &pos3 );
			Vec3_scale( &v3, &v3, boneWeight );
			Vec3_add( &accumP3, &accumP3, &v3 );

			Vec3 n3;
			Vec3_setMat33MulVec3( &n3, &worldMatrices[boneId].Rot, &nrm3 );
			Vec3_scale( &n3, &n3, boneWeight );
			Vec3_add( &accumN3, &accumN3, &n3 );
		}

		vbuffer[0] = accumP3.x;
		vbuffer[1] = accumP3.y;
		vbuffer[2] = accumP3.z;

		vbuffer[3] = accumN3.x;
		vbuffer[4] = accumN3.y;
		vbuffer[5] = accumN3.z;

		vbuffer = (float*)((u8*)vbuffer + stride);
	}
}

void copySkinMesh( mutant::simple_skinned& skin, float* vbuffer, size_t stride )
{
	for( size_t q = 0; q < skin.vertexCount; ++q )
	{
		CTransform::t_vector pos3( skin.positions[q].data[0], skin.positions[q].data[1], skin.positions[q].data[2] );
		CTransform::t_vector nrm3( skin.normals[q].data[0], skin.normals[q].data[1], skin.normals[q].data[2] );

		vbuffer[0] = pos3.x;
		vbuffer[1] = pos3.y;
		vbuffer[2] = pos3.z;

		vbuffer[3] = nrm3.x;
		vbuffer[4] = nrm3.y;
		vbuffer[5] = nrm3.z;

		vbuffer = (float*)((u8*)vbuffer + stride);
	}
}


std::auto_ptr<mutant::anim_character_set> animCharSet;
std::auto_ptr<mutant::simple_skinned> skin;
float* vbuffer;
void* ibuffer;
BoneMapT boneMap;
CTransformArrayAnimator xformArrayAnimator;

typedef std::vector<CTransform> TransformsT;
typedef std::vector<CTransform::t_matrix> MatricesT;
TransformsT transforms;
MatricesT matrices;

template <typename Device>
void updateAnim( Device* d )
{
	static float val = 0.0f;
	val++;
	if( animCharSet.get() )
	{
		xformArrayAnimator.updateTransforms( (float)val * 0.003f,
			transforms.begin(), transforms.end() );

//		mutant::anim_character& animChar = (*animCharSet)["Mutant"];
		mutant::anim_character& animChar = (*animCharSet)[gCharName];
		mutant::anim_hierarchy& charHierarchy = animChar.hierarchy( mutant::sTypeNames::HIERARCHY_DEFAULT ) ;
		CAnimatorAlgos::transformHierarchy(
			matrices.begin(), matrices.end(),
			transforms.begin(), charHierarchy );
	}
}
		
template <typename Device>
void renderAnim( Device* d )
{
	if( animCharSet.get() )
	{
		if(!transforms.empty())
			updateSkinMesh5( *skin, boneMap, &*matrices.begin(), vbuffer, sizeof(float)*6 );
		else
			copySkinMesh( *skin, vbuffer, sizeof(float)*6 );

		d->SetFVF(D3DFVF_XYZ|D3DFVF_NORMAL);
		d->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 
			0, 
			skin->vertexCount,
			skin->indexCount / 3,
			ibuffer,
			D3DFMT_INDEX16,
			vbuffer,
			sizeof(float)*6 );
	}
}
*/

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
ID3DXFont*              g_pFont = NULL;         // Font for drawing text
ID3DXSprite*            g_pSprite = NULL;       // Sprite for batching draw text calls
bool_                    g_bShowHelp = true;     // If true, it renders the UI control text
CModelViewerCamera      g_Camera;               // A model viewing camera
ID3DXEffect*            g_pEffect = NULL;       // D3DX effect interface
ID3DXMesh*              g_pMesh = NULL;         // Mesh object
//IDirect3DTexture9*      g_pMeshTexture = NULL;  // Mesh texture
CDXUTDialogResourceManager g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg         g_SettingsDlg;          // Device settings dialog
CDXUTDialog             g_HUD;                  // manages the 3D UI
CDXUTDialog             g_SampleUI;             // dialog for sample specific controls
bool_                    g_bEnablePreshader;     // if TRUE, then D3DXSHADER_NO_PRESHADER is used when compiling the shader
D3DXMATRIXA16           g_mCenterWorld;

#define MAX_LIGHTS 4
CDXUTDirectionWidget g_LightControl[MAX_LIGHTS];
float                g_fLightScale;
int                  g_nNumActiveLights;
int                  g_nActiveLight;


//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           3
#define IDC_CHANGEDEVICE        4
#define IDC_ENABLE_PRESHADER    5
#define IDC_NUM_LIGHTS          6
#define IDC_NUM_LIGHTS_STATIC   7
#define IDC_ACTIVE_LIGHT        8
#define IDC_LIGHT_SCALE         9
#define IDC_LIGHT_SCALE_STATIC  10


//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
bool_    CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool_ bWindowed, void* pUserContext );
bool_    CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void    CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void    CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool_* pbNoFurtherProcessing, void* pUserContext );
void    CALLBACK KeyboardProc( UINT nChar, bool_ bKeyDown, bool_ bAltDown, void* pUserContext );
void    CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void    CALLBACK OnLostDevice( void* pUserContext );
void    CALLBACK OnDestroyDevice( void* pUserContext );

void    InitApp();
HRESULT LoadMesh( IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh );
void    RenderText( double fTime );


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR cmdLine, int )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	if(*cmdLine)
		gSceneFileName = cmdLine;

    // Set the callback functions. These functions allow DXUT to notify
    // the application about device changes, user input, and windows messages.  The 
    // callbacks are optional so you need only set callbacks for events you're interested 
    // in. However, if you don't handle the device reset/lost callbacks then the sample 
    // framework won't be able to reset your device since the application must first 
    // release all device resources before resetting.  Likewise, if you don't handle the 
    // device created/destroyed callbacks then DXUT won't be able to 
    // recreate your device resources.
    DXUTSetCallbackDeviceCreated( OnCreateDevice );
    DXUTSetCallbackDeviceReset( OnResetDevice );
    DXUTSetCallbackDeviceLost( OnLostDevice );
    DXUTSetCallbackDeviceDestroyed( OnDestroyDevice );
    DXUTSetCallbackMsgProc( (LPDXUTCALLBACKMSGPROC)MsgProc );
    DXUTSetCallbackKeyboard( (LPDXUTCALLBACKKEYBOARD)KeyboardProc );
    DXUTSetCallbackFrameRender( OnFrameRender );
    DXUTSetCallbackFrameMove( OnFrameMove );

    // Show the cursor and clip it when in full screen
    DXUTSetCursorSettings( true, true );

    InitApp();

    // Initialize DXUT and create the desired Win32 window and Direct3D 
    // device for the application. Calling each of these functions is optional, but they
    // allow you to set several options which control the behavior of the framework.
    DXUTInit( true, true, true ); // Parse the command line, handle the default hotkeys, and show msgboxes
    DXUTCreateWindow( L"BasicHLSL" );
    DXUTCreateDevice( D3DADAPTER_DEFAULT, true, 480*2, 282*2, (LPDXUTCALLBACKISDEVICEACCEPTABLE)IsDeviceAcceptable, (LPDXUTCALLBACKMODIFYDEVICESETTINGS)ModifyDeviceSettings );

    // Pass control to DXUT for handling the message pump and 
    // dispatching render calls. DXUT will call your FrameMove 
    // and FrameRender callback when there is idle time between handling window messages.
    DXUTMainLoop();

    // Perform any application-level cleanup here. Direct3D device resources are released within the
    // appropriate callback functions and therefore don't require any cleanup code here.

    return DXUTGetExitCode();
}


//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
    g_bEnablePreshader = true;

    for( int i=0; i<MAX_LIGHTS; i++ )
        g_LightControl[i].SetLightDirection( D3DXVECTOR3( sinf(D3DX_PI*2*i/MAX_LIGHTS-D3DX_PI/6), 0, -cosf(D3DX_PI*2*i/MAX_LIGHTS-D3DX_PI/6) ) );

    g_nActiveLight = 0;
    g_nNumActiveLights = 1;
    g_fLightScale = 1.0f;

    // Initialize dialogs
    g_SettingsDlg.Init( &g_DialogResourceManager );
    g_HUD.Init( &g_DialogResourceManager );
    g_SampleUI.Init( &g_DialogResourceManager );

    g_HUD.SetCallback( OnGUIEvent ); int iY = 10; 
    g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
    g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22 );
    g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22, VK_F2 );

    g_SampleUI.SetCallback( OnGUIEvent ); iY = 10; 

    WCHAR sz[100];
    iY += 24;
    StringCchPrintf( sz, 100, L"# Lights: %d", g_nNumActiveLights ); 
    g_SampleUI.AddStatic( IDC_NUM_LIGHTS_STATIC, sz, 35, iY += 24, 125, 22 );
    g_SampleUI.AddSlider( IDC_NUM_LIGHTS, 50, iY += 24, 100, 22, 1, MAX_LIGHTS, g_nNumActiveLights );

    iY += 24;
    StringCchPrintf( sz, 100, L"Light scale: %0.2f", g_fLightScale ); 
    g_SampleUI.AddStatic( IDC_LIGHT_SCALE_STATIC, sz, 35, iY += 24, 125, 22 );
    g_SampleUI.AddSlider( IDC_LIGHT_SCALE, 50, iY += 24, 100, 22, 0, 20, (int) (g_fLightScale * 10.0f) );

    iY += 24;
    g_SampleUI.AddButton( IDC_ACTIVE_LIGHT, L"Change active light (K)", 35, iY += 24, 125, 22, 'K' );
    g_SampleUI.AddCheckBox( IDC_ENABLE_PRESHADER, L"Enable preshaders", 35, iY += 24, 125, 22, g_bEnablePreshader );
}


//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some 
// minimum set of capabilities, and rejects those that don't pass by returning E_FAIL.
//--------------------------------------------------------------------------------------
bool_ CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, 
                                  D3DFORMAT BackBufferFormat, bool_ bWindowed, void* pUserContext )
{
    // No fallback defined by this app, so reject any device that 
    // doesn't support at least ps1.1
    if( pCaps->PixelShaderVersion < D3DPS_VERSION(1,1) )
        return false;

    // Skip backbuffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3DObject(); 
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                    AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
                    D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the 
// application to modify the device settings. The supplied pDeviceSettings parameter 
// contains the settings that the framework has selected for the new device, and the 
// application can make any desired changes directly to this structure.  Note however that 
// DXUT will not correct invalid device settings so care must be taken 
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.  
//--------------------------------------------------------------------------------------
bool_ CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext )
{
    // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
    // then switch to SWVP.
    if( (pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
         pCaps->VertexShaderVersion < D3DVS_VERSION(1,1) )
    {
        pDeviceSettings->BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    // Debugging vertex shaders requires either REF or software vertex processing 
    // and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
    if( pDeviceSettings->DeviceType != D3DDEVTYPE_REF )
    {
        pDeviceSettings->BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
        pDeviceSettings->BehaviorFlags &= ~D3DCREATE_PUREDEVICE;                            
        pDeviceSettings->BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
#endif
#ifdef DEBUG_PS
    pDeviceSettings->DeviceType = D3DDEVTYPE_REF;
#endif
    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool_ s_bFirstTime = true;
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        if( pDeviceSettings->DeviceType == D3DDEVTYPE_REF )
            DXUTDisplaySwitchingToREFWarning();
    }

    return true;
}

//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// created, which will happen during application initialization and windowed/full screen 
// toggles. This is the best location to create D3DPOOL_MANAGED resources since these 
// resources need to be reloaded whenever the device is destroyed. Resources created  
// here should be released in the OnDestroyDevice callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

    V_RETURN( g_DialogResourceManager.OnCreateDevice( pd3dDevice ) );
    V_RETURN( g_SettingsDlg.OnCreateDevice( pd3dDevice ) );
    // Initialize the font
    V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                              OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                              L"Arial", &g_pFont ) );

    // Load the mesh
//	V_RETURN( LoadMesh( pd3dDevice, L"test.x", &g_pMesh ) );

    D3DXVECTOR3* pData; 
    D3DXVECTOR3 vCenter;
    FLOAT fObjectRadius;
//    V( g_pMesh->LockVertexBuffer( 0, (LPVOID*) &pData ) );
//    V( D3DXComputeBoundingSphere( pData, g_pMesh->GetNumVertices(), D3DXGetFVFVertexSize( g_pMesh->GetFVF() ), &vCenter, &fObjectRadius ) );
//    V( g_pMesh->UnlockVertexBuffer() );

	static float newRadius = 250.0f;
	if(newRadius > 0)
	{
		vCenter.x = 0;
		vCenter.y = 0;
		vCenter.z = 0;
		fObjectRadius = newRadius;
	}

    D3DXMatrixTranslation( &g_mCenterWorld, -vCenter.x, -vCenter.y, -vCenter.z );
    D3DXMATRIXA16 m;
    D3DXMatrixRotationY( &m, D3DX_PI );
    g_mCenterWorld *= m;
    D3DXMatrixRotationX( &m, D3DX_PI / 2.0f );
    g_mCenterWorld *= m;

    V_RETURN( CDXUTDirectionWidget::StaticOnCreateDevice( pd3dDevice ) );
    for( int i=0; i<MAX_LIGHTS; i++ )
        g_LightControl[i].SetRadius( fObjectRadius );

//	g_pMesh->Release();
	g_pMesh = 0;
	static float radius = 10.0f;
	V_RETURN( D3DXCreateSphere( pd3dDevice, radius, 6, 6, &g_pMesh, 0 ) );


    // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
    // shader debugger. Debugging vertex shaders requires either REF or software vertex 
    // processing, and debugging pixel shaders requires REF.  The 
    // D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the 
    // shader debugger.  It enables source level debugging, prevents instruction 
    // reordering, prevents dead code elimination, and forces the compiler to compile 
    // against the next higher available software target, which ensures that the 
    // unoptimized shaders do not exceed the shader model limitations.  Setting these 
    // flags will cause slower rendering since the shaders will be unoptimized and 
    // forced into software.  See the DirectX documentation for more information about 
    // using the shader debugger.
    DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
    #ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    #endif
    #ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    #endif

    // Preshaders are parts of the shader that the effect system pulls out of the 
    // shader and runs on the host CPU. They should be used if you are GPU limited. 
    // The D3DXSHADER_NO_PRESHADER flag disables preshaders.
    if( !g_bEnablePreshader )
        dwShaderFlags |= D3DXSHADER_NO_PRESHADER;

    // Read the D3DX effect file
    WCHAR str[MAX_PATH];
    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"MutaliskUberShader.fx" ) );

    // If this fails, there should be debug output as to 
    // why the .fx file failed to compile
	com_ptr<ID3DXBuffer> errorBuffer;
    HRESULT hr2 = D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags, NULL, &g_pEffect, &errorBuffer );
	if(errorBuffer)
	{
		std::string errorStr = std::string((char*)errorBuffer->GetBufferPointer(), (char*)errorBuffer->GetBufferSize());
	}
	V_RETURN(hr2);

    // Create the mesh texture from a file
/*    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"test.bmp" ) );

    V_RETURN( D3DXCreateTextureFromFileEx( pd3dDevice, str, D3DX_DEFAULT, D3DX_DEFAULT, 
                                       D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
                                       D3DX_DEFAULT, D3DX_DEFAULT, 0, 
                                       NULL, NULL, &g_pMeshTexture ) );
*/

    // Setup the camera's view parameters
    D3DXVECTOR3 vecEye(0.0f, 0.0f, -15.0f);
    D3DXVECTOR3 vecAt (0.0f, 0.0f, -0.0f);
    g_Camera.SetViewParams( &vecEye, &vecAt );
    g_Camera.SetRadius( fObjectRadius*3.0f, fObjectRadius*0.5f, fObjectRadius*10.0f );

//---

	scenePlayerApp.reset(new ScenePlayerApp(gSceneFileName, *pd3dDevice, *g_pEffect));

//---
/*
	animCharSet = loadAnimation(gAnimFileName);
//	mutant::anim_character& animChar = (*animCharSet)["Mutant"];
//	mutant::anim_character& animChar = (*animCharSet)["character"];
	mutant::anim_character& animChar = (*animCharSet)[gCharName];
	mutant::anim_hierarchy& charHierarchy = animChar.hierarchy( mutant::sTypeNames::HIERARCHY_DEFAULT ) ;
	xformArrayAnimator.createFromClip(
//		animChar["attack1"], charHierarchy );
//		animChar["LW Take 001"], charHierarchy );
		animChar[gClipName], charHierarchy );

	skin = loadSkin(gSkinFileName);

	if(skin.get())
		skinToBuffers( *skin, (void**)&vbuffer, &ibuffer );

	if(gEnableAnimations)
	{
		mapSkinnedBonesToHierarchy( skin->bones, skin->boneCount, charHierarchy, boneMap );
		transforms.resize( charHierarchy.size(), CTransform::identity() );
		matrices.resize( charHierarchy.size() );
	}

	if(!gLoadSkin)
		skin.release();

//---
*/


    return S_OK;
}

//--------------------------------------------------------------------------------------
// This function loads the mesh and ensures the mesh has normals; it also optimizes the 
// mesh for the graphics card's vertex cache, which improves performance by organizing 
// the internal triangle list for less cache misses.
//--------------------------------------------------------------------------------------
HRESULT LoadMesh( IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh )
{
    ID3DXMesh* pMesh = NULL;
    WCHAR str[MAX_PATH];
    HRESULT hr;

    // Load the mesh with D3DX and get back a ID3DXMesh*.  For this
    // sample we'll ignore the X file's embedded materials since we know 
    // exactly the model we're loading.  See the mesh samples such as
    // "OptimizedMesh" for a more generic mesh loading example.
    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, strFileName ) );
    V_RETURN( D3DXLoadMeshFromX(str, D3DXMESH_MANAGED, pd3dDevice, NULL, NULL, NULL, NULL, &pMesh) );

    DWORD *rgdwAdjacency = NULL;

    // Make sure there are normals which are required for lighting
    if( !(pMesh->GetFVF() & D3DFVF_NORMAL) )
    {
        ID3DXMesh* pTempMesh;
        V( pMesh->CloneMeshFVF( pMesh->GetOptions(), 
                                  pMesh->GetFVF() | D3DFVF_NORMAL, 
                                  pd3dDevice, &pTempMesh ) );
        V( D3DXComputeNormals( pTempMesh, NULL ) );

        SAFE_RELEASE( pMesh );
        pMesh = pTempMesh;
    }

    // Optimize the mesh for this graphics card's vertex cache 
    // so when rendering the mesh's triangle list the vertices will 
    // cache hit more often so it won't have to re-execute the vertex shader 
    // on those vertices so it will improve perf.     
    rgdwAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
    if( rgdwAdjacency == NULL )
        return E_OUTOFMEMORY;
    V( pMesh->GenerateAdjacency(1e-6f,rgdwAdjacency) );
    V( pMesh->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE, rgdwAdjacency, NULL, NULL, NULL) );
    delete []rgdwAdjacency;

    *ppMesh = pMesh;

    return S_OK;
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// reset, which will happen after a lost device scenario. This is the best location to 
// create D3DPOOL_DEFAULT resources since these resources need to be reloaded whenever 
// the device is lost. Resources created here should be released in the OnLostDevice 
// callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, 
                                const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

    V_RETURN( g_DialogResourceManager.OnResetDevice() );
    V_RETURN( g_SettingsDlg.OnResetDevice() );

    if( g_pFont )
        V_RETURN( g_pFont->OnResetDevice() );
    if( g_pEffect )
        V_RETURN( g_pEffect->OnResetDevice() );

    // Create a sprite to help batch calls when drawing many lines of text
    V_RETURN( D3DXCreateSprite( pd3dDevice, &g_pSprite ) );

    for( int i=0; i<MAX_LIGHTS; i++ )
        g_LightControl[i].OnResetDevice( pBackBufferSurfaceDesc  );

    // Setup the camera's projection parameters
    float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
    g_Camera.SetProjParams( D3DX_PI/4, fAspectRatio, 2.0f, 40000.0f );
    g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
    g_Camera.SetButtonMasks( MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_MIDDLE_BUTTON );

    g_HUD.SetLocation( pBackBufferSurfaceDesc->Width-170, 0 );
    g_HUD.SetSize( 170, 170 );
    g_SampleUI.SetLocation( pBackBufferSurfaceDesc->Width-170, pBackBufferSurfaceDesc->Height-300 );
    g_SampleUI.SetSize( 170, 300 );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// This callback function will be called once at the beginning of every frame. This is the
// best location for your application to handle updates to the scene, but is not 
// intended to contain actual rendering calls, which should instead be placed in the 
// OnFrameRender callback.  
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    // Update the camera's position based on user input 
    g_Camera.FrameMove( fElapsedTime );
}


//--------------------------------------------------------------------------------------
// This callback function will be called at the end of every frame to perform all the 
// rendering calls for the scene, and it will also be called if the window needs to be 
// repainted. After this function has returned, DXUT will call 
// IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    // If the settings dialog is being shown, then
    // render it instead of rendering the app's scene
    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.OnRender( fElapsedTime );
        return;
    }

    HRESULT hr;
    D3DXMATRIXA16 mWorldViewProjection;
    D3DXVECTOR3 vLightDir[MAX_LIGHTS];
    D3DXCOLOR   vLightDiffuse[MAX_LIGHTS];
    UINT iPass, cPasses;
    D3DXMATRIXA16 mWorld;
    D3DXMATRIXA16 mView;
    D3DXMATRIXA16 mProj;
   
    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0.0f,0.25f,0.25f,0.55f), 1.0f, 0) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		D3DXMATRIX cameraInvMatrix;
		D3DXMatrixInverse(&cameraInvMatrix, 0, g_Camera.GetWorldMatrix());
        // Get the projection & view matrix from the camera class
        mWorld = g_mCenterWorld * *g_Camera.GetWorldMatrix();
		mProj = *g_Camera.GetProjMatrix();
        mView = *g_Camera.GetViewMatrix();

/*		updateAnim( pd3dDevice );

		static std::vector<CTransform::t_matrix> worldMatrices;
		{
			worldMatrices.resize( boneMap.size() );

			unsigned i = 0;
			BoneMapT::iterator bIdIt = boneMap.begin();
			for( ; (i < boneMap.size()) && (bIdIt != boneMap.end()); ++bIdIt, ++i )
			{
				CTransform::t_matrix skinM;
				if(skin.get())
				{
					float* mat16 = skin->bones[ bIdIt->first ].matrix.data;
					skinM = CTransform::t_matrix( // new-age wants transposed matrix
						mat16[0], mat16[4], mat16[8],
						mat16[1], mat16[5], mat16[9],
						mat16[2], mat16[6], mat16[10],
	//					mat16[0], mat16[1], mat16[2],
	//					mat16[4], mat16[5], mat16[6],
	//					mat16[8], mat16[9], mat16[10],
	//					1, 0, 0,
	//					0, 1, 0,
	//					0, 0, 1,
	//					0, 0, 0
						mat16[12], mat16[13], mat16[14]
					);
				}
				else
				{
					Mat34_setIdentity(&skinM);
				}
				CTransform::t_matrix animM = matrices[ bIdIt->second ];
//				Mat33_transpose(&animM.Rot, &animM.Rot);
//				Mat33_setIdentity(&animM.Rot);
//				animM.Move.x = 0;
//				animM.Move.y = 0;
//				animM.Move.z = 0;

				worldMatrices[i] = animM;
//				Mat34_mul( &worldMatrices[i], (Mat34*)&animM, &skinM );
//				Mat34_mul( &worldMatrices[i], &tm, (Mat34*)&animM );
				worldMatrices[i] = worldMatrices[i];
			}
		}

		V( g_pEffect->SetTechnique( "RenderSceneWithTexture1Light" ) );//"Debug" ) )

        // Apply the technique contained in the effect 
        V( g_pEffect->Begin(&cPasses, 0) );
        for (iPass = 0; iPass < cPasses; iPass++)
        {
            V( g_pEffect->BeginPass(iPass) );			
			for(MatricesT::const_iterator it = worldMatrices.begin(); it < worldMatrices.end(); ++it)
			{
				CTransform::t_matrix const& src = *it;
				D3DXMATRIX animM;
				static int method = 0;

				if(method == 0)
				{
					animM = D3DXMATRIX (
						src.Rot.Row[0].x, src.Rot.Row[1].x, src.Rot.Row[2].x, 0.0f,
						src.Rot.Row[0].y, src.Rot.Row[1].y, src.Rot.Row[2].y, 0.0f,
						src.Rot.Row[0].z, src.Rot.Row[1].z, src.Rot.Row[2].z, 0.0f,
						src.Move.x, src.Move.y, src.Move.z, 1.0f
					);
				}

				if(method == 1)
				{
					animM = D3DXMATRIX (
						src.Rot.Row[0].x, src.Rot.Row[0].y, src.Rot.Row[0].z, 0.0f,
						src.Rot.Row[1].x, src.Rot.Row[1].y, src.Rot.Row[1].z, 0.0f,
						src.Rot.Row[2].x, src.Rot.Row[2].y, src.Rot.Row[2].z, 0.0f,
						src.Move.x, src.Move.y, src.Move.z, 1.0f
					);
				}

				if(method == 2)
				{
					animM = D3DXMATRIX(
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						src.Move.x, src.Move.y, src.Move.z, 1.0f
					);
				}

				D3DXMATRIX worldViewProjection = animM * mWorld * mView * mProj;

				V( g_pEffect->SetMatrix( "g_mWorldViewProjection", &worldViewProjection ) );
				V( g_pEffect->SetMatrix( "g_mWorld", &animM ) );
				V( g_pEffect->CommitChanges() );

				if(gRenderDebugSkeleton)
					V( g_pMesh->DrawSubset(0) );
			}

            V( g_pEffect->EndPass() );
        }
        V( g_pEffect->End() );

*/

		mWorldViewProjection = mWorld * mView * mProj;
        for( int i=0; i<g_nNumActiveLights; i++ )
        {
            vLightDir[i] = g_LightControl[i].GetLightDirection();
            vLightDiffuse[i] = g_fLightScale * D3DXCOLOR(1,1,1,1);
            
			D3DXCOLOR arrowColor = ( i == g_nActiveLight ) ? D3DXCOLOR(1,1,0,1) : D3DXCOLOR(1,1,1,1);
            V( g_LightControl[i].OnRender( arrowColor, &mView, &mProj, g_Camera.GetEyePt() ) );
        }

        V( g_pEffect->SetTechnique( "Main" ) );
        V( g_pEffect->SetValue( "vLightDir", vLightDir, sizeof(D3DXVECTOR3)*MAX_LIGHTS ) );
        V( g_pEffect->SetValue( "vLightDiffuse", vLightDiffuse, sizeof(D3DXVECTOR4)*MAX_LIGHTS ) );
		int lightTypes[MAX_LIGHTS] = {0,0,0,0}; // lightDIRECTIONAL = 0
		V( g_pEffect->SetIntArray( "nLightType", lightTypes, MAX_LIGHTS ) );
		V( g_pEffect->SetInt( "iNumLights", g_nNumActiveLights ) );
		if(g_nNumActiveLights > 0)
		{
			D3DXVECTOR4 ambientColor = D3DXVECTOR4(0,0,0,0);
			V( g_pEffect->SetValue( "vLightAmbient", ambientColor, sizeof(ambientColor) ) );
		}

		scenePlayerApp->setViewMatrix(mView);
		scenePlayerApp->setProjMatrix(mProj);
		scenePlayerApp->update(static_cast<float>(fTime));
		scenePlayerApp->process();
		static int maxActors = -1;
		scenePlayerApp->render(maxActors);

#if 0
        // Apply the technique contained in the effect 
        V( g_pEffect->Begin(&cPasses, 0) );

        for (iPass = 0; iPass < cPasses; iPass++)
        {
            V( g_pEffect->BeginPass(iPass) );

            // The effect interface queues up the changes and performs them 
            // with the CommitChanges call. You do not need to call CommitChanges if 
            // you are not setting any parameters between the BeginPass and EndPass.
            // V( g_pEffect->CommitChanges() );

            // Render the mesh with the applied technique
//			V( g_pMesh->DrawSubset(0) );

			if(skin.get() && gRenderSkin)
				renderAnim( pd3dDevice );	

            V( g_pEffect->EndPass() );
        }
        V( g_pEffect->End() );
#endif
        g_HUD.OnRender( fElapsedTime ); 
        g_SampleUI.OnRender( fElapsedTime );

        RenderText( fTime );
        
        V( pd3dDevice->EndScene() );
    }
}


//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText( double fTime )
{
    // The helper object simply helps keep track of text position, and color
    // and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
    // If NULL is passed in as the sprite object, then it will work fine however the 
    // pFont->DrawText() will not be batched together.  Batching calls will improves perf.
    CDXUTTextHelper txtHelper( g_pFont, g_pSprite, 15 );

    // Output statistics
    txtHelper.Begin();
    txtHelper.SetInsertionPos( 2, 0 );
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
    txtHelper.DrawTextLine( DXUTGetFrameStats() );
    txtHelper.DrawTextLine( DXUTGetDeviceStats() );
    
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
    txtHelper.DrawFormattedTextLine( L"fTime: %0.1f  sin(fTime): %0.4f", fTime, sin(fTime) );
    
    // Draw help
    if( g_bShowHelp )
    {
        const D3DSURFACE_DESC* pd3dsdBackBuffer = DXUTGetBackBufferSurfaceDesc();
        txtHelper.SetInsertionPos( 2, pd3dsdBackBuffer->Height-15*6 );
        txtHelper.SetForegroundColor( D3DXCOLOR(1.0f, 0.75f, 0.0f, 1.0f ) );
        txtHelper.DrawTextLine( L"Controls:" );

        txtHelper.SetInsertionPos( 20, pd3dsdBackBuffer->Height-15*5 );
        txtHelper.DrawTextLine( L"Rotate model: Left mouse button\n"
                                L"Rotate light: Right mouse button\n"
                                L"Rotate camera: Middle mouse button\n"
                                L"Zoom camera: Mouse wheel scroll\n" );

        txtHelper.SetInsertionPos( 250, pd3dsdBackBuffer->Height-15*5 );
        txtHelper.DrawTextLine( L"Hide help: F1\n" 
                                L"Quit: ESC\n" );
    }
    else
    {
        txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
        txtHelper.DrawTextLine( L"Press F1 for help" );
    }
    txtHelper.End();
}


//--------------------------------------------------------------------------------------
// Before handling window messages, DXUT passes incoming windows 
// messages to the application through this callback function. If the application sets 
// *pbNoFurtherProcessing to TRUE, then DXUT will not process this message.
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool_* pbNoFurtherProcessing, void* pUserContext )
{
    // Always allow dialog resource manager calls to handle global messages
    // so GUI state is updated correctly
    *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
        return 0;
    }

    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    *pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    g_LightControl[g_nActiveLight].HandleMessages( hWnd, uMsg, wParam, lParam );

    // Pass all remaining windows messages to camera so it can respond to user input
    g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

    return 0;
}


//--------------------------------------------------------------------------------------
// As a convenience, DXUT inspects the incoming windows messages for
// keystroke messages and decodes the message parameters to pass relevant keyboard
// messages to the application.  The framework does not remove the underlying keystroke 
// messages, which are still passed to the application's MsgProc callback.
//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc( UINT nChar, bool_ bKeyDown, bool_ bAltDown, void* pUserContext )
{
    if( bKeyDown )
    {
        switch( nChar )
        {
            case VK_F1: g_bShowHelp = !g_bShowHelp; break;
        }
    }
}


//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
    
    switch( nControlID )
    {
        case IDC_TOGGLEFULLSCREEN: DXUTToggleFullScreen(); break;
        case IDC_TOGGLEREF:        DXUTToggleREF(); break;
        case IDC_CHANGEDEVICE:     g_SettingsDlg.SetActive( !g_SettingsDlg.IsActive() ); break;

        case IDC_ENABLE_PRESHADER: 
        {
            g_bEnablePreshader = g_SampleUI.GetCheckBox( IDC_ENABLE_PRESHADER )->GetChecked(); 

            if( DXUTGetD3DDevice() != NULL )
            {
                OnLostDevice( NULL );
                OnDestroyDevice( NULL );
                OnCreateDevice( DXUTGetD3DDevice(), DXUTGetBackBufferSurfaceDesc(), NULL );
                OnResetDevice( DXUTGetD3DDevice(), DXUTGetBackBufferSurfaceDesc(), NULL );
            }
            break;
        }

        case IDC_ACTIVE_LIGHT:
            if( !g_LightControl[g_nActiveLight].IsBeingDragged() )
            {
                g_nActiveLight++;
                g_nActiveLight %= g_nNumActiveLights;
            }
            break;

        case IDC_NUM_LIGHTS:
            if( !g_LightControl[g_nActiveLight].IsBeingDragged() )
            {
                WCHAR sz[100];
                StringCchPrintf( sz, 100, L"# Lights: %d", g_SampleUI.GetSlider( IDC_NUM_LIGHTS )->GetValue() ); 
                g_SampleUI.GetStatic( IDC_NUM_LIGHTS_STATIC )->SetText( sz );

                g_nNumActiveLights = g_SampleUI.GetSlider( IDC_NUM_LIGHTS )->GetValue();
                g_nActiveLight %= g_nNumActiveLights;
            }
            break;

        case IDC_LIGHT_SCALE: 
            g_fLightScale = (float) (g_SampleUI.GetSlider( IDC_LIGHT_SCALE )->GetValue() * 0.10f);

            WCHAR sz[100];
            StringCchPrintf( sz, 100, L"Light scale: %0.2f", g_fLightScale ); 
            g_SampleUI.GetStatic( IDC_LIGHT_SCALE_STATIC )->SetText( sz );
            break;
    }
    
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// entered a lost state and before IDirect3DDevice9::Reset is called. Resources created
// in the OnResetDevice callback should be released here, which generally includes all 
// D3DPOOL_DEFAULT resources. See the "Lost Devices" section of the documentation for 
// information about lost devices.
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void* pUserContext )
{
    g_DialogResourceManager.OnLostDevice();
    g_SettingsDlg.OnLostDevice();
    CDXUTDirectionWidget::StaticOnLostDevice();
    if( g_pFont )
        g_pFont->OnLostDevice();
    if( g_pEffect )
        g_pEffect->OnLostDevice();
    SAFE_RELEASE(g_pSprite);
    
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// been destroyed, which generally happens as a result of application termination or 
// windowed/full screen toggles. Resources created in the OnCreateDevice callback 
// should be released here, which generally includes all D3DPOOL_MANAGED resources. 
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
    g_DialogResourceManager.OnDestroyDevice();
    g_SettingsDlg.OnDestroyDevice();
    CDXUTDirectionWidget::StaticOnDestroyDevice();
    SAFE_RELEASE(g_pEffect);
    SAFE_RELEASE(g_pFont);
//    SAFE_RELEASE(g_pMesh);
//    SAFE_RELEASE(g_pMeshTexture);
}



