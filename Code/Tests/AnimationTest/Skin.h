#ifndef __SKIN_H
#define __SKIN_H

typedef std::vector<std::pair<int, int> > BoneMapT;
BoneMapT& mapSkinnedBonesToHierarchy(
	mutant::simple_skinned::Bone* bones, size_t boneCount,
	mutant::anim_hierarchy const& hierarchy,
	BoneMapT& remapedBones )
{
	remapedBones.reserve( remapedBones.size() + boneCount );

	unsigned boneId = 0;
	for( size_t q = 0; q < boneCount; ++q )
	{
		std::string const& boneName = bones[q].name;

		while( hierarchy[ boneId ].name != boneName && boneId < hierarchy.size() )
			++boneId;

		if( hierarchy[ boneId ].name != boneName )
			printf( "Couldn't find '%s' in hierarchy", boneName.c_str() );

		remapedBones.push_back( std::make_pair( q, boneId ) );
	}

	return remapedBones;
}

void updateSkinMesh( mutant::simple_skinned& skin, BoneMapT& boneMap, CTransform::t_matrix const* data, float* vbuffer, size_t stride )
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
		Mat34_mul( &worldMatrices[i], (Mat34*)&data[ bIdIt->second ], &tm );
		worldMatrices[i] = worldMatrices[i];
	}

	while( i < worldMatrices.size() )
		Mat34_setIdentity( &worldMatrices[i++] );

	for( size_t q = 0; q < skin.vertexCount; ++q )
	{
		CTransform::t_vector pos3( skin.positions[q].data[0], skin.positions[q].data[1], skin.positions[q].data[2] );
		CTransform::t_vector accum3( 0.0f, 0.0f, 0.0f );
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
			Vec3_add( &accum3, &accum3, &v3 );
		}

		vbuffer[0] = accum3.x;
		vbuffer[1] = accum3.y;
		vbuffer[2] = accum3.z;
		vbuffer = (float*)((u8*)vbuffer + stride);
	}
}

void updateSkinMesh2( mutant::simple_skinned& skin, BoneMapT& boneMap, CTransform::t_matrix const* data, float* vbuffer, size_t stride, Vec3 spriteR )
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
		Mat34_mul( &worldMatrices[i], (Mat34*)&data[ bIdIt->second ], &tm );
		worldMatrices[i] = worldMatrices[i];
	}

	while( i < worldMatrices.size() )
		Mat34_setIdentity( &worldMatrices[i++] );

	for( size_t q = 0; q < skin.vertexCount; ++q )
	{
		CTransform::t_vector pos3( skin.positions[q].data[0], skin.positions[q].data[1], skin.positions[q].data[2] );
		CTransform::t_vector accum3( 0.0f, 0.0f, 0.0f );
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
			Vec3_add( &accum3, &accum3, &v3 );
		}

		vbuffer[0] = accum3.x - spriteR.x;
		vbuffer[1] = accum3.y - spriteR.y;
		vbuffer[2] = accum3.z - spriteR.z;

		vbuffer[3] = accum3.x + spriteR.x;
		vbuffer[4] = accum3.y + spriteR.y;
		vbuffer[5] = accum3.z + spriteR.z;

		vbuffer = (float*)((u8*)vbuffer + stride*2);
	}
}

#endif // __SKIN_H
