#include "pspMesh.h"

#include "pspXcompile.h"

namespace mutalisk { namespace data
{

// psp_mesh
//
psp_mesh::psp_mesh()
: vertexDecl(0)
, primitiveType(GU_TRIANGLES)
, skinInfo(0)
, boneIndexStride(0)
, boneIndexDataSize(0)
, boneIndexData(0)
, sprite(false)
{
}

psp_mesh::~psp_mesh()
{ 
	clear(*this);
}

void clear(psp_mesh& data)
{
	delete data.skinInfo;
	delete[] data.boneIndexData;
}

} // namespace data 
} // namespace mutalisk
