#include <stdio.h> 
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

int main(int argc, char** argv)
{
	char buff[1024];
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	while(fgets(buff, sizeof(buff), stdin) != NULL)
	{
		int error = luaL_loadbuffer(L, buff, strlen(buff), "line");
		error |= lua_pcall(L, 0, 0, 0);
		if(error)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
		}
	}

    lua_close(L);
	return 0;
}

