#include <stdio.h>
#include "luaLib.h"
 
/**
 * 定义在lua中可调用的函数，要遵循规范：返回值必须为int，需传入lua_State
 */
int addDemo(lua_State *L)
{
	size_t len = 0;
	int num_a = luaL_checkinteger(L, 1);
	int num_b = luaL_checkinteger(L, 2);
	int num_c = num_a + num_b;

	num_c = luaLibAdd(num_a, num_b);
	lua_pushnumber(L, num_c);
	
	return 1;
}

int printDemo(lua_State *L)
{
	luaLibPrint();
	return 0;
}

int luaopen_mylib(lua_State *L)
{
	luaL_Reg Clib[] = 
	{
		{"addDemo", addDemo},
		{"printDemo", printDemo},
		{NULL, NULL}
	};
 
	luaL_newlib(L, Clib);
	return 1;
}