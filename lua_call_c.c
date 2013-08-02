#include <stdio.h> 
#include <string.h>
#include <dirent.h>

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

int my_fun(lua_State* L)
{
	fprintf(stderr, "my_fun is being called\n");
	return 0;
}

int ls(lua_State* L)
{
	const char* path = lua_tostring(L, 1);
	DIR* dir = opendir(path);
	lua_newtable(L);
	int i = 1;
	struct dirent* entry;

	while((entry = readdir(dir)) != NULL)
	{
		lua_pushnumber(L, i++);
		lua_pushstring(L, entry->d_name);
		lua_settable(L, -3);
	}

	closedir(dir);
	return 1;
}

int main(int argc, char** argv)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lua_pushcfunction(L, my_fun);

	stack_dump(L);
	lua_setglobal(L, "my_fun");
	lua_pushcfunction(L, ls);
	lua_setglobal(L, "ls");
	stack_dump(L);

	luaL_loadfile(L, "./lua_call_c.lua");
	lua_pcall(L, 0, 0, 0);
	stack_dump(L);
    lua_close(L);
	return 0;
}

