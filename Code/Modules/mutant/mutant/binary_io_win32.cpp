#if defined WIN32
#include "binary_io_win32.h"
#include "types_ios.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace mutant
{
	file_input::file_input( std::string const& name )
	:	mFile( NULL )
	{
		mFile = CreateFile(
			name.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,
			NULL );

		if( mFile == INVALID_HANDLE_VALUE )
			throw EIoError( IO_NOFILE, "Failed to open `" + name + "' for reading" );
	}

/*	file_input::file_input( std::wstring const& name )
	:	mFile( NULL )
	{
		mFile = CreateFileW(
			name.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,
			NULL );

		if( mFile == INVALID_HANDLE_VALUE )
			throw EIoError( IO_NOFILE, "Failed to open `???' for reading" );
	}
*/
	file_input::~file_input() {
		if( mFile ) {
			CloseHandle( (HANDLE)mFile );
		}
	}

	void file_input::read( void* dest, int n, int* wasRead )
	{
		DWORD rd = 0;

		bool good = ReadFile( (HANDLE)mFile, dest, n, &rd, NULL ) != 0;

		if( wasRead )
			*wasRead = rd;

		int err = GetLastError();

		if( good && rd == 0 ) {
			throw EIoEof();
		}

		if( !good ) {
			throw EIoError( IO_ERROR, std::string("Failed to read `") + n + "' bytes from file" );
		}
	}

	file_output::file_output( std::string const& name ) {
		mFile = CreateFile(
			name.c_str(),
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL );

		if( mFile == INVALID_HANDLE_VALUE )
			throw EIoError( IO_NOFILE, "Failed to open `" + name + "' for writing" );
	}

/*	file_output::file_output( std::wstring const& name ) {
		mFile = CreateFileW(
			name.c_str(),
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL );

		if( mFile == INVALID_HANDLE_VALUE )
			throw EIoError( IO_NOFILE, "Failed to open `???' for writing" );
	}
*/
	file_output::~file_output() {
		if( mFile ) {
			CloseHandle( (HANDLE)mFile );
		}
	}

	void file_output::write( void const* src, int n, int* wasWritten ) {
		DWORD wr = 0;

		bool good = WriteFile( (HANDLE)mFile, src, n, &wr, NULL ) != 0;

		if( wasWritten  )
			*wasWritten = wr;

		int err = GetLastError();
		if( !good ) {
			throw EIoError( IO_ERROR, std::string("Failed to write `") + n + "' bytes to file" );
		}
	}

}

#endif
