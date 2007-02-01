#ifndef MUTALISK_DATA_DX9_MESH_H_
#define MUTALISK_DATA_DX9_MESH_H_

#include "../common.h"
#include "../mesh.h"
#include <d3d9.h>

namespace mutalisk { namespace data
{

	struct dx9_mesh : public parent<base_mesh>
	{
		enum { Version = 0x0102 };

		unsigned int fvfVertexDecl;
		D3DPRIMITIVETYPE primitiveType;
		skin_info* skinInfo;	// $HACK

		// memory management
		dx9_mesh(); ~dx9_mesh();
	};

	// I/O
	template <typename In> In& operator>> (In& i, dx9_mesh& data);
	template <typename Out> Out& operator<< (Out& o, dx9_mesh const& data);

	// memory management
	void clear(dx9_mesh& data);

} // namespace data 
} // namespace mutalisk

#include "dx9Mesh.inl"

#endif // MUTALISK_DATA_DX9_MESH_H_
