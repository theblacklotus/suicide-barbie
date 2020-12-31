#include "binary_compressed_input.h"
#include <iostream>

using namespace mutant;

template<typename _A>
void CHECK_ERR( int e, _A const& a )  {
	if( e!=Z_OK )
		std::cerr << "ZLIB ERROR: (" << e << ") " << (a) << std::endl;
}

mutant_compressed_input::mutant_compressed_input( std::auto_ptr<binary_input>& input )
:	mInput( input )
{
	mBuffer = new unsigned char[BUF_SIZE];
	initInflate();
}

mutant_compressed_input::~mutant_compressed_input()
{
	flush();
	delete []mBuffer;
}

void mutant_compressed_input::read( void* dest, int len, int* wasRead )
{
	int err = Z_OK;

	zstream.avail_out = len;
	zstream.next_out = (Bytef*)dest;

	while( zstream.avail_out != 0 ) {
		if( zstream.avail_in == 0 ) {
			int was_read = 0;
			try
			{
				mInput->read( mBuffer, BUF_SIZE, &was_read );
				zstream.avail_in = was_read;
				zstream.next_in = mBuffer;
			} catch( EIoEof& ) {
				if( was_read != 0 ) {
					zstream.avail_in = was_read;
					zstream.next_in = mBuffer;
				} else
				{
/*_ __no_except					throw;*/
				}
			}
		}

		err = inflate( &zstream, Z_NO_FLUSH );

		if( err == Z_STREAM_END )
			break;

		CHECK_ERR(err, "inflate");
	}

	if( wasRead )
		*wasRead = len - zstream.avail_out;
}


void mutant_compressed_input::initInflate()
{
	int err = Z_OK;

	zstream.zalloc = (alloc_func)0;
	zstream.zfree = (free_func)0;
	zstream.opaque = (voidpf)0;

	zstream.next_in  = mBuffer;
	zstream.avail_in = 0;
	zstream.next_out = 0;

	err = inflateInit( &zstream );
	CHECK_ERR( err, "inflateInit" );
}

void mutant_compressed_input::flush()
{
	int err = Z_OK;

	err = inflateEnd( &zstream );
	CHECK_ERR( err, "inflateEnd" );
}

