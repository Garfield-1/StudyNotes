# Lua调用C

Lua调用C的几种主流方法

1. 通过在C中注册函数给lua调用
2. 封装成c动态链接库，在lua中require
3. 在LuaJIT里面可以使用ffi高性能的调用C

## 调用C动态库

将C函数封装成动态库，通过`luaL_newlib` 将C 函数打包成一个 Lua 模块，以供 Lua 脚本使用

