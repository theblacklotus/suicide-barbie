#include "LuaPlayer.h"

extern "C" {
	#include "lauxlib.h"
	#include "lualib.h" 
	#include "lua.h" 
}

#include <string>

using namespace mutalisk;
using namespace mutalisk::lua;

// ------------------------------------------------------------------
std::string mutalisk::lua::getError(::lua_State* L)
{
	int top = lua_gettop( L );
	if( lua_type( L, top ) == LUA_TSTRING )
		return lua_tostring( L, -1 );
	else
		return std::string( "Error message not available" );		
}

// ------------------------------------------------------------------
class CLuaPathTrack
{
public:
	static char const* funcName;

	static void reset( std::string const& str ) {
		mLuaCurrentPath = getDirPart( str );
	}

	static void registerFunc( lua_State* L )
	{
		lua_pushstring( L, funcName );
		lua_pushcclosure( L, &myDoFile, 0 );
		lua_settable( L, LUA_GLOBALSINDEX );
	}

	static std::string getDirPart( std::string const& str )
	{
		size_t n = str.find_last_of( "/\\" );
		if( n == std::string::npos )
			return "";
		else
			return str.substr( 0, n + 1 );
	}

	static std::string getNamePart( std::string const& str )
	{
		size_t n = str.find_last_of( "/\\" );
		if( n == std::string::npos )
			return str;
		else
			return str.substr( n + 1 );        
	}

	static std::string getNamePartWithoutExtension( std::string const& str )
	{
		size_t n = getNamePart( str ).find_last_of( "." );
		if( n == std::string::npos )
			return str;
		else
			return getNamePart( str ).substr( 0, n + 1 );
	}

	static std::string getExtensionPart( std::string const& str )
	{
		size_t n = str.find_last_of( "." );
		if( n == std::string::npos )
			return str;
		else
			return str.substr( n + 1 );        
	}

protected:
	static int myDoFile(lua_State *L)
	{
		const char *fname = luaL_optstring(L, 1, NULL);

		std::string prev = mLuaCurrentPath;
		std::string curName = mLuaCurrentPath + fname;

		mLuaCurrentPath += getDirPart( fname );

		int status = luaL_loadfile(L, curName.c_str() );

		if (status != 0)
			lua_error(L);

		lua_call(L, 0, LUA_MULTRET);
		int result = lua_gettop(L) - 1;

		mLuaCurrentPath = prev;

		return result;
	}

private:
	static std::string mLuaCurrentPath;
};

char const* CLuaPathTrack::funcName = "include";
std::string CLuaPathTrack::mLuaCurrentPath;

// ------------------------------------------------------------------

using namespace std;

static int myPanic(lua_State *L)
{
	return 0;
}

LuaPlayer::LuaPlayer()
{
	L = lua_open();
	luaopen_base( L );
	luaopen_string( L );
	luaopen_table( L );
	luaopen_math( L );
//	luaopen_io( L );
//	luaopen_debug( L );

	CLuaPathTrack::registerFunc( L );

	lua_atpanic( L, myPanic );
}

LuaPlayer::~LuaPlayer()
{
	lua_close( L );
}

void LuaPlayer::exec( std::string const& luaNamePath )
{
	std::string filePath;

	if( !luaNamePath.empty() && luaNamePath[ 0 ] != '!' )
	{
		CLuaPathTrack::reset( mPrefix + CLuaPathTrack::getDirPart( luaNamePath ) );
		filePath = mPrefix + luaNamePath;
	}
	else
	{
		CLuaPathTrack::reset( CLuaPathTrack::getDirPart( luaNamePath.substr(1) ) );
		filePath = luaNamePath.substr(1);
	}

//	LUAPLAYER_TRY
	try
	{
		int status = 0;
		//int errFuncHandler = luaErrorHandler();
		printf("moo\n");
		status = luaL_loadfile( L, filePath.c_str() );
		if( status != 0 )
		{
			THROW_LUAERROR( status, getError( L ) + ", while parsing " + luaNamePath + " [parsing]" );
		} else {
			//status = lua_pcall(L, 0, LUA_MULTRET, errFuncHandler);
			printf("moo1\n");
			status = lua_pcall(L, 0, LUA_MULTRET, 0);
			printf("moo2\n");
			if( status != 0 )
			{
				printf("moo3\n");
				THROW_LUAERROR( status, getError( L ) + ", while executing " + luaNamePath + " [executing]" );
			}
		}
	} 
	catch(...)
	{
		printf("error\n");
	}
	LUAPLAYER_CATCH;
}

void LuaPlayer::checkFunction( std::string const& funcName )
{
	int status = 0;
	lua_getglobal( L, funcName.c_str() );

	int top = lua_gettop( L );
	if( lua_isnil( L, top ) )
	{
		THROW_LUAERROR( status, std::string( "Function '" ) + funcName + "' not found" );
	}
}

int LuaPlayer::luaErrorHandler() const
{
	lua_pushstring( L, "debug" );
	lua_gettable( L, LUA_GLOBALSINDEX );
	lua_pushstring( L, "traceback" );
	lua_gettable( L, -2 );
	return lua_gettop( L );
}

void LuaPlayer::garbageCollect()
{
	lua_close( L );
	

	L = lua_open();
	luaopen_base( L );
	luaopen_string( L );
	luaopen_table( L );
	luaopen_math( L );
	luaopen_io( L );
	luaopen_debug( L );

	CLuaPathTrack::registerFunc( L );
}

void LuaPlayer::onError( std::string const& luaError )
{
	LuaPlayer::getInstance().mLastError = luaError;
}

LuaPlayer* LuaPlayer::createInstance() 
{
	// HACK: work-around for static initialization bug in Release
	static LuaPlayer* dualProtectedInstance = 0;
	if( !dualProtectedInstance )
		dualProtectedInstance = new LuaPlayer();
	return dualProtectedInstance;
}

void LuaPlayer::deleteInstance( LuaPlayer& c )
{
	delete &c;
}

LuaPlayer& LuaPlayer::getInstance()
{
	static LuaPlayer* luaPlayer = LuaPlayer::createInstance();
//	ASSERT(luaPlayer);
	return *luaPlayer;
}