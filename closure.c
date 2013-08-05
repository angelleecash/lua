#include <stdio.h> 
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>

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

static int counter(lua_State* L)
{
	int v = lua_tointeger(L, lua_upvalueindex(1));
	lua_pushinteger(L, ++v);
	lua_pushvalue(L, -1);
	lua_replace(L, lua_upvalueindex(1));
	return 1;
}

static int new_counter(lua_State* L)
{
	P("counter %p, &counter %p", counter, &counter);

	lua_pushinteger(L, 0);
	lua_pushcclosure(L, &counter, 1);

	return 1;
}

int main(int argc, char** argv)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushcfunction(L, new_counter);
	lua_setglobal(L, "new_counter");

	STACK_DUMP();

	luaL_loadfile(L, "./closure_dummy.lua");
	lua_pcall(L, 0, 0, 0);

	STACK_DUMP();

    lua_close(L);
	return 0;
}

