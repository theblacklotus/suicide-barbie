#include "binary_io_psp.h"
//#include "types_ios.h"

#include <pspiofilemgr.h>

namespace mutant
{
	file_input::file_input( std::string const& name )
	:	mFile( 0 )
	{
		if( (mFile = sceIoOpen( name.c_str(), PSP_O_RDONLY, 0777 )) > 0 )
		{
			sceIoLseek(mFile, 0, PSP_SEEK_SET);
		}
	}

	file_input::~file_input() {
		if( mFile )
			sceIoClose( mFile );
	}

	void file_input::read( void* dest, int n, int* wasRead ) {
		int bytesRead = 0;
		if( mFile )
			bytesRead = sceIoRead( mFile, dest, n );
		if( wasRead )
			*wasRead = bytesRead;
	}

	file_output::file_output( std::string const& name ) {
	}

	file_output::~file_output() {
	}

	void file_output::write( void const* src, int n, int* wasWritten ) {
		*wasWritten = 0;
	}

}


