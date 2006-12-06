#include <mutant/mutant.h>

using namespace mutant;

namespace mutalisk { namespace data
{

// scene
//
inline template <typename In> In& operator>> (In& i, scene& data)
{
	try
	{
		// meshes
		data.meshCount = i.readDword();
		data.meshIds = new scene::Ref[data.meshCount];
		i.readArray( data.meshIds, (int)data.meshCount );

		// lights
		data.lightCount = i.readDword();
		data.lights = new scene::Light[data.lightCount];
		for( size_t q = 0; q < data.lightCount; ++q )
		{
			i.readString( data.lights[q].nodeName );
			i.readType( data.lights[q].worldMatrix );
		}

		// cameras
		data.cameraCount = i.readDword();
		data.cameras = new scene::Camera[data.cameraCount];
		for( size_t q = 0; q < data.cameraCount; ++q )
		{
			i.readString( data.cameras[q].nodeName );
			i.readType( data.cameras[q].worldMatrix );
		}
		data.defaultCameraIndex = i.readDword();

		// actors
		data.actorCount = i.readDword();
		data.actors = new scene::Actor[data.actorCount];
		for( size_t q = 0; q < data.actorCount; ++q )
		{
			i.readString( data.actors[q].nodeName );
			i.readType( data.actors[q].worldMatrix );
			data.actors[q].meshIndex = i.readDword();
		}

		// clips
		i.readString( data.animCharId );
		data.defaultClipIndex = i.readDword();

	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return i;
}

inline template <typename Out> Out& operator<< (Out& o, scene const& data)
{
	try
	{
		// meshes
		o.writeDword( data.meshCount );
		o.writeData( data.meshIds, data.meshCount );

		// lights
		o.writeDword( data.lightCount );
		for( size_t q = 0; q < data.lightCount; ++q )
		{
			o.writeString( data.lights[q].nodeName );
			o.writeType( data.lights[q].worldMatrix );
		}

		// cameras
		o.writeDword( data.cameraCount );
		for( size_t q = 0; q < data.cameraCount; ++q )
		{
			o.writeString( data.cameras[q].nodeName );
			o.writeType( data.cameras[q].worldMatrix );
		}
		o.writeDword( data.defaultCameraIndex );

		// actors
		o.writeDword( data.actorCount );
		for( size_t q = 0; q < data.actorCount; ++q )
		{
			o.writeString( data.actors[q].nodeName );
			o.writeType( data.actors[q].worldMatrix );
			o.writeDword( data.actors[q].meshIndex );
		}

		// clips
		o.writeString( data.animCharId );
		o.writeDword( data.defaultClipIndex );

	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return o;
}

} // namespace data 
} // namespace mutalisk
