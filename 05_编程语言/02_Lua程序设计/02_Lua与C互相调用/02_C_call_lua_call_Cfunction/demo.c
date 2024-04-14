#include <stdio.h>
#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

int addDemo(lua_State *L)
{
	int num_a = luaL_checkinteger(L, 1);
	int num_b = luaL_checkinteger(L, 2);
	int num_c = num_a + num_b;

	lua_pushnumber(L, num_c);
	
	return 1;
}

int main(void)
{
    lua_State *L = luaL_newstate();    // 创建Lua状态机。
    luaL_openlibs(L);                  // 打开Lua状态机"L"中的所有Lua标准库。

    lua_pushcfunction(L, addDemo);
    lua_setglobal(L, "addDemo");      // 弹出栈顶元素，并在Lua中用名为"addDemo"的全局变量存储。

    const char* testfunc = "add = addDemo(1,2);print(\"add =\",add)";

    if(luaL_dostring(L, testfunc)) {
        printf("Failed to invoke.\n");
    }

    lua_close(L);    // 关闭Lua状态机。

    return 0;
}