#pragma once

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#ifdef _LUA_EXPORTS  
#define LUA_API __declspec(dllexport)
#else
#define lUA_API __declspec(dllimport)
#endif


//extern "C" LUA_API
int luaopen_Luaclib(lua_State * L);  //定义导出函数

