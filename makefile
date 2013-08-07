CC = gcc
INCLUDE = -I ~/lua-5.2.2/src/ -I ~/expat/lib
CFLAGS = -std=c99 -Wall -g $(SYSCFLAGS) $(MYCFLAGS) $(INCLUDE)
LDFLAGS = $(SYSLDFLAGS) $(MYLDFLAGS)
LIBS = -lm -ldl $(SYSLIBS) $(MYLIBS)

SYSCFLAGS =
MYCFLAGS = 

EXPAT_LIB = ~/expat/.libs/libexpat.a
EXPAT_SO = ~/expat/.libs/libexpat.so


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

string: string.o
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) string.o $(LIBS) 

closure: closure.o
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) closure.o $(LIBS) 

tuple:
	$(CC) $(CFLAGS) $(INCLUDE) -fPIC --share -o tuple.so tuple.c 

boolarray:
	$(CC) $(CFLAGS) $(INCLUDE) -fPIC --share -o boolarray.so boolarray.c 

dir:
	$(CC) $(CFLAGS) $(INCLUDE) -fPIC --share -o dir.so dir.c 

xml:
	$(CC) $(CFLAGS) $(INCLUDE) -fPIC --share -o xml.so xml.c $(EXPAT_SO)

c_stuff.o: c_stuff.c
lua_call_c.o: lua_call_c.c
lua_config.o: lua_config.c
c_call_lua.o: c_call_lua.c
string.o: string.c
closure.o: closure.c



clean:
	rm -f *.o c_call_lua lua_config lua_call_c c_stuff string closure *.so
