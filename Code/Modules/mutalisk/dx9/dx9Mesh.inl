#include <mutant/mutant.h>

using namespace mutant;

namespace mutalisk { namespace data
{

// dx9_mesh
//
inline template <typename In> In& operator>> (In& i, dx9_mesh& mesh)
{
	clear(mesh);

	try
	{
		i >> mesh.base();
		mesh.fvfVertexDecl = i.readDword();
		mesh.primitiveType = static_cast<D3DPRIMITIVETYPE>(i.readDword());
		if( i.readBool() )
		{
			mesh.skinInfo = new skin_info;
			i >> *mesh.skinInfo;
		}
		else
			mesh.skinInfo = 0;

	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return i;
}

inline template <typename Out> Out& operator<< (Out& o, dx9_mesh& mesh)
{
	try
	{
		o << mesh.base();
		o.writeDword( mesh.fvfVertexDecl );
		o.writeDword( mesh.primitiveType );

		o.writeBool( (mesh.skinInfo != 0) );
		if( mesh.skinInfo )
			o << *mesh.skinInfo;

	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return o;
}

} // namespace data 
} // namespace mutalisk