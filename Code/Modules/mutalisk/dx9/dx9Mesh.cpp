#include "dx9Mesh.h"

namespace mutalisk { namespace data
{

// dx9_mesh
//
dx9_mesh::dx9_mesh()
: fvfVertexDecl(0)
, primitiveType(D3DPT_TRIANGLELIST)
, skinInfo(0)
{
}

dx9_mesh::~dx9_mesh()
{ 
	clear(*this);
}

void clear(dx9_mesh& data)
{
	delete data.skinInfo;
}

} // namespace data 
} // namespace mutalisk
