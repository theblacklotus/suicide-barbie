#include <mutant/mutant.h>

using namespace mutant;

namespace mutalisk { namespace data
{

// dx9_mesh
//
inline template <typename In> In& operator>> (In& i, dx9_mesh& data)
{
	clear(data);

	try
	{
		unsigned versionCheck = (data.Version == i.readDword()); ASSERT(versionCheck);

		// base_mesh
		i >> data.base();

		// dx9_mesh
		data.fvfVertexDecl = i.readDword();
		data.primitiveType = static_cast<D3DPRIMITIVETYPE>(i.readDword());
		if( i.readBool() )
		{
			data.skinInfo = new skin_info;
			i >> *data.skinInfo;
		}
		else
			data.skinInfo = 0;

	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return i;
}

inline template <typename Out> Out& operator<< (Out& o, dx9_mesh& data)
{
	try
	{
		o.writeDword(mesh.Version);

		// base_mesh
		o << data.base();
		
		// dx9_mesh
		o.writeDword( data.fvfVertexDecl );
		o.writeDword( data.primitiveType );

		o.writeBool( (data.skinInfo != 0) );
		if( data.skinInfo )
			o << *data.skinInfo;

	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return o;
}

} // namespace data 
} // namespace mutalisk