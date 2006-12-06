#include "ScenePlayer.h"

////////////////////////////////////////////////

void CSkinnedAlgos::processSkinMesh(Vec3 const* srcPositions, Vec3 const* srcNormals, float const* srcWeights, unsigned char const* srcBoneIndices,
	Vec3 *dstPositions, Vec3* dstNormals, size_t srcVertexStride, size_t srcWeightStride, size_t srcBoneIndexStride,
	size_t dstVertexStride, size_t vertexCount,
	mutalisk::data::skin_info const& skinInfo, BoneMapT const& boneMap, CTransform::t_matrix const* matrices)
{		
	static std::vector<CTransform::t_matrix> worldMatrices;
	worldMatrices.resize( boneMap.size() );

	unsigned i = 0;
	BoneMapT::const_iterator bIdIt = boneMap.begin();
	for( ; (i < boneMap.size()) && (bIdIt != boneMap.end()); ++bIdIt, ++i )
	{
		float* mat16 = skinInfo.bones[ bIdIt->first ].matrix.data;
		CTransform::t_matrix tm( // new-age wants transposed matrix
			mat16[0], mat16[1], mat16[2],
			mat16[4], mat16[5], mat16[6],
			mat16[8], mat16[9], mat16[10],
/*			1, 0, 0,
			0, 1, 0,
			0, 0, 1,*/
/*			mat16[0], mat16[4], mat16[8],
			mat16[1], mat16[5], mat16[9],
			mat16[2], mat16[6], mat16[10],
*/
//			0, 0, 0
//			-mat16[12], -mat16[13], -mat16[14]
			mat16[12], mat16[13], mat16[14]
		);

		CTransform::t_matrix am( // new-age wants transposed matrix
			1, 0, 0,
			0, 1, 0,
			0, 0, 1,	
			0, 0, 0
		);
		am = matrices[ bIdIt->second ];

		static float t = 0.0;
		static CTransform::t_matrix prevM;
//		t += 0.001f;
		tm.Move.x = 0;
		tm.Move.y = 0;
		tm.Move.z = 0;
		Vec3 move = tm.Move;
		/*if(i == 0)
		{
			move.x = 0;
			move.y = 0;
			move.z = 0;
		}
		else
		{
			move.x = skinInfo.bones[i-1].matrix.data[12];
			move.y = skinInfo.bones[i-1].matrix.data[13];
			move.z = skinInfo.bones[i-1].matrix.data[14];
		}*/
		Mat34_setRotHPBMove(&am, t, 0, 0, &move);
		if(i>0)
			Mat34_mul(&am, &prevM, &am);
		prevM = am;

//		D3DXMATRIX txm(mat16);
//		D3DXMatrixInverse(&txm, 0, &txm);

/*		CTransform::t_matrix itm(
			txm._11, txm._12, txm._13,
			txm._21, txm._22, txm._23,
			txm._31, txm._32, txm._33,
			txm._41, txm._42, txm._43);
*/
		CTransform::t_matrix itm;
		Mat33_transpose(&itm.Rot, &tm.Rot);
		itm.Move.x = -tm.Move.x;
		itm.Move.y = -tm.Move.y;
		itm.Move.z = -tm.Move.z;

		//tm.Move = am.Move - tm.Move;
		//Vec3_sub(tm.Move, am.Move, tm.Move);
		//Vec3_add(tm.Move, am.Move, itm.Move);
		Mat34_mul( &worldMatrices[i], &am, &itm );
//		Mat34_mul( &worldMatrices[i], &itm, &am );

/*		worldMatrices[i].Rot.Row[0].x = 1;
		worldMatrices[i].Rot.Row[1].y = 1;
		worldMatrices[i].Rot.Row[2].z = 1;
		worldMatrices[i].Rot.Row[0].y = 0;
		worldMatrices[i].Rot.Row[0].z = 0;
		worldMatrices[i].Rot.Row[1].x = 0;
		worldMatrices[i].Rot.Row[1].z = 0;
		worldMatrices[i].Rot.Row[2].x = 0;
		worldMatrices[i].Rot.Row[2].y = 0;*/
///		worldMatrices[i].Move.y = 0;
//		worldMatrices[i].Move.z = 0;


//		worldMatrices[i] = tm;
//		Mat34_mul( &worldMatrices[i], &am, &tm );
//		Mat34_mul( &worldMatrices[i], &tm, &am );
//		worldMatrices[i] = am;
	}

	while( i < worldMatrices.size() )
		Mat34_setIdentity( &worldMatrices[i++] );

//	worldMatrices[2] = worldMatrices[1];
//	worldMatrices[3] = worldMatrices[2];


/*	i = 0;
	static unsigned x = 16;
	bIdIt = boneMap.begin();
	for( ; (i < boneMap.size()) && (bIdIt != boneMap.end()); ++bIdIt, ++i )
		if(bIdIt->first != x)
			Mat34_setIdentity( &worldMatrices[i] );
*/

	Mat34 identityM;
	Mat34_setIdentity( &identityM );

	unsigned char const* srcPositionsRaw = reinterpret_cast<unsigned char const*>(srcPositions);
	unsigned char const* srcNormalsRaw = reinterpret_cast<unsigned char const*>(srcNormals);
	unsigned char const* srcWeightsRaw = reinterpret_cast<unsigned char const*>(srcWeights);
	unsigned char const* srcBoneIndicesRaw = reinterpret_cast<unsigned char const*>(srcBoneIndices);

	unsigned char* dstPositionsRaw = reinterpret_cast<unsigned char*>(dstPositions);
	unsigned char* dstNormalsRaw = reinterpret_cast<unsigned char*>(dstNormals);
	for( size_t q = 0; q < vertexCount; ++q )
	{
		CTransform::t_vector pos3 = *reinterpret_cast<CTransform::t_vector const*>(srcPositionsRaw + q * srcVertexStride);
		CTransform::t_vector nrm3 = *reinterpret_cast<CTransform::t_vector const*>(srcNormalsRaw + q * srcVertexStride);
		float const* weights = reinterpret_cast<float const*>(srcWeightsRaw + q * srcWeightStride);
		unsigned char const* boneIndices = srcBoneIndicesRaw + q * srcBoneIndexStride;
		CTransform::t_vector accumP3( 0.0f, 0.0f, 0.0f ), accumN3( 0.0f, 0.0f, 0.0f );
		float accumWeight = 0.0f;
		for( size_t w = 0; w < skinInfo.weightsPerVertex; ++w )
		{
			unsigned char boneId = boneIndices[w];
			float boneWeight = weights[w];
			if( w != skinInfo.weightsPerVertex - 1 )
				accumWeight += boneWeight;
			else
				boneWeight = 1.0f - accumWeight;

			Mat34 worldMatrix = worldMatrices[boneId];
//			if(boneWeight > 0.95) 
//				boneWeight = 1;
//			else
//				worldMatrix = identityM;

			Vec3 v3;
			Vec3_setMat34MulVec3( &v3, &worldMatrix, &pos3 );
			Vec3_scale( &v3, &v3, boneWeight );
			Vec3_add( &accumP3, &accumP3, &v3 );

			Vec3 n3;
			Vec3_setMat33MulVec3( &n3, &worldMatrix.Rot, &nrm3 );
			Vec3_scale( &n3, &n3, boneWeight );
			Vec3_add( &accumN3, &accumN3, &n3 );
		}

		*dstPositions = accumP3;
		*dstNormals = accumN3;

		dstPositionsRaw += dstVertexStride;
		dstNormalsRaw += dstVertexStride;
		dstPositions = reinterpret_cast<Vec3*>(dstPositionsRaw);
		dstNormals = reinterpret_cast<Vec3*>(dstNormalsRaw);
	}
}
