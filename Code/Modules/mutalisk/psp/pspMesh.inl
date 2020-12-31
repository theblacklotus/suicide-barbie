#include <mutant/mutant.h>

using namespace mutant;

namespace mutalisk { namespace data
{

// psp_mesh
//
inline template <typename In> In& operator>> (In& i, psp_mesh& data)
{
	clear(data);

	try
	{
		unsigned versionCheck = (data.Version == i.readDword()); ASSERT(versionCheck);

		// base_mesh
		i >> data.base();

		// psp_mesh
		data.vertexDecl = i.readDword();
		data.primitiveType = i.readDword();
		if( i.readBool() )
		{
			data.skinInfo = new skin_info;
			i >> *data.skinInfo;

			data.weightStride = i.readDword();
			data.weightDataSize = i.readDword();
			data.weightData = new unsigned char[data.weightDataSize];
			i.readArray(data.weightData, data.weightDataSize);

			data.boneIndexStride = i.readDword();
			data.boneIndexDataSize = i.readDword();
			data.boneIndexData = new unsigned char[data.boneIndexDataSize];
			i.readArray(data.boneIndexData, data.boneIndexDataSize);
		}
		else
		{	
			data.skinInfo = 0;

			data.boneIndexStride = 0;
			data.boneIndexDataSize = 0;
			data.boneIndexData = 0;
		}
	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return i;
}

inline template <typename Out> Out& operator<< (Out& o, psp_mesh& data)
{
	try
	{
		o.writeDword(data.Version);

		// base_mesh
		o << data.base();

		// psp_mesh
		o.writeDword(data.vertexDecl);
		o.writeDword(data.primitiveType);

		o.writeBool((data.skinInfo != 0));
		if( data.skinInfo )
		{
			o << *data.skinInfo;

			o.writeDword(data.weightStride);
			o.writeDword(data.weightDataSize);
			o.writeData (data.weightData, data.weightDataSize);

			o.writeDword(data.boneIndexStride);
			o.writeDword(data.boneIndexDataSize);
			o.writeData (data.boneIndexData, data.boneIndexDataSize);
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
