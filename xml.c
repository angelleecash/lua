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

#define STACK_DUMP() (VERBOSE ? (stack_dump(L)) : (EMPTY))
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
	fprintf(stderr, "\n");
	DASHES
}

typedef struct lxp
{
	lua_State* L;
	XML_Parser parser;
} lxp;

static void start_element(void* ud, const char* name, const char** atts)
{
	P("start element %s", name);

	lxp* lxpp = (lxp*) ud;
	lua_State* L = lxpp->L;

	STACK_DUMP();

	lua_getuservalue(L, 1);
	lua_pushvalue(L, -1);
	iterate_table(L, 4);
	STACK_DUMP();
	//lua_getfield(L, -1, "startElement");
	lua_pushstring(L, "startElement");
	STACK_DUMP();
	lua_gettable(L, -2);

	lua_pushvalue(L, 1);
	lua_pushstring(L, name);
	lua_newtable(L);

	for(;*atts;atts+=2)
	{
		lua_pushstring(L, *(atts+1));
		lua_setfield(L, -2, *atts);
	}

	STACK_DUMP();

	lua_pcall(L, 3, 0, 0);
	STACK_DUMP();
	P("start element done");
}

static void chardata(void* ud, const char* s, int len)
{
	P("chardata %s", s);

	lxp* lxpp = (lxp*) ud;
	lua_State* L = lxpp->L;

	STACK_DUMP();

	lua_pushvalue(L, 1);
	lua_pushlstring(L, s, len);
	lua_pcall(L, 2, 0, 0);
	STACK_DUMP();
	
	P("chardata done");
}

static void end_element(void* ud, const char* name)
{
	P("end element %s", name);
	lxp* lxpp = (lxp*) ud;
	lua_State* L = lxpp->L;

	lua_getuservalue(L, 1);
	lua_getfield(L, -1, "endElement");

	STACK_DUMP();

	lua_pushvalue(L, 1);
	lua_pushstring(L, name);
	lua_pcall(L, 2, 0, 0);
	STACK_DUMP();

	P("end element done");
}

static int parse(lua_State* L)
{
	P("parsing ...");
	STACK_DUMP();

	lxp* lxpp = (lxp*)luaL_checkudata(L, 1, METATABLE);

	size_t len;
	const char* s = luaL_optlstring(L, 2, NULL, &len);

	//set up env for callback
	//

	lxpp->L = L;

	int status = XML_Parse(lxpp->parser, s, len, s == NULL);
	lua_pushboolean(L, status);

	STACK_DUMP();
	P("parse done");
	return 1;
}

static int make_parser(lua_State* L)
{
	P("make parser");
	lxp* lxpp;
	STACK_DUMP();

	
/*
	lua_getfield(L, -1, "startElement");

	STACK_DUMP();
	lua_pop(L, 1);
	*/

	lxpp = (lxp*) lua_newuserdata(L, sizeof(lxp));
	STACK_DUMP();
	lxpp->parser = NULL;

	luaL_getmetatable(L, METATABLE);
	STACK_DUMP();
	lua_setmetatable(L, -2);

	lxpp->parser = XML_ParserCreate(NULL);

	STACK_DUMP();

	lua_pushvalue(L, 1);
	lua_setuservalue(L, -2);

	XML_SetUserData(lxpp->parser, lxpp);
	XML_SetElementHandler(lxpp->parser, start_element, end_element);
	XML_SetCharacterDataHandler(lxpp->parser, chardata);
	P("make parser done");
	return 1;
}

static int lxp_close(lua_State* L)
{
	P("closing parser");
	lxp* lxpp = (lxp*)luaL_checkudata(L, 1, METATABLE);
	if(lxpp->parser)
	{
		XML_ParserFree(lxpp->parser);
		lxpp->parser = NULL;
	}

	P("close parser done");
	return 0;
}

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
