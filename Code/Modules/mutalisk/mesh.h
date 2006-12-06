#ifndef MUTALISK_DATA_MESH_H_
#define MUTALISK_DATA_MESH_H_

#include "common.h"
#include <string>

namespace mutalisk { namespace data
{

	struct base_mesh
	{
		unsigned int vertexCount;
		unsigned int vertexStride;
		unsigned int vertexDataSize;
		byte* vertexData;

		unsigned int indexCount;
		unsigned int indexSize;
		byte* indexData;

		// memory management
		base_mesh(); ~base_mesh();
	};

	struct skin_info
	{
		struct Bone
		{
			Mat16 matrix;
			std::string	name;
		};

		unsigned int weightsPerVertex;
		unsigned int boneCount;
		Bone* bones;

		// memory management
		skin_info(); ~skin_info();
	};

	// I/O
	template <typename In> In& operator>> (In& i, base_mesh& mesh);
	template <typename Out> Out& operator<< (Out& o, base_mesh const& mesh);

	template <typename In> In& operator>> (In& i, skin_info& skin);
	template <typename Out> Out& operator<< (Out& o, skin_info const& skin);

	// memory management
	void clear(base_mesh& mesh);
	void clear(skin_info& skin);

} // namespace data 
} // namespace mutalisk

#include "mesh.inl"

#endif // MUTALISK_DATA_MESH_H_
