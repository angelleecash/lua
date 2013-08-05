CC = gcc
CFLAGS = -std=c99 -O2 -Wall -g $(SYSCFLAGS) $(MYCFLAGS)
LDFLAGS = $(SYSLDFLAGS) $(MYLDFLAGS)
LIBS = -lm -ldl $(SYSLIBS) $(MYLIBS)

SYSCFLAGS =
MYCFLAGS = 

SYSLIBS = ~/lua-5.2.2/src/liblua.a
MYLIBS = 

SYSLDFLAGS = 
MYLDFLAGS =

default: 

c_call_lua:
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) c_call_lua.c $(LIBS) 

lua_config:
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) lua_config.c $(LIBS) 

lua_call_c:
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) lua_call_c.c $(LIBS) 

clean:
	rm c_call_lua lua_config lua_call_c
