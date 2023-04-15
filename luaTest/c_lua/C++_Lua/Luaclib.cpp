#include "Luaclib.h"


static int averageFunc(lua_State* L) {
	int n = lua_gettop(L);
	double sum = 0;
	int i;

	//循环参数求和
	for (i = 1; i <= n; i++) {
		sum += lua_tonumber(L, i);
	}
		lua_pushnumber(L, sum / n);	//压入平均值
		lua_pushnumber(L, sum); // 压入和
		return 2;	//返回两个结果
}

static int sayHelloFunc(lua_State* L) {
	printf("hello world!");
	return 0;
}

static const struct luaL_Reg myLib[] = {
	{"average", averageFunc},
	{"sayHello", sayHelloFunc},
	{NULL, NULL}
};

int luaopen_Luaclib(lua_State* L) {
	printf("到此为止!");
	luaL_register(L, "ss", myLib);
    
    //#define lua_register(L,n,f) (lua_pushcfunction(L, (f)), lua_setglobal(L, (n)))

	/*lua_newtable(L);
	luaL_setfuncs(L, myLib, 0);*/
	return 1;
}
