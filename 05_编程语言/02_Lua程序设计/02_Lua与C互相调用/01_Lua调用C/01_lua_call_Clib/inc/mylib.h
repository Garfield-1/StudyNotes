#ifndef __MYLIB_H__
#define __MYLIB_H__

#include <lua.h>
#include <lauxlib.h>

int addDemo(lua_State *L);
int printDemo(lua_State *L);
int luaopen_myLib(lua_State *L);

#endif