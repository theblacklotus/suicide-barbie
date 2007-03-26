#include <mutant/mutant.h>

using namespace mutant;

namespace mutalisk { namespace data
{

// psp_mesh
//
inline template <typename In> In& operator>> (In& i, psp_mesh& mesh)
{
	clear(mesh);

	try
	{
		assert(mesh.Version == i.readDword());

		// base_mesh
		i >> mesh.base();

		// psp_mesh
		mesh.vertexDecl = i.readDword();
		mesh.primitiveType = i.readDword();
		if( i.readBool() )
		{
			mesh.skinInfo = new skin_info;
			i >> *mesh.skinInfo;

			mesh.weightStride = i.readDword();
			mesh.weightDataSize = i.readDword();
			mesh.weightData = new unsigned char[mesh.weightDataSize];
			i.readArray(mesh.weightData, mesh.weightDataSize);

			mesh.boneIndexStride = i.readDword();
			mesh.boneIndexDataSize = i.readDword();
			mesh.boneIndexData = new unsigned char[mesh.boneIndexDataSize];
			i.readArray(mesh.boneIndexData, mesh.boneIndexDataSize);
		}
		else
		{	
			mesh.skinInfo = 0;

			mesh.boneIndexStride = 0;
			mesh.boneIndexDataSize = 0;
			mesh.boneIndexData = 0;
		}
	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return i;
}

inline template <typename Out> Out& operator<< (Out& o, psp_mesh& mesh)
{
	try
	{
		o.writeDword(mesh.Version);

		// base_mesh
		o << mesh.base();

		// psp_mesh
		o.writeDword(mesh.vertexDecl);
		o.writeDword(mesh.primitiveType);

		o.writeBool((mesh.skinInfo != 0));
		if( mesh.skinInfo )
		{
			o << *mesh.skinInfo;

			o.writeDword(mesh.weightStride);
			o.writeDword(mesh.weightDataSize);
			o.writeData (mesh.weightData, mesh.weightDataSize);

			o.writeDword(mesh.boneIndexStride);
			o.writeDword(mesh.boneIndexDataSize);
			o.writeData (mesh.boneIndexData, mesh.boneIndexDataSize);
		}

	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return o;
}

} // namespace data 
} // namespace mutalisk
