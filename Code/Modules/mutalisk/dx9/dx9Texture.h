#ifndef MUTALISK_DX9_TEXTURE_H_
#define MUTALISK_DX9_TEXTURE_H_

namespace mutalisk { namespace data
{
	struct dx9_texture
	{
	};

inline template <typename In> In& operator>> (In& i, dx9_texture& texture)
{
	try
	{
	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return i;
}

} // namespace data 
} // namespace mutalisk

#endif // MUTALISK_DX9_TEXTURE_H_
