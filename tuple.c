#include "stdio.h"

#include "lua.h"
#include "lauxlib.h"

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

#define STACK_DUMP stack_dump(L)
#define P(format, args...) fprintf(stderr, "" format "\n", ## args)

int tuple(lua_State* L)
{
	STACK_DUMP;
	int op = luaL_optint(L, 1, 0);
	if(op == 0)
	{
		P("get all");
		int i=1;
		for(; !lua_isnone(L, lua_upvalueindex(i)); i++)
		{
			lua_pushvalue(L, lua_upvalueindex(i));
		}

		STACK_DUMP;
		return i-1;
	}
	else
	{
		if(lua_isnone(L, lua_upvalueindex(op)))
		{
			return 0;
		}

		lua_pushvalue(L, lua_upvalueindex(op));
		return 1;
	}

}

int new(lua_State* L)
{
	lua_pushcclosure(L, tuple, lua_gettop(L));
	return 1;
}

static const struct luaL_Reg tuplelib[] = {
											{"new", new},
											{NULL, NULL}
										};

int luaopen_tuple(lua_State* L)
{
	luaL_openlib(L, "tuple", tuplelib, 1);
	return 1;
}
