#include <stdio.h> 
#include <string.h>
#include <limits.h>
#include <dirent.h>

#include "luaconf.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lapi.h"
#include "expat.h"

#define VERBOSE 1
#define EMPTY printf("%s", "")

#define STACK_DUMP(L) (VERBOSE ? (stack_dump(L)) : (EMPTY))
#define P(format, args...) (VERBOSE ? fprintf(stderr, "" format "\n", ## args) : (EMPTY))

#define BITS_PER_WORD (CHAR_BIT*sizeof(unsigned int))
#define I_WORD(i) ((i) / BITS_PER_WORD)
#define I_BIT(i) ((1 << i) % BITS_PER_WORD)

#define METATABLE "xml.meta"
#define checkarray() (bool_array*) luaL_checkudata(L, 1, METATABLE) 

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

static void iterate_table(lua_State* L, int t)
{
	lua_pushnil(L);
	while(lua_next(L, t))
	{
		P("%s(%s)-%s", lua_typename(L, lua_type(L, -2)), lua_tostring(L, -2), lua_typename(L, lua_type(L, -1)));
		//P("%s-%s", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)));

		lua_pop(L, 1);
	}
	lua_pop(L, 1);
}

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
				fprintf(stderr, "%d. string %s", i , lua_tostring(L, i));
				break;
			case LUA_TBOOLEAN:
				fprintf(stderr, "%d. boolean %d", i , lua_toboolean(L, i));
				break;
			case LUA_TNUMBER:
				fprintf(stderr, "%d. number %f", i , lua_tonumber(L, i));
				break;
			default:
				fprintf(stderr, "%d. other type %s", i , lua_typename(L, type));
				break;
		}
		fprintf(stderr, " ");
	}
	if(top > 0)
	{
		fprintf(stderr, "\n");
	}
	DASHES
}
/*
static const luaL_Reg xml_fun[] = {
									{"parse", parse},
									{"close", lxp_close},
									{"__gc", lxp_close},
									{NULL, NULL}
								  };


static const luaL_Reg xml_lib[] = {
									{"new", make_parser},
									{NULL, NULL}
								  };

int luaopen_xml(lua_State* L)
{
	luaL_newmetatable(L, METATABLE);

	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	luaL_register(L, NULL, xml_fun);
	luaL_register(L, "xml", xml_lib);

	return 1;
}
*/
int main(int argc, char** argv)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lua_State* L1 = lua_newthread(L);

	luaL_loadfile(L, "./thread_dummy.lua");
	STACK_DUMP(L);
	lua_pcall(L, 0, 0, 0);

	STACK_DUMP(L);
	lua_getglobal(L, "fun1");
	lua_pushinteger(L, 0);
	int r = lua_resume(L, L, 1);
	P("return with %d", r);
	STACK_DUMP(L);

	r = lua_resume(L, L, 0);

	P("return with %d", r);
	STACK_DUMP(L);
    lua_close(L);
	return 0;
}

