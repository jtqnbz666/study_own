
#include <stdio.h>
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

int luaopen_luaclib(lua_State* L) {
	
	luaL_register(L, "ss", myLib);		//lua5.1


	/*lua_newtable(L);					//lua5.2
	luaL_setfuncs(L, myLib, 0);*/
	return 1;
}

//#include "Luaclib.h"
//#include<stdio.h>
//#include<string.h>
//
//
//static int averageFunc(lua_State* L) {
//	int n = lua_gettop(L);
//	double sum = 0;
//	int i;
//
//	//循环参数求和
//	for (i = 1; i <= n; i++) {
//		sum += lua_tonumber(L, i);
//	}
//	lua_pushnumber(L, sum / n);	//压入平均值
//	lua_pushnumber(L, sum); // 压入和
//	return 2;	//返回两个结果
//}
//
//static int sayHelloFunc(lua_State* L) {
//	printf("hello world!");
//	return 0;
//}
//
//static const struct luaL_Reg myLib[] = {
//	{"average", averageFunc},
//	{"sayHello", sayHelloFunc},
//	{NULL, NULL}
//};
////该C库的唯一入口函数。其函数签名等同于上面的注册函数。见如下几点说明：
////1. 我们可以将该函数简单的理解为模块的工厂函数。
////2. 其函数名必须为luaopen_xxx，其中xxx表示library名称。Lua代码require "xxx"需要与之对应。
////3. 在luaL_setfuncs的调用中，其第二个参数为待注册函数的数组。
////4. 需要强调的是，所有需要用到"xxx"的代码，不论C还是Lua，都必须保持一致，这是Lua的约定，
////   否则将无法调用。
//int luaopen_Luaclib(lua_State* L)
//{
//	//const char* libName = "mytestlib"; //
//	luaL_register(L, libName, mylibs); //由于在lua-5.3中已没有luaL_register这个函数，所以换成下面两行代码
//	
//	/*lua_newtable(L);
//	luaL_setfuncs(L, myLib, 0);*/
//	return 1;
//}
