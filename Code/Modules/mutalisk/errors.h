#ifndef MUTALISK_ERRORS_H_
#define MUTALISK_ERRORS_H_

#include "cfg.h"
#include <string>
#include "platform.h"

#if defined USE_EXCEPTIONS
	#include <stdexcept>
#endif

#define BASE_ERROR_PARAMS	__FILE__, __LINE__
#if defined __ERROR_C1055_WORKAROUND
	#undef BASE_ERROR_PARAMS
	#define BASE_ERROR_PARAMS	__FILE__, -1
#endif

#if defined USE_EXCEPTIONS
	#define THROW(e) throw e
	#define TRY try
	#define CATCH(e) catch(e)
#else
	#define THROW(e) {}
	#define TRY {}
	#define CATCH(e) {} if(0)
#endif

#define THROW_ERROR(msg)				THROW(EBaseError(msg, BASE_ERROR_PARAMS))
#if defined MUTALISK_DX9
#define THROW_DXERROR(result,msg)		THROW(EDXError(result, msg, BASE_ERROR_PARAMS))
#elif defined MUTALISK_PSP
#endif

class EBaseError 
#if defined USE_EXCEPTIONS
	: public std::runtime_error 
#endif
{
public:
	EBaseError( std::string msg, std::string fileName, int lineNumber ) :
#if defined USE_EXCEPTIONS
		std::runtime_error( msg ),
#endif
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

#if defined MUTALISK_DX9
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
#elif defined MUTALISK_PSP
#endif

#endif // MUTALISK_ERRORS_H_
