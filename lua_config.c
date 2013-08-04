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

void  get_table_field(lua_State* L, const char* key)
{
	lua_pushstring(L, key);
	lua_gettable(L, -2);
}

int main(int argc, char** argv)
{
	char buff[1024];
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	int k = luaL_loadfile(L, "./config.lua");
	fprintf(stderr, "----------------------%d",k);

	lua_pcall(L, 0, 0, 0);

	lua_getglobal(L, "width");
	lua_getglobal(L, "height");
    lua_getglobal(L, "background");
	stack_dump(L);

	fprintf(stderr, "width=%d\n", lua_tointeger(L, -3));
	fprintf(stderr, "height=%d\n", lua_tointeger(L, -2));

    get_table_field(L, "red");
	stack_dump(L);
	fprintf(stderr, "red=%d\n", lua_tointeger(L, -1));
	lua_pop(L, 1);

	get_table_field(L, "green");
	fprintf(stderr, "green=%d\n", lua_tointeger(L, -1));
	lua_pop(L, 1);

	get_table_field(L, "blue");
	fprintf(stderr, "blue=%d\n", lua_tointeger(L, -1));
	lua_pop(L, 1);

	lua_getglobal(L, "fun");
	lua_pcall(L, 0, 0, 0);

	lua_getglobal(L, "fun2");
	lua_pushinteger(L, 1);
	lua_pushinteger(L, 2);
	lua_pcall(L, 2, 2, 0);

	fprintf(stderr, "fun2 returned %d, %d\n", lua_tointeger(L, -2), lua_tointeger(L, -1) );

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

