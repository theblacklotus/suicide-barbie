#include "mesh.h"

namespace mutalisk { namespace data
{

// base_mesh
//
base_mesh::base_mesh()
: vertexCount(0)
, vertexStride(0)
, vertexDataSize(0)
, vertexData(0)
, indexCount(0)
, indexSize(0)
, indexData(0)
{
}

base_mesh::~base_mesh()
{ 
	clear(*this);
}

void clear(base_mesh& data)
{
	delete[] data.vertexData;
	delete[] data.indexData;
}

// skin_info
//
skin_info::skin_info()
: weightsPerVertex(0)
, boneCount(0)
, bones(0)
{
}

skin_info::~skin_info()
{
	clear(*this);
}

void clear(skin_info& data)
{
	delete[] data.bones;
}

} // namespace data 
} // namespace mutalisk
