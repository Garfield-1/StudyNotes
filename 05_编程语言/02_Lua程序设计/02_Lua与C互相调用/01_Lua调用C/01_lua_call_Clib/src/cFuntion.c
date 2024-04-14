#include <stdio.h>
#include "luaLib.h"
 
/**
 * 定义在lua中可调用的函数，要遵循规范：返回值必须为int，需传入lua_State
 */
int luaLibAdd(int a, int b)
{
	return a + b;
}

int luaLibPrint()
{
	char *str = "lua calls the C function demo";
	printf("%s\n", str);
	return 0;
}