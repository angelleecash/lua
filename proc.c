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
#include "pthread.h"

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

#define REGISTRY_PROC "registry.proc"

typedef struct Proc
{
	lua_State* L;
	pthread_t thread;
	pthread_cond_t cond;
	const char* channel;
	struct Proc *previous, *next;
}Proc;

static Proc* wait_send = NULL;
static Proc* wait_receive = NULL;
static pthread_mutex_t kernel_access = PTHREAD_MUTEX_INITIALIZER;

static Proc* get_proc(lua_State* L)
{
	lua_getfield(L, LUA_REGISTRYINDEX, REGISTRY_PROC);

	Proc* p = (Proc*) lua_touserdata(L, -1);
	lua_pop(L, 1);
	return p;
}

static void move_values(lua_State* send, lua_State* receive)
{
	int n = lua_gettop(send);
	for(int i=2; i <= n; i++)
	{
		lua_pushstring(receive, lua_tostring(send, i));
	}
}

static Proc* search_proc(Proc** list, const char* channel)
{
	Proc* proc = *list;
	if(proc == NULL)
	{
		return NULL;
	}

	do
	{
		if(strcmp(channel, proc->channel) == 0)
		{
			if(*list == proc)
			{
				*list = (proc->next == proc) ? NULL : proc->next;
			}
			proc->previous->next = proc->next;
			proc->next->previous = proc->previous;
			return proc;
		}

		proc = proc->next;
	}while(proc != *list);
	return NULL;
}

static void wait_on_list(lua_State* L, const char* channel, Proc** list)
{
	Proc* proc = get_proc(L);
	if(*list == NULL)
	{
		*list = proc;
		proc->next = proc;
		proc->previous = proc;
	}
	else
	{
		proc->previous = (*list)->previous;
		proc->previous->next = proc;
		proc->next = *list;
		(*list)->previous = proc;
	}
	proc->channel = channel;

	do
	{
		pthread_cond_wait(&proc->cond, &kernel_access);
	}while(proc->channel);
}

static int send(lua_State* L)
{
	const char* channel = luaL_checkstring(L, 1);
	pthread_mutex_lock(&kernel_access);
	Proc* proc = search_proc(&wait_receive, channel);
	if(proc)
	{
		P("receiver found for channel %s", channel);
		move_values(L, proc->L);
		proc->channel = NULL;
		pthread_cond_signal(&proc->cond);
	}
	else
	{
		P("no receiver for channel %s, wait...", channel);
		wait_on_list(L, channel, &wait_send);
	}

	pthread_mutex_unlock(&kernel_access);
	return 0;
}

static int receive(lua_State* L)
{
	const char* channel = luaL_checkstring(L, 1);
	lua_settop(L, 1);

	pthread_mutex_lock(&kernel_access);
	Proc* proc = search_proc(&wait_send, channel);

	if(proc)
	{
		P("sender found for channel %s", channel);
		move_values(proc->L, L);
		proc->channel = NULL;
		pthread_cond_signal(&proc->cond);
	}
	else
	{
		P("no sender for channel %s, wait...", channel);
		wait_on_list(L, channel, &wait_receive);
	}

	pthread_mutex_unlock(&kernel_access);
	return lua_gettop(L) - 1;
}

int luaopen_proc(lua_State* L);
static void* run(void* arg)
{
	P("thread running ...");
	lua_State* L = (lua_State*)(arg);
	luaL_openlibs(L);
	lua_cpcall(L, luaopen_proc, NULL);
	STACK_DUMP(L);
	lua_pcall(L, 0, 0, 0);
	Proc* proc = get_proc(L);
	pthread_cond_destroy(&(proc->cond));
	lua_close(L);
	return NULL;
}

static int start(lua_State* L)
{
	const char* chunk = luaL_checkstring(L, 1);
	P("loading chunk:\n%s", chunk);
	lua_State* L1 = luaL_newstate();
	luaL_loadstring(L1, chunk);

	pthread_t thread;
	int status = pthread_create(&thread, NULL, run, L1);
	P("create thread %s", status?"fail":"succeed");
	pthread_detach(thread);
	return 0;
}

static int proc_exit(lua_State* L)
{
	pthread_exit(NULL);
	return 0;
}

static const luaL_Reg proc_lib[] = {
										{"start", start}, 
										{"send", send},
										{"receive", receive}, 
										{"exit", proc_exit},
										{NULL, NULL}
								    };

int luaopen_proc(lua_State* L)
{
	Proc* proc = (Proc*) lua_newuserdata(L, sizeof(Proc));
	lua_setfield(L, LUA_REGISTRYINDEX, REGISTRY_PROC);
	proc->L = L;
	proc->thread = pthread_self();
	proc->channel = NULL;
	pthread_cond_init(&proc->cond, NULL);
	luaL_register(L, "proc", proc_lib);
	return 1;
}

/*
int main(int argc, char** argv)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	STACK_DUMP(L);
    lua_close(L);
	return 0;
}
*/
