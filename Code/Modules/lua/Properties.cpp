#include "Properties.h"

extern "C" {
	#include "lauxlib.h"
	#include "lualib.h" 
}

#include <sstream>
#include "LuaPlayer.h"
#include "Errors.h"

using namespace mutalisk;
using namespace mutalisk::lua;

namespace {
	enum
	{
		ResultLuaIndex = -1,
		KeyLuaIndex = -2,
		ValueLuaIndex = -1
	};

bool readFromLuaStack(lua_State* L, int luaIndex, std::string& name)
{
	if(lua_isstring(L, luaIndex))
	{
		name = lua_tostring(L, luaIndex);
		return true;
	}
	else if(lua_isnumber(L, luaIndex))
	{
		std::stringstream ss(name);
		ss << lua_tonumber(L, luaIndex);
		return true;
	}
	return false;
}

void readFromLuaStack(lua_State* L, int luaIndex, Properties& prop)
{
	std::string name;
	// table is in the stack at index 'luaIndex'
	while(lua_next(L, luaIndex) != 0)
	{
		// 'key' is at index -2 and 'value' at index -1
		if(!readFromLuaStack(L, KeyLuaIndex, name))
		{
			THROW_ERROR("key expects [string|number]");
		}

		if(lua_istable(L, ValueLuaIndex))
		{
			// @TBD: vector
			// prop.vectors[name] = value;
		}
		else if(lua_isnumber(L, ValueLuaIndex))
		{
			lua_Number value(lua_tonumber(L, ValueLuaIndex));
			prop.numbers[name] = value;
		}
		else if(lua_isstring(L, ValueLuaIndex))
		{
			std::string value(lua_tostring(L, ValueLuaIndex));
			prop.strings[name] = value;
		}
		else
		{
			// value expects [table|string|number]
			THROW_ERROR("value expects [table|string|number]");
		}

		lua_pop(L, 1);  // removes 'value'; keeps 'key' for next iteration
     }
}

void readFromLuaStack(lua_State* L, int luaIndex, PropertiesByNameT& props)
{
	std::string name;
	// table is in the stack at index 'luaIndex'
	while(lua_next(L, luaIndex) != 0)
	{
		// 'key' is at index -2 and 'value' at index -1
		if(!readFromLuaStack(L, KeyLuaIndex, name))
		{
			THROW_ERROR("key expects [string|number]");
		}

		if(lua_istable(L, ValueLuaIndex))
		{
			//lua_gettable(L, ValueLuaIndex);

			props.insert(make_pair(name, Properties()));

			lua_pushnil(L);  // first key
			readFromLuaStack(L, ResultLuaIndex - 1, props[name]);
			//lua_pop(L, 1);	// removes table
		}
		else
		{
			// value expects [table]
			THROW_ERROR("value expects [table]");
		}
		
		lua_pop(L, 1);  // removes 'value'; keeps 'key' for next iteration
     }
}
}

bool readFromVariable(std::string const& varName, PropertiesByNameT& props)
{
	// @TBD:
	return false;
}
void mutalisk::lua::readFromResult(PropertiesByNameT& props)
{
	lua_State* L = mutalisk::lua::LuaPlayer::getInstance().luaState();
	lua_pushnil(L);  // first key
	readFromLuaStack(L, ResultLuaIndex - 1, props);
//	return true;
}
void mutalisk::lua::readFromResult(Properties& prop)
{
	lua_State* L = mutalisk::lua::LuaPlayer::getInstance().luaState();
	lua_pushnil(L);  // first key
	readFromLuaStack(L, ResultLuaIndex - 1, prop);
//	return true;
}