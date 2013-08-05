#include <stdio.h> 
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"


void dashes(int n)
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

void stack_dump(lua_State* L)
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


int main(int argc, char** argv)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushboolean(L, 1);
	lua_pushnumber(L, 10);
	lua_pushnil(L);
	lua_pushstring(L, "hello");

//	int r = lua_checkstack(L, 1000);
//	fprintf(stderr, "check stack %d\n", r);

	stack_dump(L);
	
	lua_pushvalue(L, -4);
	stack_dump(L);

	lua_replace(L, 3);
	stack_dump(L);

	lua_settop(L, 6);
	stack_dump(L);
	
	lua_settop(L, -3);
	stack_dump(L);

	/*
	while(fgets(buff, sizeof(buff), stdin) != NULL)
	{
		int error = luaL_loadbuffer(L, buff, strlen(buff), "line");
		error |= lua_pcall(L, 0, 0, 0);
		if(error)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
		}
	}
	*/
    lua_close(L);
	return 0;
}

