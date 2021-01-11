#include "binary_compressed_output.h"
#if !defined(__psp__)
#include <iostream>
#endif

using namespace mutant;

template<typename _A>
void CHECK_ERR( int e, _A const& a )
{
#if !defined(__psp__)
	if(e!=Z_OK)
		std::cerr << "ZLIB ERROR: (" << e << ") " << (a) << std::endl;
#endif
}

mutant_compressed_output::mutant_compressed_output( std::auto_ptr<binary_output>& output )
:	mOutput( output )
{
	mBuffer = new unsigned char[BUF_LEN];
	initDeflate();
}

mutant_compressed_output::~mutant_compressed_output()
{
	flush();

	int err = deflateEnd(&zstream);
	CHECK_ERR(err, "deflateEnd");

	delete []mBuffer;
}

void mutant_compressed_output::write( void const* src, int len, int* wasWritten )
{
	int err = 0;

	zstream.next_in  = (Bytef*)src;
	zstream.avail_in = len;

	while( zstream.avail_in != 0 ) {
		if( zstream.avail_out == 0 ) {
			mOutput->write( mBuffer, BUF_LEN, 0 );

			zstream.next_out = mBuffer;
			zstream.avail_out = BUF_LEN;
		}
		err = deflate( &zstream, Z_NO_FLUSH );

		if( err == Z_STREAM_END )
			break;

		CHECK_ERR( err, "deflate" );
	}

	if( wasWritten )
		*wasWritten = len;
}

void mutant_compressed_output::initDeflate()
{
	int err;

	zstream.zalloc = (alloc_func)0;
	zstream.zfree = (free_func)0;
	zstream.opaque = (voidpf)0;

	err = deflateInit(&zstream, Z_DEFAULT_COMPRESSION);
	CHECK_ERR( err, "initDeflate" );

	zstream.next_out = mBuffer;
	zstream.avail_out = BUF_LEN;
}

void mutant_compressed_output::flush()
{
	int err = Z_OK;

	if( zstream.total_in != 0 ) {
		bool done = false;

		while(1) {
			int len = BUF_LEN - zstream.avail_out;
			if( len != 0 ) {
				mOutput->write( mBuffer, len, 0 );
				zstream.next_out = mBuffer;
				zstream.avail_out = BUF_LEN;
			}

			if( done )
				break;

			err = deflate( &zstream, Z_FINISH);

			if( len == 0 && err == Z_BUF_ERROR )
				err = Z_OK;

			if( err != Z_STREAM_END )
				CHECK_ERR(err, "deflate");

			done = (zstream.avail_out != 0 || err == Z_STREAM_END);

			if( err != Z_OK && err != Z_STREAM_END)
				break;
		}
	}
}
