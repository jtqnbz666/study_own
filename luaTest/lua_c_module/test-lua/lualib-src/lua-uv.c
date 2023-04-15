#include <lua.h>

#include <lauxlib.h>

#include <lualib.h>

#include <stdio.h>

// 闭包实现：  函数 + 上值  luaL_setfuncs
// lua_upvalueindex(1)
// lua_upvalueindex(2)
static int
lecho (lua_State *L) {
	lua_Integer n = lua_tointeger(L, lua_upvalueindex(2));
    n++;
    const char* str = lua_tostring(L, -1);
    fprintf(stdout, "[n=%lld]---%s\n", n, str);
    lua_pushinteger(L, n);
    lua_replace(L, lua_upvalueindex(2));
    return 0;
}

static const luaL_Reg l[] = {
	{"echo", lecho},
	{NULL, NULL},
};

int
luaopen_uv_c(lua_State *L) { // local tbl = require "tbl.c"
    // 创建一张新的表，并预分配足够保存下数组 l 内容的空间
	luaL_newlibtable(L, l);// 1
    lua_pushinteger(L, 0);// 2          //这是压入上值
    lua_pushinteger(L, 10);// 3         //同样是上值
	luaL_setfuncs(L, l, 2);//           传入两个上值
    // luaL_newlib(L, l);
	return 1;
}
