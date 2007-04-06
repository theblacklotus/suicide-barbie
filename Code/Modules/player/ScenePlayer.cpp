#include "ScenePlayer.h"

namespace mutalisk {
////////////////////////////////////////////////
std::string gResourcePath = "";
void setResourcePath(std::string const& path)
{
	gResourcePath = path;
}

std::string getResourcePath()
{
	return gResourcePath;
}

std::auto_ptr<mutant::mutant_reader> createFileReader(std::string const& fileName)
{
	std::auto_ptr<mutant::binary_input> input = mutant::reader_factory::createInput(gResourcePath + fileName);
	std::auto_ptr<mutant::mutant_reader> mutReader(new mutant::mutant_reader(input));
	mutReader->enableLog(false);

	return mutReader;
}

void setMatrix(CTransform::t_matrix& matrix, float const* matrixData)
{
	// new-age wants transposed matrix
	matrix = xMat34(
		matrixData[0],	matrixData[4],	matrixData[8],
		matrixData[1],	matrixData[5],	matrixData[9],
		matrixData[2],	matrixData[6],	matrixData[10],
		matrixData[12],	matrixData[13],	matrixData[14]
	);
}

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
		CTransform::t_matrix tm;
		float const* mat16 = skinInfo.bones[ bIdIt->first ].matrix.data;
		setMatrix(tm, mat16);

		CTransform::t_matrix itm;
		Mat34_invertOrthogonal(&itm, &tm);

		CTransform::t_matrix am( // new-age wants transposed matrix
			1, 0, 0,
			0, 1, 0,
			0, 0, 1,	
			0, 0, 0
		);
		am = matrices[ bIdIt->second ];

		worldMatrices[i] = am * itm;
	}

	unsigned lastI = i;
	while( i < worldMatrices.size() )
		Mat34_setIdentity( &worldMatrices[i++] );

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
		
		for( size_t w = 0; /*w < 3 && */w < skinInfo.weightsPerVertex; ++w )
		{
			unsigned char boneId = boneIndices[w];
			ASSERT(boneId < worldMatrices.size());
			ASSERT(boneId <= lastI);
			float boneWeight = weights[w];
			accumWeight += boneWeight;

			if(boneWeight < 0.001f)
				continue;

			Mat34& worldMatrix = worldMatrices[boneId];

			Vec3 v3;
			Vec3_setMat34MulVec3( &v3, &worldMatrix, &pos3 );
			Vec3_scale( &v3, &v3, boneWeight );
			Vec3_add( &accumP3, &accumP3, &v3 );

			Vec3 n3;
			Vec3_setMat33MulVec3( &n3, &worldMatrix.Rot, &nrm3 );
			Vec3_scale( &n3, &n3, boneWeight );
			Vec3_add( &accumN3, &accumN3, &n3 );
		}
		if(accumWeight < 0.95f && accumWeight > 1.05f)
			printf("bad weights\n");

		*dstPositions = accumP3;
		*dstNormals = accumN3;

		dstPositionsRaw += dstVertexStride;
		dstNormalsRaw += dstVertexStride;
		dstPositions = reinterpret_cast<Vec3*>(dstPositionsRaw);
		dstNormals = reinterpret_cast<Vec3*>(dstNormalsRaw);
	}
}
} // namespace mutalisk
