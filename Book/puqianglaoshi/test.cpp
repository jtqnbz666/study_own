#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
// bss和data段是编译时就确定好的，直到整个程序结束都不会变，但text段可能发生改变，比如加载动态库。
int arr[50000] = {1,2};//若初始化则放在data段，不初始化放bss，放bss只会标记一下，不会完全放进去。
char *str  = "hhha"; //不管是静态还是动态都在text段，，有时候可能是data段，不用纠结


int main() {
/* 这我自己写的不准确
   char arr[5] = {"test"};
   char* str1 ="hello; //str1 在栈区，观看可执行文件的大小看不出来,因为堆和栈是运行的时候才分配的。 而“hello”是放在text段的/
    str[1] = 'a'; //这样是错误的，因为对于现在的情况"hhha"放在text(代码段)不能修改。
    arr[0] = 'h'; //可以修改，因为arr在栈，编译的时候，test不占文件大小，运行的时候被放到arr的栈空间上, 而上边的str中的"hhha"放在text段，不能修改
    cout << arr;//hest

    const int a = 5; //这里的a 尽管是const修饰，但因为是局部变量，同样放在栈上,而不是data段
    static int d ;    
    static int b = 1; 
    static int c = 1999; 
*/ 

//老师讲的
     int b;                  //栈
    char s[] = "abc";       //s在栈, "abc"不在执行文件中，运行时放入s的栈中
    //s中的东西可变，下边的p3, p4不可变
    char s1[] = "abc";       //这个abc和上边的abc 不是 同一块/
    char* p1, *p2;          //栈
    char* p3 = "123456";    //p3 在栈， “123456”放在text(代码段中)
    char* p4 = "123456";    //p3和p4指向同一个地方
    //static int c = 0;       //c是局部，但又是静态，放在bss段，只管赋值是0也算初始化，如果是全局静态变量赋值为0就不算初始化
     p1 = (char*)malloc(10);
    free(p1);
     p1 = NULL;




//下边这些写在全局变量中
  /*  const int a = 1; //data， 只读 也就是 .rodata
   // static int b = 2; //data;
    int c = 3;       //data;
    int d;  //bss
    int d2 = 0;     //bss
  */
//这些写成局部
    char* p = "testr"; //p在栈区，不占text大小， "testr"在代码区(text)
    char* k;
    char* l;
    char* t;
    static int e = 5; //data
    static int f = 1;     // 若初始化为0或者不初始化（bss）；  若初始化不为0 则在data段
    char g[] = "hello world";// g在栈， 运行时hello world也放栈区
    int h = 6; //栈
    const int i = 3; //栈
    
return 0;

    
}
