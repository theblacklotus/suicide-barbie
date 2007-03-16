#ifndef MUTALIKS_LUA__PLAYER_H
#define MUTALIKS_LUA__PLAYER_H

#include "cfg.h"
#include <string>

#include "lua_include.h"

namespace mutalisk { namespace lua
{
	std::string getError(::lua_State* L);

#define LUAPLAYER_TRY
#define LUAPLAYER_CATCH																		\

/*#define LUAPLAYER_TRY try
#define LUAPLAYER_CATCH																		\
catch( luabind::cast_failed& e ) {															\
	THROW_LUAERROR( 0,  std::string( "Unable to make cast" ) +								\
		LuaPlayerErrorHelper::getInfoName( e ) + " [executing]"  );						\
} catch( luabind::error& e ) {																\
	THROW_LUAERROR( 0, LuaPlayerErrorHelper::getError(e.state() ) + " [executing]"  );		\
} int __dummy = 0;																			\
*/

class LuaPlayer
{
public:
	static LuaPlayer& getInstance();

public:
/*	template<class BIND_FUNCTOR>
	static void bind( BIND_FUNCTOR& f ) {
		assert_( LuaPlayer::getInstance().L );
		f( LuaPlayer::getInstance().L );
	}

	template<class T>
	void push( T& obj, std::string const& name ) {
		assert_( L );
		luabind::object globals = luabind::get_globals( L );
		globals[ name ] = obj;
	}

	template<class T>
	T pop( std::string const& name ) {
		assert_( L );
		luabind::object globals = luabind::get_globals( L );
		luabind::object luaObject = globals.at( name );
		if( !isNil( luaObject ) )
			return luabind::object_cast<T>( luaObject );
		else
			return T();
	}

	template<class T>
	T pop( int index = -1 ) {
		assert_( L );
		luabind::object luaObject = luabind::object( L );
		luaObject.set();
		return luabind::object_cast<T>( luaObject );
	}*/

	void setPrefix( std::string const& prefix = "./" ) {
		mPrefix = prefix;
	}

	void exec( std::string const& luaName );

	/*
	void call( std::string const& funcName ) {
		checkFunction( funcName );
		LUAPLAYER_TRY {luabind::call_functionEx<void>( L, funcName.c_str(), luaErrorHandler() );} LUAPLAYER_CATCH;
	}
	template<typename A0>
	void call( std::string const& funcName, A0 a0 ) {
		checkFunction( funcName );
		LUAPLAYER_TRY {luabind::call_functionEx<void>( L, funcName.c_str(), luaErrorHandler(), a0 );} LUAPLAYER_CATCH;
	}
	template<typename A0, typename A1 >
	void call( std::string const& funcName, A0 a0, A1 a1 ) {
		checkFunction( funcName );
		LUAPLAYER_TRY {luabind::call_functionEx<void>( L, funcName.c_str(), luaErrorHandler(), a0, a1 );} LUAPLAYER_CATCH;
	}
	template<typename A0, typename A1, typename A2>
	void call( std::string const& funcName, A0 a0, A1 a1, A2 a2 ) {
		checkFunction( funcName );
		LUAPLAYER_TRY {luabind::call_functionEx<void>( L, funcName.c_str(), luaErrorHandler(), a0, a1, a2 );} LUAPLAYER_CATCH;
	}
	template<typename A0, typename A1, typename A2, typename A3>
	void call( std::string const& funcName, A0 a0, A1 a1, A2 a2, A3 a3 ) {
		checkFunction( funcName );
		LUAPLAYER_TRY {luabind::call_functionEx<void>( L, funcName.c_str(), luaErrorHandler(), a0, a1, a2, a3 );} LUAPLAYER_CATCH;
	}
	template<typename A0, typename A1, typename A2, typename A3, typename A4>
	void call( std::string const& funcName, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4 ) {
		checkFunction( funcName );
		LUAPLAYER_TRY {luabind::call_functionEx<void>( L, funcName.c_str(), luaErrorHandler(), a0, a1, a2, a3, a4 );} LUAPLAYER_CATCH;
	}*/

	void garbageCollect();

	/*
	bool isNil( luabind::object const& luaObject ) {
		return( luaObject.type() == LUA_TNIL );
	}
	bool isTable( luabind::object const& luaObject ) {
		return( luaObject.type() == LUA_TTABLE );
	}
	bool isFunction( luabind::object const& luaObject ) {
		return( luaObject.type() == LUA_TFUNCTION );
	}*/

	lua_State* luaState() const { return L; }
	int luaErrorHandler() const;

public:
	static void onError( std::string const& luaError );

protected:
	LuaPlayer();
	~LuaPlayer();

	void checkFunction( std::string const& funcName );

private:
//	friend CustomLifetimePolicy<LuaPlayer>;
	static LuaPlayer* createInstance();
	static void deleteInstance( LuaPlayer& c );

private:
	lua_State*	L;
	std::string	mLastError;
	std::string mPrefix;
};
} // namespace lua
} // namespace mutalisk


#endif // MUTALIKS_LUA__PLAYER_H