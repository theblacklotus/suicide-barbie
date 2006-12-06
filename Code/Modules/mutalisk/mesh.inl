#include <mutant/mutant.h>

using namespace mutant;

namespace mutalisk { namespace data
{

// base_mesh
//
inline template <typename In> In& operator>> (In& i, base_mesh& mesh)
{
	clear(mesh);

	try
	{
		mesh.vertexCount = i.readDword();
		mesh.vertexDataSize = i.readDword();
		mesh.vertexStride = i.readDword();
		mesh.vertexData = new unsigned char[mesh.vertexDataSize];
		i.readArray( mesh.vertexData, mesh.vertexDataSize );

		mesh.indexCount = i.readDword();
		mesh.indexSize = i.readDword();
		mesh.indexData = new unsigned char[mesh.indexCount * mesh.indexSize];
		i.readArray( mesh.indexData, mesh.indexCount * mesh.indexSize );

	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return i;
}

inline template <typename Out> Out& operator<< (Out& o, base_mesh const& mesh)
{
	try
	{
		o.writeDword( mesh.vertexCount );
		o.writeDword( mesh.vertexDataSize );
		o.writeDword( mesh.vertexStride );
		o.writeData ( mesh.vertexData, mesh.vertexDataSize );

		o.writeDword( mesh.indexCount );
		o.writeDword( mesh.indexSize );
		o.writeData ( mesh.indexData, mesh.indexCount * mesh.indexSize );

	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return o;
}

// skin_info
//
inline template <typename In> In& operator>> (In& i, skin_info& skin)
{
	clear(skin);

	try
	{
		skin.weightsPerVertex = i.readDword();
		skin.boneCount = i.readDword();
		skin.bones = new data::skin_info::Bone[skin.boneCount];
		for( size_t q = 0; q < skin.boneCount; ++q )
		{
			i.readType( skin.bones[q].matrix );
			i.readString( skin.bones[q].name );
		}
	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return i;
}

inline template <typename Out> Out& operator<< (Out& o, skin_info const& skin)
{
	try
	{
		o.writeDword( skin.weightsPerVertex );
		o.writeDword( skin.boneCount );
		for( size_t q = 0; q < skin.boneCount; ++q )
		{
			o.writeType( skin.bones[q].matrix );
			o.writeString( skin.bones[q].name );
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
