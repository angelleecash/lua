#include <stdio.h> 
#include <string.h>
#include <dirent.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"



static int ls(lua_State* L)
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

static const luaL_Reg mylib[] = {
											{"ls", ls},
											{NULL, NULL}
										};

LUAMOD_API int luaopen_mylib(lua_State* L)
{
	//fprintf(stderr, "my lib being opened");
	//luaL_newlib(L, mylib);
	luaL_openlib(L, "mylib", mylib, 1);
	//luaL_register(L, "mylib", mylib);

	return 1;
}



