CC = gcc
INCLUDE = -I ~/lua-5.2.2/src/
CFLAGS = -std=c99 -Wall -g $(SYSCFLAGS) $(MYCFLAGS) $(INCLUDE)
LDFLAGS = $(SYSLDFLAGS) $(MYLDFLAGS)
LIBS = -lm -ldl $(SYSLIBS) $(MYLIBS)

SYSCFLAGS =
MYCFLAGS = 

SYSLIBS = ~/lua-5.2.2/src/liblua.a
MYLIBS = 

SYSLDFLAGS = 
MYLDFLAGS =

default: 

c_call_lua: c_call_lua.o
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) c_call_lua.o $(LIBS) 

lua_config: lua_config.o
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) lua_config.o $(LIBS) 

lua_call_c: lua_call_c.o
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) lua_call_c.o $(LIBS) 

c_stuff: c_stuff.o
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) c_stuff.o $(LIBS) 

c_stuff.o: c_stuff.c
lua_call_c.o: lua_call_c.c
lua_config.o: lua_config.c
c_call_lua.o: c_call_lua.c

clean:
	rm -f *.o c_call_lua lua_config lua_call_c c_stuff
