#include <stdio.h> 
#include <string.h>
#include <limits.h>

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

typedef struct bool_array
{
	int size;
	unsigned int v[1];
}bool_array;

static int newarray(lua_State* L)
{
	P("new array");
	STACK_DUMP();
	int n = luaL_checkint(L, 1);
	int bytes = sizeof(bool_array) + I_WORD(n-1) * sizeof(unsigned int);
	bool_array* ba = (bool_array*) lua_newuserdata(L, bytes);
	ba->size = n;
	for(int i=0; i <= I_WORD(n-1);i ++)
	{
		ba->v[i] = 0;
	}
	return 1;
}

static int setarray(lua_State* L)
{
	P("set array");
	STACK_DUMP();

	bool_array* ba = (bool_array*) lua_touserdata(L, 1);
	int index = luaL_checkint(L, 2) - 1;
	luaL_checkany(L, 3);

	luaL_argcheck(L, ba != NULL, 1, "array expect for set array");
	luaL_argcheck(L, index >=0 && index < ba->size, 2, "index invalid");

	int word = I_WORD(index);
	int mask = I_BIT(index);
	if(lua_toboolean(L, 3))
	{
		ba->v[word] |= mask;
	}
	else
	{
		ba->v[word] &= ~mask;
	}
	return 0;
}

static int getarray(lua_State* L)
{
	P("get array");
	STACK_DUMP();

	bool_array* ba = (bool_array*) lua_touserdata(L, 1);
	int index = luaL_checkint(L, 2) - 1;

	luaL_argcheck(L, ba != NULL, 1, "array expect for get array");
	luaL_argcheck(L, index >=0 && index < ba->size, 2, "index invalid");
	int word = I_WORD(index);
	int mask = I_BIT(index);

	lua_pushboolean(L, ba->v[word] & mask);
	return 1;
}

static int getarraysize(lua_State* L)
{
	P("get array size");
	STACK_DUMP();

	bool_array* ba = (bool_array*) lua_touserdata(L, 1);

	luaL_argcheck(L, ba != NULL, 1, "array expect for get array size");


	lua_pushinteger(L, ba->size);
	return 1;
}

static const struct luaL_Reg bool_array_lib[] = {
													{"new", newarray},
													{"set", setarray},
													{"get", getarray},
													{"size", getarraysize},
													{NULL, NULL}
												};

int luaopen_boolarray(lua_State* L)
{
	luaL_openlib(L, "boolarray", bool_array_lib, 1);
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
