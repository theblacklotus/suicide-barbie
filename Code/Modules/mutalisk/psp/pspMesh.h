#ifndef MUTALISK_PSP_MESH_H_
#define MUTALISK_PSP_MESH_H_

#include "../common.h"
#include "../mesh.h"

namespace mutalisk { namespace data
{
	struct psp_mesh : public parent<base_mesh>
	{
		unsigned int vertexDecl;
		unsigned int primitiveType;
		
		skin_info* skinInfo;			// $HACK
		unsigned int boneIndexStride;
		unsigned int boneIndexDataSize;
		byte* boneIndexData;

		// memory management
		psp_mesh(); ~psp_mesh();
	};
	
	// I/O
	template <typename In> In& operator>> (In& i, psp_mesh& data);
	template <typename Out> Out& operator<< (Out& o, psp_mesh const& data);

	// memory management
	void clear(psp_mesh& data);

} // namespace data
} // namespace mutalisk

#include "pspMesh.inl"

#endif // MUTALISK_PSP_MESH_H_
