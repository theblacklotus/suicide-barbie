#ifndef MUTANT_BINARY_IO_WIN32_H_
#define MUTANT_BINARY_IO_WIN32_H_

#include "cfg.h"

#include <string>
#include "binary_io.h"

namespace mutant
{
	class file_input : public binary_input
	{
	public:
		file_input( std::string const& name );
//		file_input( std::wstring const& name );
		~file_input();
		virtual void read( void* dest, int n, int* wasRead );

	private:
		void*	mFile;
	};

	class file_output : public binary_output
	{
	public:
		file_output( std::string const& name );
//		file_output( std::wstring const& name );
		~file_output();
		virtual void write( void const* src, int n, int* wasWritten );

	private:
		void*	mFile;
	};
}

#endif // MUTANT_BINARY_IO_WIN32_H_
