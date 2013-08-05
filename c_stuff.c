#include <stdio.h> 
#include <string.h>
#include <dirent.h>

#include "luaconf.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lapi.h"


static void dashes(int n)
{
	for(int i=0; i < n; i++)
	{
		fprintf(stderr, "-");
	}
	if(n > 0)
	{
		fprintf(stderr, "\n");
	}
}


#define DASHES dashes(40);

static void stack_dump(lua_State* L)
{
	DASHES
	int top = lua_gettop(L);

	fprintf(stderr, "dumping stack size %d\n", top);
	for(int i=1; i <= top; i++)
	{
		int type = lua_type(L, i);
		switch(type)
		{
			case LUA_TSTRING:
				fprintf(stderr, "%d. string %s\n", i , lua_tostring(L, i));
				break;
			case LUA_TBOOLEAN:
				fprintf(stderr, "%d. boolean %d\n", i , lua_toboolean(L, i));
				break;
			case LUA_TNUMBER:
				fprintf(stderr, "%d. number %f\n", i , lua_tonumber(L, i));
				break;
			default:
				fprintf(stderr, "%d. other type %s\n", i , lua_typename(L, type));
				break;
		}
	}
	DASHES
}

#define STACK_DUMP() stack_dump(L);
#define P(format, args...) fprintf(stderr, "" format "\n", ## args)

int fun(lua_State* L)
{
	int v = lua_tointeger(L, -1);
	v *= 2;
	lua_pop(L, 1);
	lua_pushinteger(L, v);
	return 1;
}

int map(lua_State* L)
{
	lua_strlen(L, 1);
	int n = lua_objlen(L, 1);
	for(int i=1; i<= n; i++)
	{
		lua_pushvalue(L, 2);
		lua_rawgeti(L, 1, i);
		lua_pcall(L, 1, 1, 0);
		lua_rawseti(L, 1, i);
	}

	return 0;
}

int main(int argc, char** argv)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	STACK_DUMP();
	lua_newtable(L);
    P("table created");
	lua_pushinteger(L, 300);
	P("integer pushed %d", 300);
	lua_rawseti(L, -2, 3);
	STACK_DUMP();
	lua_setglobal(L, "t");
	luaL_loadfile(L, "./dummy.lua");
	lua_pcall(L, 0, 0, 0);
	STACK_DUMP();

	//map function
	lua_newtable(L);
	lua_pushinteger(L, 1);
	lua_pushinteger(L, 100);
    lua_settable(L,1);

	lua_pushinteger(L, 2);
	lua_pushinteger(L, 200);
    lua_settable(L,1);

	lua_pushinteger(L, 3);
	lua_pushinteger(L, 300);
    lua_settable(L,1);


	P("After set up");
	STACK_DUMP();

	lua_pushcfunction(L, fun);

	map(L);
	P("map done");
	STACK_DUMP();

	lua_pop(L, 1);

	STACK_DUMP();

	lua_setglobal(L, "t");
	luaL_loadfile(L, "./dummy.lua");
	lua_pcall(L, 0, 0, 0);
	STACK_DUMP();


    lua_close(L);
	return 0;
}

