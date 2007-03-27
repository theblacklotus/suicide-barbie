#include <mutant/mutant.h>

using namespace mutant;

namespace mutalisk { namespace data
{

// helper functions
// @TBD: move to utility file
inline template <typename In, typename T> In& operator>> (In& i, mutalisk::array<T>& c)
{
	c.resize(i.readDword());
	i.readArray(c.begin(), c.end());

}
inline template <typename Out, typename T> Out& operator<< (Out& o, mutalisk::array<T> const& c)
{
	o.writeDword(c.size());
	o.writeData(c.begin(), c.end());
}

// shader
//
inline template <typename In> In& operator>> (In& i, shader_fixed& data)
{
	try
	{
		unsigned versionCheck = (data.Version == i.readDword()); ASSERT(versionCheck);

		i.readType(data.ambient);
		i.readType(data.diffuse);
		i.readType(data.specular);
		i.readType(data.emissive);

		typedef unsigned TextureIndexT;
		i.readType(data.diffuseTexture);
		i.readType(data.envmapTexture);

		i.readType(data.uOffset);
		i.readType(data.vOffset);
		i.readType(data.transparency);

		i.readType(data.frameBufferOp);
		i.readType(data.zBufferOp);

		i.readType(data.aux0);
	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return i;
}

inline template <typename Out> Out& operator<< (Out& o, shader_fixed const& data)
{
	try
	{
		o.writeDword(data.Version);

		o.writeType(data.ambient);
		o.writeType(data.diffuse);
		o.writeType(data.specular);
		o.writeType(data.emissive);

		typedef unsigned TextureIndexT;
		o.writeType(data.diffuseTexture);
		o.writeType(data.envmapTexture);

		o.writeType(data.uOffset);
		o.writeType(data.vOffset);
		o.writeType(data.transparency);

		o.writeDword(data.frameBufferOp);
		o.writeDword(data.zBufferOp);

		o.writeType(data.aux0);
	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return o;
}


// shader
//
inline template <typename In> In& operator>> (In& i, shader& data)
{
	try
	{
		unsigned versionCheck = (data.Version == i.readDword()); ASSERT(versionCheck);

		// indices
		i >> data.indices.textures;
		i >> data.indices.matrices;
		i >> data.indices.vecs;
		i >> data.indices.floats;
		i >> data.indices.ints;

		// values
		i >> data.values.textures;
		i >> data.values.matrices;
		i >> data.values.vecs;
		i >> data.values.floats;
		i >> data.values.ints;
	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return i;
}

inline template <typename Out> Out& operator<< (Out& o, shader const& data)
{
	try
	{
		o.writeDword(data.Version);

		// indices
		o << data.indices.textures;
		o << data.indices.matrices;
		o << data.indices.vecs;
		o << data.indices.floats;
		o << data.indices.ints;

		// values
		o << data.values.textures;
		o << data.values.matrices;
		o << data.values.vecs;
		o << data.values.floats;
		o << data.values.ints;

	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return o;
}

} // namespace data 
} // namespace mutalisk
