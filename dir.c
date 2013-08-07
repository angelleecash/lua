#include <stdio.h> 
#include <string.h>
#include <limits.h>
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

#define BITS_PER_WORD (CHAR_BIT*sizeof(unsigned int))
#define I_WORD(i) ((i) / BITS_PER_WORD)
#define I_BIT(i) ((1 << i) % BITS_PER_WORD)

#define METATABLE "dir.meta"
#define checkarray() (bool_array*) luaL_checkudata(L, 1, METATABLE) 

static int dir_iter(lua_State* L)
{
	DIR* d = *(DIR**) lua_touserdata(L, lua_upvalueindex(1));
	struct dirent* entry;
	if((entry = readdir(d)) != NULL)
	{
		lua_pushstring(L, entry->d_name);
		return 1;
	}
	else
	{
		return 0;
	}
}

static int dir(lua_State* L)
{
	P("dir");
	STACK_DUMP();
	const char* path = luaL_checkstring(L, -1);

	DIR** d = (DIR**)lua_newuserdata(L, sizeof(DIR*));
	luaL_getmetatable(L, METATABLE);
	lua_setmetatable(L, -2);

	*d = opendir(path);

	STACK_DUMP();
	lua_pushcclosure(L, dir_iter, 1);

	return 1;
}

static int dir_gc(lua_State* L)
{
	P("GARBAGE COLLECTING DIR");
	DIR* d = * (DIR**) lua_touserdata(L, -1);
	if(d)
	{
		closedir(d);
	}
	return 0;
}

int luaopen_dir(lua_State* L)
{
	STACK_DUMP();

	luaL_newmetatable(L, METATABLE);
	STACK_DUMP();

	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, dir_gc);

	lua_settable(L, -3);

	lua_pushcfunction(L, dir);
	lua_setglobal(L, "dir");
	return 1;
}
/*
int main(int argc, char** argv)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	STACK_DUMP();

    lua_close(L);
	return 0;
}
*/
