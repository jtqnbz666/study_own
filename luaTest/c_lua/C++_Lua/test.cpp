#include <stdio.h>

#include <iostream>
#include <string.h>
#include <lua.hpp>
//#include <Luaclib.h>   //在linux运行时需要注释这句 
using namespace std;
#pragma comment(lib,"lua5.1.lib")


int main() {
    lua_State* L = luaL_newstate(); //lua_open();
	luaL_openlibs(L);   //必不可少
    //加载lua文件
    int bRet = luaL_loadfile(L, "hello.lua");
    if (bRet) {
        cout << "load file error" << endl;
        return  0;
    }
    //运行lua文件
    bRet = lua_pcall(L, 0, 0, 0);
    if (bRet) {
        cout << "pcall error" << endl;
        return  0;
    }

    //读取变量
    lua_getglobal(L, "str");
    string str = lua_tostring(L, -1);
    cout << "str = " << str.c_str() << endl; // str =  I am so cool

    //读取table
    lua_getglobal(L, "tbl");
    lua_getfield(L, -1, "name");
    str = lua_tostring(L, -1);
    cout << "tbl:name = " << str.c_str() << endl;
    //读取函数
    lua_getglobal(L, "add");
    lua_pushnumber(L, 10); //压入第一个参数
    lua_pushnumber(L, 20); //压入第二个参数
    int iRet = lua_pcall(L, 2, 1, 0); //调用参数， 调用完以后，会将值压入栈中
    // 2表示参数个数， 1表示返回结果个数,返回的结果会压入栈中

    if (iRet) {
        const char* pErrorMsg = lua_tostring(L, -1);
        cout << pErrorMsg << endl;
        lua_close(L);
        return  0;
    }

    if (lua_isnumber(L, -1)) {//取值输出
        double fValue = lua_tonumber(L, -1);
        cout << "Result is " << fValue << endl;

    }
    return 1;
}