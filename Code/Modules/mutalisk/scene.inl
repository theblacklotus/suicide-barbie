#include <mutant/mutant.h>

using namespace mutant;

namespace mutalisk { namespace data
{

// scene::Node
//
inline template <typename In> In& operator>> (In& i, scene::Node& data)
{
	try
	{
		i.readString( data.nodeName );
		i.readType( data.worldMatrix );
	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return i;
}

inline template <typename Out> Out& operator<< (Out& o, scene::Node const& data)
{
	try
	{
		o.writeString(data.nodeName);
		o.writeType(data.worldMatrix);
	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return o;
}

// scene
//
inline template <typename In> In& operator>> (In& i, scene& data)
{
	try
	{
		ASSERT(data.Version == i.readDword());

		// meshes
		data.meshIds.resize(i.readDword());
		i.readArray(data.meshIds.begin(), data.meshIds.end());

		// textures
		data.textureIds.resize(i.readDword());
		i.readArray(data.textureIds.begin(), data.textureIds.end());

		// shaders
		data.shaderIds.resize(i.readDword());
		i.readArray(data.shaderIds.begin(), data.shaderIds.end());

		// lights
		data.lights.resize(i.readDword());
		for(size_t q = 0; q < data.lights.size(); ++q)
		{
//			i.readString( data.lights[q].nodeName );
//			i.readType( data.lights[q].worldMatrix );
			i >> data.lights[q].base();
		}

		// cameras
		data.cameras.resize(i.readDword());
		for(size_t q = 0; q < data.cameras.size(); ++q)
		{
//			i.readString( data.cameras[q].nodeName );
//			i.readType( data.cameras[q].worldMatrix );
			i >> data.cameras[q].base();

		}
		data.defaultCameraIndex = i.readDword();

		// actors
		data.actors.resize(i.readDword());
		for(size_t q = 0; q < data.actors.size(); ++q)
		{
//			i.readString( data.actors[q].nodeName );
//			i.readType( data.actors[q].worldMatrix );
			i >> data.actors[q].base();
			data.actors[q].meshIndex = i.readDword();
			data.actors[q].materials.resize(i.readDword());
			for(size_t w = 0; w < data.actors[q].materials.size(); ++w)
			{
				data.actors[q].materials[w].textureIndex = i.readDword();
				data.actors[q].materials[w].shaderIndex = i.readDword();
			}
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
		o.writeDword(data.Version);

		// meshes
		o.writeDword(data.meshIds.size());
		o.writeData(data.meshIds.begin(), data.meshIds.end());

		// textures
		o.writeDword(data.textureIds.size());
		o.writeData(data.textureIds.begin(), data.textureIds.end());
		
		// shaders
		o.writeDword(data.shaderIds.size());
		o.writeData(data.shaderIds.begin(), data.shaderIds.end());

		// lights
		o.writeDword(data.lights.size());
		for( size_t q = 0; q < data.lights.size(); ++q )
		{
//			o.writeString(data.lights[q].nodeName);
//			o.writeType(data.lights[q].worldMatrix);
			o << data.lights[q].base();
		}

		// cameras
		o.writeDword(data.cameras.size());
		for(size_t q = 0; q < data.cameras.size(); ++q)
		{
//			o.writeString(data.cameras[q].nodeName);
//			o.writeType(data.cameras[q].worldMatrix);
			o << data.cameras[q].base();
		}
		o.writeDword(data.defaultCameraIndex);

		// actors
		o.writeDword(data.actors.size());
		for(size_t q = 0; q < data.actors.size(); ++q)
		{
//			o.writeString(data.actors[q].nodeName);
//			o.writeType(data.actors[q].worldMatrix);
			o << data.actors[q].base();
			o.writeDword(data.actors[q].meshIndex);
			o.writeDword(data.actors[q].materials.size());
			for(size_t w = 0; w < data.actors[q].materials.size(); ++w)
			{
				o.writeDword(data.actors[q].materials[w].textureIndex);
				o.writeDword(data.actors[q].materials[w].shaderIndex);
			}
		}

		// clips
		o.writeString(data.animCharId);
		o.writeDword(data.defaultClipIndex);

	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return o;
}

} // namespace data 
} // namespace mutalisk
