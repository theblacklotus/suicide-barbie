#ifndef __ERRORS_H
#define __ERRORS_H

#include "cfg.h"

#include <string>
#include <stdexcept>

#define BASE_ERROR_PARAMS	__FILE__, __LINE__
#ifdef __ERROR_C1055_WORKAROUND
	#undef BASE_ERROR_PARAMS
	#define BASE_ERROR_PARAMS	__FILE__, -1
#endif

#if defined WIN32
	#define THROW_ERROR(msg)				throw EBaseError( msg, BASE_ERROR_PARAMS )
	#define THROW_DXERROR(result,msg)		throw EDXError( result, msg, BASE_ERROR_PARAMS )
#elif defined __psp__
	#define THROW_ERROR(msg) ()
	#define THROW_DXERROR(result,msg) ()
#endif


class EBaseError : public std::runtime_error {
public:
	EBaseError( std::string msg, std::string fileName, int lineNumber )
	:	std::runtime_error( msg ),
		mFileName( fileName ),
		mLineNumber( lineNumber )
	{	
		mWhereMsg = "file: " + mFileName;
	};

	// mimick the runtime_error style
	virtual const char* where() const {	return mWhereMsg.c_str(); }

	std::string const& getFileName() const { return mFileName; }
	int getLineNumber() const { return mLineNumber; }

private:
	std::string		mWhereMsg;
	std::string 	mFileName;
	int				mLineNumber;
};

#if defined WIN32
	#include <d3dx9.h>
	class EDXError : public EBaseError {
	public:
		EDXError( HRESULT dxresult, std::string msg, std::string fileName, int lineNumber )
		:	EBaseError( msg, fileName, lineNumber ),
			mDXResult( dxresult )
		{
		};

		HRESULT const& getDXResult() const { return mDXResult; };

	private:
		HRESULT			mDXResult;
	};
#elif defined __psp__
#endif


#endif // __ERRORS_H
