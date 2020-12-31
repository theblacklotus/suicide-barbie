#ifndef MUTANT_BINARY_IO_H_
#define MUTANT_BINARY_IO_H_

#include <stdexcept>

namespace mutant
{
	enum eIoError
	{
		IO_ERROR,
		IO_NOFILE
	};

	class EIoError : public std::runtime_error {
	public:
		EIoError( eIoError error_code, std::string const& w = "Unspecified Mutant I/O error" ) : std::runtime_error( w ), mErrorCode( error_code ) {}
		eIoError errorCode() const { return mErrorCode; }
	private:
		eIoError	mErrorCode;
	};

	class EIoEof : public std::runtime_error {
	public:
		EIoEof() : std::runtime_error( "Unexpected end of file" ) {
		}
	};

	class binary_input
	{
	public:
		virtual ~binary_input() /*_ __gcc = 0*/ {}
		virtual void read( void* dest, int n, int* wasRead ) = 0; // throws EIoError
	};

	class binary_output
	{
	public:
		virtual ~binary_output() /*_ __gcc = 0*/ {}
		virtual void write( void const* src, int n, int* wasWritten ) = 0;
	};
}

#endif // MUTANT_BINARY_IO_H_
