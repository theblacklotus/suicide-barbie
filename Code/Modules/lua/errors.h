#ifndef MUTALISK_LUA___ERRORS_H
#define MUTALISK_LUA___ERRORS_H

#include "cfg.h"
#include <mutalisk/errors.h>

#define THROW_LUAERROR(result,msg)		THROW(ELuaError(result, msg, BASE_ERROR_PARAMS))

class ELuaError : public EBaseError {
public:
	ELuaError( int luaError, std::string msg, std::string fileName, int lineNumber )
	:	EBaseError( msg, fileName, lineNumber ),
		mLuaError( luaError ) {}

	int getLuaError() const { return mLuaError; }

private:
	int	const	mLuaError;
};

#endif // MUTALISK_LUA___ERRORS_H