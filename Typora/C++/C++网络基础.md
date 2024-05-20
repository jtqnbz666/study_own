

##### C与C++的区别** (一)

+ 命名空间

  + 基本语法

  ```c++
  //空间名
  namespace  标识符
  {
  	//变量
      //函数
      //结构体
      //类 
  }
  namespace MM
  {
  	int age;
      char name[20];
  }
  ```

  + 如何访问空间中的东西： 空间名::空间中的成员

  ```c++
  //基本访问方式
  //相对于C语言来说，需要一个前缀
  //::  作用域分辨符
  MM::age=1001;
  strcpy(MM::name,"灰姑凉");
  //省略前缀
  using namespace 空间名;    //省略当前这个空间名
  ```

  使用using 语法注意点是: 防止空间变量或者函数不要和空间外的变量或者函数名字相同

  + 命名空间的嵌套

  ```c++
  namespace A
  {
      int num;
  	namespace B
      {
          int age;
      }    
  }
  //如何访问: 剥洋葱
  A::num=1001;
  A::B::age=18;
  //省略前缀
  using namespace A;
  using namespace A::B;
  num=1991;
  age=1992
  ```

+ C++最简单的程序

  ```c++
  #include <iostream>
  #include <stdio.h>
  #include <ctime>
  #include <cstdlib>
  using namespace std;
  int main() 
  {
  	return 0;
  }
  ```

  + 头文件的改变
    + 标准输入输出头文件: #include <iostream>
    + 包含C语言的文件： #include <cxxx> 当然用#include <xxx.h>
    + 自己写的头文件还是用C语言的方式包含  #include ”myhead.h“
  + 基本输出的改变： cout 加上<<
    + 不再需要格式控制字符
    + 依然支持转移字符
    + 换行： endl替换\n
  + 基本输入的改变： cin 加上>>
    + 不再不需要格式空字符

  ```c++
  #include <iostream>
  #include <stdio.h>
  #include <ctime>
  #include <cstdlib>
  void testPrint() 
  {
  	int age=1;
  	char name[20]="老babay";
  	double dNum=1.1;
  	std::cout << age << "\n";
  	std::cout << age << "\t" << name << "\t\t" << dNum << "\n";
  	using namespace std;			//一般情况这句话会写头文件下面
  	cout << age << "\t" << name << "\t\t" << dNum << "\n";
  	cout << age << "\t" << name << "\t\t" << dNum << endl;
  	//cout.put('A');  IO流中讲
  }
  void testInput() 
  {
  	char name[20];
  	int age;
  	int num;
  	//单个数据输入
  	std::cout << "请输入一个整数:";
  	std::cin >> age;
  	std::cout << "请输入name,age,num:";
  	std::cin >> name >> age >> num;
  	std::cout << name << "\t" << age << "\t" << num << std::endl;
  }
  int main() 
  {
  	testPrint();
  	testInput();
  	return 0;
  }
  ```

+ C++数据类型的改变
  + 空指针的改变: NULL  改为nullptr
  + bool类型引入
    + 赋值为: true ,false
    + 非零值非空值表示成立，只有0和空表示不成立
    + bool占用字节数是1
    + 打印结果只有两个值: true:1 false: 0
    + 一般条件表达式或者逻辑表达式，或者充当开关变量，标记变量

+ 引用类型： C++极其重要的类型

  + 基本用法: 起别名

  ```c++
  类型& 标识符1=标识符2
  //标识符2有一个别名字的叫做标识符1， 他们是一个东西
  ```

  + 常引用： 给常量起别名

  ```c++
  const 类型& 标识符=常量;
  ```

  + 右值引用: 给右值起别名

  ```c++
  类型&& 标识符=右值
  ```

  + 引用的用法
    + 当做函数参数:防止拷贝本的产生
    + 当做函数返回值： 增加左值使用用法(等效返回值一个变量)

  + 自动推断类型： auto类型

  ```c++
  #include <iostream>
  using namespace std;
  bool empty(int size) 
  {
  	return size == 0;
  }
  //Reference types 引用类型
  int  returnValue(int num) 
  {
  	return num;
  }
  int num = 0;
  int& returnValueReference()
  {
  	return num;			//warning C4172: 返回局部变量或临时变量的地址: num
  }
  void SwapC(int a, int b)  //int a=实参1  int b=实参2
  {
  	int temp = a;
  	a = b;
  	b = temp;
  }
  void SwapCpp(int& a, int& b) //int &a =实参1 ，int &b=实参2
  {
  	//以后想要在子函数中修改什么，传入相应的引用
  	int temp = a;
  	a = b;
  	b = temp;
  }
  void modify(int* &p) 
  {
  	p = &num;
  }
  //const在C++上面更为严格，类型需要严重性的一致
  //如果你想要既可以传入常量也可以传入变量，需要const修饰
  void  printConstValue(const char* str) 
  {
  	cout << str << endl;
  }
  //只需要传入常量
  void printOnlyConstValue(int&& num)
  {
  	cout << num << endl;
  }
  void testReferenceTypes() 
  {
  	//3.1 基本用法
  	int a = 1;
  	int& b = a;					//a就是b b就是a
  	b = 100;
  	cout << a << endl;
  	cout << b << endl;
  	//int& constNum = 12;		//常量的引用，这样是错误的 
  	const int& constNum = 12;
  	//右值引用也可以表示常量
  	int&& rightValue = 1001;
  	//3.2 引用的用法
  	//充当函数参数：防止拷贝本产生
  	//当做函数返回值: 增加左值使用  不能返回局部变量引用
  	//返回值是一个值
  	//returnValue(num) = 12;		//错误
  	//1 = 2;
  	returnValueReference() = 1111;
  	cout << num << endl;
  	//充当函数
  	int aa = 1;
  	int bb = 2;
  	SwapC(aa, bb);
  	cout << aa << "\t" << bb << endl;
  	SwapCpp(aa, bb);  //注意不需要取地址传参
  	cout << aa << "\t" << bb << endl;
  	int* p = nullptr;
  	modify(p);
  	cout << *p << endl;
  	//常引用传参
  	printConstValue("ILoveyou");
  	//右值引用当做函数参数，只能传入右值
  	printOnlyConstValue(1212);
  	//move可以把左值变成右值
  	printOnlyConstValue(move(num));
  }
  
  //auto类型
  int  Max(int a, int b) 
  {
  	return a > b ? a : b;
  }
  void printMax(int(*p)(int, int), int a, int b) 
  {
  	cout << p(a, b) << endl;
  }
  
  void testAuto() 
  {
  	//5.1 不能单独定义变量
  	//auto a;  错误
  	//5.2 auto一般结合赋值使用
  	auto intNum = 1;
  	cout << intNum << endl;   //int
  	int (*pMax)(int, int) = Max;
  	auto ppMax = Max;
  	cout << ppMax(1, 2) << endl;
  	auto pp = printMax;
  	pp(ppMax, 1, 2);
  }
  
  int main() 
  {
  	//1.基本数据类型的改变
  	int* p = nullptr;
  	//2.bool类型引入
  	bool bNum = true;		//正常赋值,用true和false
  	//非正常的赋值
  	bNum = -111;
  	cout << bNum << endl;	//1
  	//3.引用类型
  	testReferenceTypes();
  	//4. :: 作用分辨符
  	//就近原则
  	int num = 100000;
  	cout << num << endl;
  	cout << ::num << endl;		//代表是全局区变量
  	//5.自动推断类型
  	testAuto();
  	return 0;
  }
  ```

+ C++函数的改变 

  + 内联思想： 函数以二进制形式存在，去提高效率

    + 内联函数：用inline修饰的函数

    + 短小精悍
    + 在结构体中或者类型实现的函数，默认为内联

  + 重载思想： C++允许同名不同参的函数存在

    + 参数个数不同
    + 参数类型不同
    + 参数顺序不同(建立在存在不同类型)
    + 常属性的成员函数和类中的普通函数重载  (类中会讲)

  + 缺省思想： 给函数形参初始化，达到不同形态的函数调用

    + 缺省的顺序 必须是从右往左，连续缺省
    + 缺省时候，要避免存在普通函数，防止存在二义性

  + Lambda表达式： 函数的定义 并且返回一个函数指针，所以一般在C++中会结合auto使用

    ```c++
    [捕获方式](函数参数)能否修改 是否存在异常->函数返回值类型{函数体;};
    捕获方式: 理解使用函数外面的变量的方式
    1.[]
    2.[=] 
    3.[&]
    4.[&x,=]
    5.[this]  类中数据成员的捕获
    函数参数： 自己写函数的时候的函数参数
    能否修改: mutable
    是否存在异常: throw() 不存在异常
    函数返回值类型:自己写函数的时候的函数返回值
    函数体： 原来函数的函数体
    //注意点： 一般在用的时候，能否修改 是否存在异常->函数返回值类型 是可以省略
    [捕获方式](函数参数){函数体;};
    ```
  
  ```c++
  #include <iostream>
  using namespace std;
  inline int Max(int a, int b) 
  {
  	return a > b ? a : b;
  }
  //1.参数的数目不同
  void print(int a, int b, int c) 
  {
  	cout << a + b + c << endl;
  }
  void print(int a, int b) 
  {
  	cout << a + b << endl;
  }
  //2.参数的类型不同
  void print(char a, char b)
  {
  	cout << a + b << endl;
  }
  //3.顺序不同
  //错误，不可以跟上面函数构成重载
  //void print(char b, char a) 
  //{
  //	cout << a + b << endl;
  //}
  void print(int age, double num) 
  {
  	cout << age << "\t" << num << endl;
  }
  void print(double num, int age) 
  {
  	cout << age << "\t" << num << endl;
  }
  //const属性不构成重载
  //void print(int a) 
  //{
  //	cout << a << endl;
  //}
  //void print(const int a) 
  //{
  //	cout << a << endl;
  //}
  //4.缺省:不传入参数使用默认参数
  void printValue(int a=1, int b=2, int c=3,int d=4) 
  {
  	cout << a + b + c + d << endl;
  }
  //5.Lambda表达
  void printMax(int(*p)(int, int), int a, int b) 
  {
  	cout << p(a, b) << endl;
  }
  void testLambda() 
  {
  	//最完整的Lambda表达式
  	//int Max(int a, int b) 
  	int(*pMax)(int, int) = [](int a, int b)mutable throw()->int {return a > b ? a : b; };
  	//一般用的时候，怎么简单怎么来，结合auto+省略的Lambda表达式去使用
  	auto p= [](int a, int b){return a > b ? a : b; };
  	cout << p(1, 2) << endl;
  	//进阶一下： 函数的定义和调用写在一起
  	cout << [](int a, int b) {return a > b ? a : b; }(1, 3) << endl;
  	printMax([](int a, int b) {return a + b; }, 3, 4);
  	printMax([](int a, int b) {return a - b; }, 3, 4);
  	//捕获方式
  	int num = 1;
  	//[]() {cout << num << endl; };错误  没有捕获方式，表示没用权力
  	auto p1=[=]() {cout << num << endl; };
  	auto p2 = [&]() {cout << num << endl; };
  	p1();					//打印1
  	p2();
  	num = 1001;
  	p1();					//不会因为值的改变而改变调用  ，打印1
  	p2();
  	//[this] 后面讲了类的时候在讲
  }
  int main() 
  {
  	cout << Max(1, 2) << endl;
  	print(1, 2, 3);
  	print(1, 2);
  	print('A', 'B');
  	print(1, 1.1);
  	print(1.1, 1);
  	printValue();			//a=1,b=2,c=3,d=4;
  	printValue(11);			//a=11,b=2,c=3,d=4;
  	printValue(11, 22);       //a=11,b=22,c=3,d=4
  	testLambda();
  	return 0;
  }
  ```

##### **C与C++的区别** (二）

+ 动态申请内存的区别

  + C语言中申请内存: realloc malloc calloc  C++中只有new
  + C语言释放内存: free   C++中 delete

  ```c++
  #include <iostream>
  #include <cstdio>
  #include <assert.h>
  #include <cstring>  //和#include <string>有区别	
  using namespace std;
  struct MM 
  {
  	char name[20];
  	int age;
  };
  void testNewMemory() 
  {
  	//1.申请单个变量内存
  	int* pC = (int*)malloc(sizeof(int));
  	assert(pC);
  	*pC = 123;
  	free(pC);
  	pC = nullptr;
  	int* cpp = new int;
  	*cpp = 123;
  	cout << *cpp << endl;
  	delete cpp;
  	cpp = nullptr;
  	//2.申请一段内存   一维数组
  	int* pcArray = (int*)malloc(sizeof(int) * 3);  //int pcArray[3];
  	assert(pcArray);
  	pcArray[0] = 1;
  	cout << pcArray[0] << endl;
  	free(pcArray);
  	pcArray = nullptr;
  	int* cppArray = new int[3];    //int cppArray[3];
  	cppArray[0] = 1;
  	cout << cppArray[0] << endl;
  	delete[] cppArray;
  	cppArray = nullptr;
  	//3.申请内存并做初始化
  	//C语言有一个calloc
  	//3.1 单个数据用()
  	int* pNum = new int(100);      //*pNum=100
  	cout << pNum << endl;
  	delete pNum;
  	pNum = nullptr;
  	//3.2 多个数据用{}
  	int* pArray = new int[3]{ 1,2,3 };  //int pArray[3]={1,2,3};
  	for (int i = 0; i < 3; i++) 
  	{
  		cout << pArray[i] << "\t";
  	}
  	cout << endl;
  }
  //C++允许你重新管理申请堆内存
  void testReNew() 
  {
  	char* sum = new char[100];
  	//前面十六个字节存放4个整数
  	//(sum + 0)代表是从那个位置开始申请
  	int* pInt = new(sum + 0) int[4]{1,2,3,4};
  	//来10个字节存储字符串
  	char* pstr = new(pInt + 4) char[10]{"ILoveyou"};
  	//等效：char* pstr = new(sum + 16) char[10]{"ILoveyou"};
  	cout << sum + 16 << endl;
  	cout << pstr << endl;
  	//....
  	delete[] sum;
  	sum = nullptr;
  }
  void testUserData() 
  {
  	//结构体不需要struct关键字
  	//单一结构体也是{}初始化
  	MM* pMM = new MM { "张三", 18 };
  	cout << pMM->name << "\t" << pMM->age;
  }
  int main() 
  {
  	testNewMemory();
  	testReNew();
  	testUserData();
  	return 0;
  }
  ```

+ C++结构体的基本区别

  + 类型上： 不在需要struct ，直接结构体名可以充当类型，.c文件必须struct关键字
  + 访问方式和C语言没区别
    + 必须要用结构体变量访问
    + 变量访问的方式： 变量.成员
    + 指针表示，访问: 指针->成员

  + C++结构体中允许存在函数
    + 结构体中的函数如何访问数据：直接访问
    + 结构体中函数如何在类外实现：函数名必须要用：结构体名::函数名
    + 通过结构体中的函数去设置结构体数据
    + C++结构体申请内存

  ```c++
  #include <cstring>
  #include <iostream>
  using namespace std;
  struct MM 
  {
  	//数据成员
  	char name[20];
  	int age;
  	//成员函数
  	void print() 
  	{
  		cout << name << "\t" << age << endl;
  	}
  	void printData();
  	void setData(const char* mmName, int mmAge);
  };
  void MM::printData() 
  {
  	cout << name << "\t" << age << endl;
  }
  void MM::setData(const char* mmName, int mmAge) 
  {
  	strcpy_s(name,20, mmName);    //"宝宝"
  	age = mmAge;				  //19
  }
  void testCppStruct() 
  {
  	//1.基本的访问方式
  	MM mm = { "张三",19 };
  	cout << mm.name << "\t" << mm.age << endl;
  	MM* pMM = &mm;
  	cout << pMM->name << "\t" << pMM->age << endl;
  	//2.C++结构体中的函数的访问
  	mm.print();						//打印就是mm.name,mm.age
  	pMM->print();
  	MM baby = { "baby",19 };
  	baby.print();					//baby.name, baby.age
  	baby.printData();
  	//3.通过函数去描述行为
  	MM boy;
  	boy.setData("宝宝", 19);			//boy.name="宝宝"，boy.age=19
  	boy.print();					//boy.name ,boy.age
  	//4.C++结构体申请内存
  	//这种写法是建立在没有构造函数的基础下是对的
  	MM* p = new MM;
  	p->setData("申请内存", 199);
  	p->print();
  	delete p;
  	p = nullptr;
  	MM* pArray = new MM[3];    
  	for (int i = 0; i < 3; i++) 
  	{
  		pArray[i].setData("数组", i + 19);
  		pArray[i].print();
  	}
  	delete[] pArray;
  	pArray = nullptr;
  }
  int main() 
  {
  	testCppStruct();
  	return 0;
  }
  ```

+ C++中string： 本节课只需要知道如何使用C++string即可，不需要知道为什么这样用

  + 首先需要知道C++string 是一个类，头文件是#include<string>
  + 如何使用C++string

  + string 中的一些函数操作 [basic_string 类 | Microsoft Docs](https://docs.microsoft.com/zh-cn/cpp/standard-library/basic-string-class?view=msvc-160)

  ```c++
  #include <string>
  #include <iostream>
  #include <cstdio>
  using namespace std;
  void testCppString() 
  {
  	//1.常用的创建方式
  	//1.1常用的创建方式
  	string str1;				//类似创建变量的方式
  	std::string noStd;			//没有using namespace std ,也需要加前缀
  	str1 = "ILoveyou";
  	cout << str1 << endl;
  	string str2("ILoveyou");
  	cout << str2 << endl;
  	string str3(str2);
  	cout << str3 << endl;
  	//1.2不常用
  	string str4(5, 'O');				//str4="OOOOO";
  	cout << str4 << endl;
  	string str5("Iloveyou", 1, 5);		//从0开始，用第一个到第五个字符初始化
  	cout << str5 << endl;
  	//2.基本操作
  	//2.1string里面有一个函数可以表示字符串属性
  	cout << str5.size() << endl;		//元素个数
  	cout << str5.length() << endl;		//长度
  	cout << str5.capacity() << endl;	//容量
  	string  longStr = "12345678910123423";
  	cout << longStr.capacity() << endl;
  	//2.2 常规操作
  	//比较，直接比即可  所有条件运算符直接用
  	//运算符重载
  	cout << (longStr > str5) << endl;
  	cout << (longStr == str5) << endl;
  	cout << (longStr != str5) << endl;
  	//连接直接用+
  	string strF = "First";
  	string strS = "Second";
  	string result = strF + strS;
  	cout << result << endl;
  	//3.string与char* 转换
  	//不能用%s方式直接打印string
  	//调用string类中两个函数
  	//data() ,c_str()
  	string info("C++中的string");
  	printf("%s\n", info.c_str());
  	printf("%s\n", info.data());
  	char name[20] = "";
  	string strName = "张三";
  	strcpy_s(name, 20, strName.c_str());
  	printf("%s\n", name);
  	//4.下表法访问string
  	string pStr = "ILoveyou";
  	for (int i = 0; i < pStr.length(); i++) 
  	{
  		cout << pStr.at(i);
  	}
  	cout << endl;
  	cout << pStr << endl;
  }
  int main() 
  {
  	testCppString();
  	return 0;
  }
  ```

+ C++类型转换 

  + 基本数据类型转换: 类型(变量)
  + static_cast
    + 可以用做基本数据类型转换
    + 把空类型的指针转换为目标指针类型
    + 不能转换带有const属性指针
  + const_cast： const属性指针的一些转换操作
  + reinterpret_cast：指针转整数，整数转指针
  + dynamic_cast    :后续讲完多态在给同学们讲解  

##### 作业

写两个子函数

+ 第一个子函数，实现二维数组的内存申请
+ 第二个子函数，实现内存释放

##### 类和对象  

+ 类和对象
  + 什么是类? 类是一些具有共同属性和行为的事物的抽象
  + 什么是对象? 类的具体化(实例化)，明确属性和行为
  + 属性: 数据成员(int，float，char...)描述， 一系列事物公有特征
  + 行为: 成员函数(类中函数)描述,一系列公有事物共同操作

+ 类的特点
  + 封装性
  + 继承性
  + 多态性
  + 抽象性
  + 隐藏性

+ 学会创建类的语法
  + 权限限定词:public,protected,private，在结构体中也是可以使用
  + 类中默认是private属性，结构体中默认是public属性
  + 权限限定词作用是用来限定类外对类中数据访问
    + 一般情况类外访问类中数据必须通过对象来访问，所以我们写C++程序，做的第一件事是创建对象
    + 权限限定词是可以多个，或者没有

```c++
class 类名
{
public:			//公有
protected:		//保护
private:    	//私有
};  //分号必须要有的
```

+ 明白类中的权限问题
  + 静态数据成员和静态成员也受权限限定，可以不需要对象也可以调用
  + 类外只能访问public属性，别的属性都不能被访问，类中没有权限限定
  + protected和private 在继承中有区别，目前不需要知道
+ 学会如何访问类中的数据
  + 类中数据可以直接初始化
  + 接口的概念，C++当中一般指是public属性下方法(成员函数)
  + 两种修改类中数据的方式
    + 通过提供一个带参的函数去修改数据成员
    + 通过返回引用的方式修改数据成员

+ C++各种对象的形式对类中数据的访问

  + 普通对象
    + 注意权限问题，类外只能访问public属性下内容
    + 对象用: 对象.成员
  + 对象数组
  + 对象指针
    + 可以用指针指向运算符(->)访问 ：指针->成员
    + 也可以用(*指针).成员

  + 对象本身就是一个数据，所以普通数据能做他都可以做
    + 当做函数返回值
    + 当做函数参数
    + 当做变量赋值

  + this指针
    + this指针可以解决形参名和数据成员名字相同的问题
    + 可以函数充当函数返回值

  + 类的成员函数的表示方式
    + 如何定义一个类的成员函数的指针
    + 如果通过成员函数的函数指针调用函数

##### 构造函数和析构函数   

+ 构造函数
  + 没有返回值
  + 函数名和类名相同
  + 构造对象的时候被调用
  + 构造函数一般情况都是用来数据成员初始化
  + 一般情况构造函数是public属性
  + 默认的拷贝构造函数是没有参数，无参构造函数,一旦自己写构造函数，默认的就不存在
+ 拷贝构造函数
  + 拷贝构造函数也是构造函数,具有构造函数相同属性
  + 拷贝构造函数只有一个参数，就是对对象的引用
  + 拷贝构造主要是为了实现通过一个对象去创建对象
+ 析构函数
  + 函数名: ~类名
  + 析构函数没有参数
  + public属性
  + 不写析构存在一个默认的析构
  + 析构函数是对象死亡的时候(生命周期结束)，自动调用，不需要人调用
  + 什么时候需要手动写析构函数，当类中的数据成员做到了内存申请的时候，需要手动写析构函数

+ 深拷贝和浅拷贝： 当数据成员是指针的时候，并做内存申请
+ 默认的函数的相关操作
  + 我们可以删除默认的函数，用delete删除
  + 我们使用默认的函数，用default函数

##### C++特殊成员  

+ const成员

  + 常数据成员： const修饰的数据成员

    + const属性代表只读(不可以修改)

    + 常数据成员的初始化必须要采用初始化参数列表

    ```c++
    //初始化参数列表
    类名(变量1，变量2,...):数据成员1(变量1)，数据成员2(变量2)...{}
    ```

  + 常成员函数:  const修饰的成员成员函数,注意写法，const是放在函数后面

    + 在当前常成员函数中不能修改数据成员
    + 常成员可以和普通同时存在

  + 常对象: const 修饰对象

    + 常对象只能调用常成员函数

+ static成员

  + static数据成员
    + 必须在类外做初始化
    + 静态数据成员是属于类的，不属于对象，是所有对象共有，也就是说所有对象用的是一个数据
    + 静态数据成员访问不需要对象，可以直接用类名限定访问: 类名::成员
    + 静态数据成员也受权限限定
  + static成员函数
    + 访问可以不需要对象，用对象访问也可以的
    + 也受权限限定
    + 静态成员函数不能直接访问非静态数据成员,只能通过指定访问
  + static对象
    + 保留上一次运行的结果，初始化操作只做一次

+ 类的组合 ：一个类是另一个类一部分

  + 构造函数的的写法：必须采用初始化参数列表
  + 构造的顺序(考试)
    + 组合类中构造顺序和初始化参数列表写的顺序无关，只和声明顺序有关

##### C++友元  

+ 友元函数：单纯提供一个场所给予类的对象具有无视权限的功能
  + 普通函数成为友元： 再类中用friend 声明即可
  + 以另一个类的成员函数成为友元函数
+ 友元类

##### C++类的继承  

+ 继承基本语法

```c++
class 子类名: 继承方式 父类名
{

    
};
//继承方式: 权限限定词
//public  公有
//protected: 保护
//private: 私有继承
class 派生类名:继承方式 基类名
{
    //生成新的属性和行为
};
```

继承的实质: 父类(基类)当中的属性，子类(派生类)中也有一份，这一份的属性是由继承方式的决定的。

+ 继承权限问题

|                 | public    | protected | private  |
| --------------- | --------- | --------- | -------- |
| public ：继承   | public    | protected | 不可访问 |
| protected：继承 | protected | protected | 不可访问 |
| private:继承    | private   | private   | 不可访问 |

+ 继承具有遗传性

继承的属性是一值存在的，无论被继承多少代，都是存在，所以一般类的击沉不会写太多层数，导致类很冗长。

+ 继承中的构造函数
  + 子类构造函数必须要调用父类的构造函数
  + 子类的构造函数必须采用初始化列表的写法去初始化继承下来的属性

+ 继承的分类
  + 单继承：只有一个父类
  + 多继承: 2个或者以上的父类
    + 菱形继承

+ 继承中的同名访问问题  //21:20继续
  + 正常赋值的访问：  默认访问方式采用就近原则,当然可以采用类名限定指定的方式
  + 非正常赋值的访问： 没有virtual看指针类型

+ 继承的作用：

  抽象中的抽象。其实大家在使用继承的时候，更多的是继承别人的东西

  自己设计代码采用继承的方式，更多增加代码的重用性

C++继承作业: 抽象画图工具的工具类

##### C++虚函数和多态 

+ 虚函数与虚函数表
+ 纯虚函数和抽象类
+ 多态
+ ADT过程和虚析构函数
+ dynamic下行转换和交叉转换  
+ 类中类的访问

##### C++模板    

+ 函数模板
+ 类模板
+ 模板特化

##### C++可变参模板   

+ 可变参函数模板
  + 折叠参数类型的定义 : typename ...Arg  
  + 折叠参数的定义: Arg  ...arg;
  + 如何展开折叠参数
    + 递归的方式去展开参数包
    + 通过列表({}的数据)展开参数包
+ 可变参类模板： 类模板用到可变参数
  + 继承+模板特化的方式展开
  + 递归的方式展开参数包
+ 标准库中的可变参模板
  + 包含头文件：#include <tuple>
  + 如何创建
  + 如何获取数据
  + 其他操作

##### C++STL容器篇   

+ array:定长数组
+ vector：动态数组  
+ list
+ stack
+ queue/deque/priority_queue
+ initializer_list
+ bitset
+ set/multiset
+ map/multimap 

##### C++迭代器  

+ 迭代器是什么？ 用来访问容器一个桥梁 ，本质就是类中类的对象，去模仿指针行为

+ 按照定义方式分类:

  + 正向迭代器

    容器类名::iterator  迭代器名;

    begin();

    end();

  + 常正向迭代器

    容器类名::const_iterator  迭代器名;

    cbegin();

    cend()

  + 反向迭代器

    容器类名::reverse_iterator 迭代器名;

    rbegin();

    rend();

  + 常反向迭代器

    容器类名::const_reverse_iterator 迭代器名;

    crbgin();

    crend();

+ 按照功能分类
  + 正向迭代器
  + 双向迭代器
  + 随机访问迭代器

+ 所有容器的迭代器的分布情况

  | 容器                       | 迭代器       |
  | -------------------------- | ------------ |
  | array                      | 随机访问     |
  | vector                     | 随机访问     |
  | deque                      | 随机访问     |
  | list                       | 双向         |
  | set/multiset               | 双向         |
  | map/multimap               | 双向         |
  | stack/queue/priority_queue | 不支持迭代器 |

+ 迭代器的相关辅助函数
  + advance(iterator iter,size_t n);     等效指针的p+n操作
  + distance(iterator begin,iterator end); 检测距离
  + iter_swap(iterator first,iterator second); 交换first与second指向的元素

+ 流型迭代器(了解一下)
  + 输出流型迭代器
    + ostream_iterator<type> iter(ostreamObject); 
    + ostream_iterator<type> iter(ostreamObject,char* str);
    + iter=3;  实际含义是把3打印到屏幕上
  + 输入流型迭代器
    + istream_iterator<type> object;     //错误流, END_OF_STREAM;
    + istream_iterator<tyoe> object(istreamObject)
    + *object  等效于cin操作

+ copy算法

  copy(iterator begin,iterator end,iterator newBegin);

##### C++仿函数  

+ 仿函数什么?  仿函数就是类中的成员函数,这个成员函数可以让对象模仿函数调用的行为
  + 函数调用行为?  函数名(函数参数)  
  + C++中可以让类实现: 类名(函数参数) 调用函数

+ 自己写一个仿函数
  +  重载()运算符

+ 接触比较多的仿函数是两个排序准则: greater<type>(), less<type>()

##### C++函数适配器

+ bind函数

C++函数包装器

+ function 类

##### C++STL算法  

+ STL查找算法

  + 基本查找
    + find:区间查找
    + find_if:条件查找
    + find_firt_of: 查找区间第一次出现值
    + adjacent_find: 查找第一次重复的数
    + search:子序列查找
    + search_n: 子序列查找出现次数

  + 统计查找
    + count: 区间统计
    + count_if: 条件统计个数
    + equal:比较

  + 有序查找
    + binary_search：二分查找
    + upper_bound: 查找最后一个大于查找的值
    + lower_bound: 大于等于查找的值
    + equal_range:区间比较---有序序列

+ STL排序通用算法

  - merge: 归并排序，存于新容器
  - inplace_merge: 归并排序，覆盖原区间
  - sort: 排序，更改原容器顺序
  - stable_sort: 排序，保存原容器数据顺序
  - nth_element: 关键字排序
  - partition:范围排序
  - partial_sort:范围排序

  - partial_sort_copy:范围排序外加复制操作
  - stable_partition: 范围排序，保存原容器顺序

  - random_shuffle: 随机排序
  - reverse:逆序原容器
  - reverse_copy: 逆序容器保存到新容器

  - rotate:移动元素到容器末尾
  - rotate_copy:移动元素到新容器

+ STL删除替换算法
  + copy: 拷贝函数
  + copy_backward: 逆序拷贝
  + iter_swap: 交换
  + remove: 删除
  + remove_copy: 删除元素复制到新容器
  + remove_if:条件删除
  + remove_copy_if:条件删除拷贝到新容器
  + replace:替换
  + replace_copy: 替换，结果放到新容器
  + replace_if: 条件替换
  + replace_copy_if:条件替换，结果另存
  + swap: 交换
  + swap_range:区间交换
  + unique:去重
  + unique_copy:去重，结果另存

+ STL排列组合算法  休息到21:30

  + next_permutation:下一个排序序列的组合

  + prev_permutation:上一个排序序列的组合

+ STL 算术算法

  + accumulate:区间求和

  + partial_sum:相邻元素的和

  + inner_product:序列内积运算

  + adjacent_difference:相邻元素的差

+ STL 生成异变算法

  + for_each:迭代访问

  + fill:填充方式初始容器

  + fill_n:指定长度填充容器

  + generate_n:填充前n个位置

  + transform:一元转换和二元转换

+ STL 关系算法

  + equal:两容器元素是否都相同

  + includes:是否是包含关系

  + lexicographical_compare:比较两个序列

  + max:求最大值

  + max_element:返回最大值的iterator

  + min:求最小值

  + min_element:求最小值的iterator

  + mismatch:找到第一个不同的位置

+ STL 集合算法

  + set_union:差集

  + set_intersection:并集

  + set_difference:保存第一个中有第二个没有的元素

  + set_symmetric_difference:对称差集

+ STL堆算法

  + make_heap:生成一个堆

  + pop_heap:出堆

  + push_heap:入堆

  + sort_heap:堆排序

##### C++智能指针  

​	智能指针本质就是一个模板类，通过类的对象生命周期的自动结束效果，实现内存的自动释放。也就是把指针当做对象去处理，所以一般不会去new一个智能指针，自己new还是需要自己手动释放，一般都是创建一个智能指针对象，去管理堆区的变量。

+ shared_ptr
+ weak_ptr
+ unique_ptr

##### C++类型推断

+ auto
+ decltype

##### C++IO流   

+ 输入输出流
+ 字符流
+ 文件流

##### C++正则表达式 

正则是一种规则，它用来匹配（进而捕获、替换）字符串。这种规则需要“模式”、“字符串”这两样东西，“模式”根据正则规则，来处理“字符串”。这种规则被许多语言支持，C++11以后才支持正则。

###### 具有特殊意义的元字符

\：\字符能够改变字符原本的含义

^：^字符指示字符串的头，且要求字符串以字符开头，不占位。\^表示一个真正的^符号。

$：$字符指示字符串的尾，且要求字符串以字符结尾，不占位。\$表示一个真正的$符号。

()：分组，大正则中包含小正则。可以改变默认的优先级。在模式中可以使用\1来表示第一组已然捕获到的东西。

//ydpatjj@163.com

\b：指示字符串的边界（头/尾/空格左/空格右），字符\b要求边界的左边是字符，\b字符要求边界的右边是字符。

.：表示一个除了\n以外的任意一个字符。\\.表示一个真正的.符号。

|：a|b  a或b之一

[abc]：abc之中的任意一个

\[^abc]:  abc之外的

\[a-z]:	任意小写字母

\[^a-z]:  除了小写字母之外的

\w：任意一个字母数字下划线，等价于[(0-9)(a-z)(A-Z)(_)]

\W：字母数字下划线之外的，等价于[]

\d:	任意一个数子

\D:	除了数字之外的

\s:	空白符（空格、制表符、换页符）

###### 量词元字符

*：字符*要求字符出现0到多次   {0,}

+：字符+要求字符出现1到多次     (\w)  {1,} 

?：字符?要求字符出现0次或1次    {0,1}

{n}：字符{n}要求字符出现n次

{n,}：字符{n,}要求字符出现n到多次  {0,}

{n,m}：字符{n,m}要求字符出现n到m次、

所以含有`\`的元字符，在C++定义时，都要写成`\\`

###### 校验数字的表达式

数字：^ [0 - 9] * $
n位的数字：^ \d{ n }$
至少n位的数字：^ \d{ n, }$
m - n位的数字： ^ \d{ m,n }$
零和非零开头的数字： ^ (0 | \[1 - 9][0 - 9] *)$
非零开头的最多带两位小数的数字： ^ (\[1 - 9][0 - 9] *) + (.[0 - 9]{ 1,2 }) ? $
带1 - 2位小数的正数或负数： ^ (\ - ) ? \d + (\.\d{ 1,2 }) ? $
正数、负数、和小数： ^ (\ - | \ + ) ? \d + (\.\d + ) ? $
有两位小数的正实数： ^ [0 - 9] + (.[0 - 9]{ 2 }) ? $
有1~3位小数的正实数： ^ [0 - 9] + (.[0 - 9]{ 1,3 }) ? $
非零的正整数： ^ [1 - 9]\d * $ 或 ^ (\[1 - 9][0 - 9] *) { 1, 3 }$ 或^ \ + ? \[1 - 9][0 - 9] * $
非零的负整数： ^ \ - \[1 - 9][]0 - 9"*$ 或 ^-[1-9]\d*$
非负整数： ^ \d + $ 或 ^ [1 - 9]\d * | 0$
非正整数： ^ -[1 - 9]\d * | 0$ 或 ^ ((-\d + ) | (0 + ))$
非负浮点数： ^ \d + (\.\d + ) ? $ 或 ^ [1 - 9]\d * \.\d * | 0\.\d * [1 - 9]\d * | 0 ? \.0 + | 0$
非正浮点数： ^ ((-\d + (\.\d + ) ? ) | (0 + (\.0 + ) ? ))$ 或 ^ (-([1 - 9]\d * \.\d * | 0\.\d * [1 - 9]\d*)) | 0 ? \.0 + | 0$
正浮点数： ^ [1 - 9]\d * \.\d * | 0\.\d * [1 - 9]\d * $ 或 ^ (([0 - 9] + \.[0 - 9] * \[1 - 9][0 - 9] *) | ([0 - 9] * \[1 - 9][0 - 9] * \.[0 - 9] + ) | ([0 - 9] * \[1 - 9][0 - 9] *))$
负浮点数： ^ -([1 - 9]\d * \.\d * | 0\.\d * [1 - 9]\d*)$ 或 ^ (-(([0 - 9] + \.[0 - 9] * \[1 - 9][0 - 9] *) | ([0 - 9] * \[1 - 9][0 - 9] * \.[0 - 9]) | ([0 - 9] * \[1 - 9][0 - 9] *)))$
浮点数： ^ (-? \d + )(\.\d + ) ? $ 或 ^ -? ([1 - 9]\d * \.\d * | 0\.\d * [1 - 9]\d * | 0 ? \.0 + | 0)$

###### 校验字符的表达式

汉字： ^ [\u4e00 - \u9fa5]{ 0, }$
英文和数字： ^ [A - Za - z0 - 9] + $ 或 ^ [A - Za - z0 - 9]{ 4,40 }$
长度为3 - 20的所有字符： ^ .{3, 20}$
由26个英文字母组成的字符串： ^ [A - Za - z] + $
由26个大写英文字母组成的字符串： ^ [A - Z] + $
由26个小写英文字母组成的字符串： ^ [a - z] + $
由数字和26个英文字母组成的字符串： ^ [A - Za - z0 - 9] + $
由数字、26个英文字母或者下划线组成的字符串： ^ \w + $ 或 ^ \w{ 3,20 }$
中文、英文、数字包括下划线： ^ [\u4E00 - \u9FA5A - Za - z0 - 9_] + $
中文、英文、数字但不包括下划线等符号： ^ [\u4E00 - \u9FA5A - Za - z0 - 9] + $ 或 ^ [\u4E00 - \u9FA5A - Za - z0 - 9]{ 2,20 }$
可以输入含有 ^ %&',;=?$\"等字符：\[^%&', ; = ? $\x22] + 12 禁止输入含有~的字符：\[^ ~\x22] +

###### 特殊需求表达式

Email地址： ^ \w + ([-+.]\w + ) * @\w + ([-.]\w + ) * \.\w + ([-.]\w + ) * $
域名：\[a - zA - Z0 - 9][-a - zA - Z0 - 9]{ 0,62 }(/ .\[a - zA - Z0 - 9][-a - zA - Z0 - 9]{ 0,62 }) + / . ?
InternetURL：[a - zA - z] + ://[^\s]* 或 ^http://([\w-]+\.)+[\w-]+(/[\w-./?%&=]*)?$
手机号码： ^ (13[0 - 9] | 14[5 | 7] | 15[0 | 1 | 2 | 3 | 5 | 6 | 7 | 8 | 9] | 18[0 | 1 | 2 | 3 | 5 | 6 | 7 | 8 | 9])\d{ 8 }$
电话号码(0511 - 4405222、021 - 87888822)：\d{ 3 } - \d{ 8 } | \d{ 4 } - \d{ 7 }
身份证号(15位、18位数字)： ^ \d{ 15 } | \d{ 18 }$
短身份证号码(数字、字母x结尾)： ^ ([0 - 9]) { 7, 18 }(x | X) ? $ 或 ^ \d{ 8,18 } | [0 - 9x]{ 8,18 } | [0 - 9X]{ 8,18 } ? $
帐号：(字母开头，允许5 - 16字节，允许字母数字下划线)： ^ \[a - zA - Z][a - zA - Z0 - 9_]{ 4,15 }$
密码:(以字母开头，长度在6~18之间，只能包含字母、数字和下划线)： ^ [a - zA - Z]\w{ 5,17 }$
强密码(必须包含大小写字母和数字的组合，不能使用特殊字符，长度在8 - 10之间)：* ^ (? = .*\d)(? = .*[a - z])(? = .*[A - Z]).{8, 10}$
日期格式： ^ \d{ 4 } - \d{ 1,2 } - \d{ 1,2 }
一年的12个月(01～09和1～12)： ^ (0 ? [1 - 9] | 1[0 - 2])$
一个月的31天(01～09和1～31)： ^ ((0 ? [1 - 9]) | ((1 | 2)[0 - 9]) | 30 | 31)$
xml文件： ^ ([a - zA - Z] + -? ) + [a - zA - Z0 - 9] + \\.\[x | X]\[m | M][l | L]$
中文字符的正则表达式：[\u4e00 - \u9fa5]
双字节字符：\[^ \x00 - \xff](包括汉字在内，可以用来计算字符串的长度(一个双字节字符长度计2，ASCII字符计1))
空白行的正则表达式：\n\s * \r(可以用来删除空白行)
HTML标记的正则表达式：<(\S* ? )[^ > ] *>.* ? < / \1> | <.* ? / > (复杂的嵌套标记依旧无能为力)
首尾空白字符的正则表达式： ^ \s * | \s * $或(^ \s*) | (\s * $) (可以用来删除行首行尾的空白字符(包括空格、制表符、换页符等等))
腾讯QQ号：\[1 - 9][0 - 9]{ 4, } (腾讯QQ号从10000开始)
中国邮政编码：[1 - 9]\d{ 5 }(? !\d) (中国邮政编码为6位数字)
IP地址：\d + \.\d + \.\d + \.\d + (提取IP地址时有用)
IP地址：((? : (? : 25[0 - 5] | 2[0 - 4]\\d | [01] ? \\d ? \\d)\\.) { 3 }(? : 25[0 - 5] | 2[0 - 4]\\d | [01] ? \\d ? \\d))

##### C++日期时间库 

+ 时间间隔 duration
+ 时钟 clock()
+ 时间点 time point
+ 时间转换

##### 时间间隔duration

duration表示一段时间间隔，用来记录时间长度，可以表示几秒、几分钟、几个小时的时间间隔,

```c++
template <class _Rep, class _Period = ratio<1>>
class duration;
/*
ratio 代表的是一个分子除以分母的数值
ratio<1,1000000000>     纳秒
ratio<1,1000000> 		微秒
ratio<1,1000>	   		毫秒
ratio<60*60*24>			一天
ratio<60*60>			小时
ratio<60>				分钟
ratio<1>				秒
*/
```

为了方便使用，在标准库中定义了一些常用的时间间隔时、分、秒、毫秒、微秒、纳秒

| 类型                              | 定义                       |
| --------------------------------- | -------------------------- |
| 纳秒：`std::chrono::nanoseconds`  | duration<long long, nano>  |
| 微秒：`std::chrono::microseconds` | duration<long long, micro> |
| 毫秒：`std::chrono::milliseconds` | duration<long long, milli> |
| 秒：`std::chrono::seconds`        | duration<long long>        |
| 分钟：`std::chrono::minutes`      | duration<int, ratio<60>>   |
| 小时：`std::chrono::hours`        | duration<int, ratio<3600>> |

```c++
#include <chrono>
#include <iostream>
#include <thread>
using namespace std;
int main() 
{
	this_thread::sleep_for(std::chrono::seconds(1));
	cout << "1秒后" << endl;
	return 0;
}
```

时间运算

```c++
#include <chrono>
#include <iostream>
#include <thread>
using namespace std;
int main() 
{
	this_thread::sleep_for(std::chrono::seconds(1));
	cout << "1秒后" << endl;
	std::chrono::seconds  test(100);
	cout << test.count() << endl;
	std::chrono::hours  h(1);
	chrono::seconds result = h - test;
	cout << result.count() << endl;
	//cout << result << endl;
	return 0;
}
```

##### 时钟clocks

###### system_clock

系统范围的实时时钟,对当前时间点 time_point 的访问，将得到时间点转换为 time_t 类型的时间对象，就可以基于这个时间对象获取到当前的时间信息了

```c++
//返回表示当前时间的时间点。
static std::chrono::time_point<std::chrono::system_clock> now() noexcept;
//将 time_point 时间点类型转换为 std::time_t 类型
static std::time_t to_time_t( const time_point& t ) noexcept;
//将 std::time_t 类型转换为 time_point 时间点类型
static std::chrono::system_clock::time_point from_time_t( std::time_t t ) noexcept;
```

获取当前的系统时间:

```c++
//http://www.cplusplus.com/reference/chrono/time_point/time_point/
#include <chrono>
#include <iostream>
#include <thread>
#include <iomanip>
#include <ctime>
using namespace std;
int main() 
{
	chrono::system_clock::time_point result;
	chrono::duration<int, ratio<60 * 60 * 24>> day(1);
	//1970.1.1时间 + 1天
	chrono::system_clock::time_point ppt(day); 
	//1970.1.1时间 + 10天
	chrono::system_clock::time_point t(chrono::duration<int, ratio<60 * 60 * 24>>(10));
	//系统当前时间
	chrono::system_clock::time_point tp = chrono::system_clock::now();
	//转换为time_t类型数据
	time_t tm = chrono::system_clock::to_time_t(tp);
	cout << "今天日期:" << ctime(&tm);
	std::tm* p = localtime(&tm);
	cout <<"格式化输出时间:" << put_time(p, "%F %X") << endl;
	cout <<"count:" << tp.time_since_epoch().count() << endl;
	return 0;
}
```

###### steady_clock

程序耗时的时长，此时使用 syetem_clock 就不合适了，因为这个时间可以跟随系统的设置发生变化。在 C++11 中提供的时钟类 steady_clock 相当于秒表，只要启动就会进行时间的累加，并且不能被修改，非常适合于进行耗时的统计。

计算程序执行期间消耗的总时长

```c++
#include <chrono>
#include <iostream>
#include <thread>
#include <iomanip>
#include <ctime>
using namespace std;
int main() 
{
	chrono::steady_clock::time_point start = chrono::steady_clock::now();
	cout << "start:" << endl;
	for (int i = 0; i < 1000; ++i) 
	{
		cout << "运行中...." << endl;
	}
	chrono::steady_clock::time_point end = chrono::steady_clock::now();
	auto dt = end - start;
	cout << "耗时:" << dt.count() << "ns" << endl;

	return 0;
}
```

###### high_resolution_clock

high_resolution_clock 提供的时钟精度比 system_clock 要高，它也是不可以修改的。在底层源码中，这个类其实是 steady_clock 类的别名。

```c++
#include <chrono>
#include <iostream>
#include <thread>
#include <iomanip>
#include <ctime>
using namespace std;
int main() 
{
	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
	cout << "start:" << endl;
	for (int i = 0; i < 1000; ++i) 
	{
		cout << "运行中...." << endl;
	}
	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
	auto dt = end - start;
	cout << "耗时:" << dt.count() << "ns" << endl;
	return 0;
}
```

##### **时间转换函数**

###### duration_cast

duration_cast 是 chrono 库提供的一个模板函数，这个函数不属于 duration 类。通过这个函数可以对 duration 类对象内部的时钟周期 Period，和周期次数的类型 Rep 进行修改

浮点时长和整数时长间转型能隐式进行无需使用 duration_cast ，其他情况下都需要通过函数进行转换。

```c++
#include <chrono>
#include <iostream>
#include <thread>
#include <iomanip>
#include <ctime>
using namespace std;
int main() 
{
	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
	cout << "start:" << endl;
	for (int i = 0; i < 1000; ++i) 
	{
		cout << "运行中...." << endl;
	}
	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
	//整数时长：要求 duration_cast
	auto dt = chrono::duration_cast<chrono::milliseconds>(end - start);
	cout << "耗时:" << dt.count() << "ms" << endl;
	//小数时长：不要求 duration_cast
	chrono::duration<double, ratio<1, 1000>> dms = end - start;
	cout << "耗时:" << dt.count() << "ms" << endl;
	return 0;
}
```

###### time_point_cast

time_point_cast 也是 chrono 库提供的一个模板函数，这个函数不属于 time_point 类。函数的作用是对时间点进行转换，因为不同的时间点对象内部的时钟周期 Period，和周期次数的类型 Rep 可能也是不同的，一般情况下它们之间可以进行隐式类型转换，也可以通过该函数显示的进行转换，函数原型如下：

```c++
#include <chrono>
#include <iostream>
using namespace std;

template<class Duration>
void print_ms(const chrono::time_point<chrono::high_resolution_clock, Duration>& time_point)
{
    std::cout << time_point.time_since_epoch().count() << " ms\n";
}
int main()
{
    chrono::time_point<chrono::high_resolution_clock, chrono::milliseconds> time_point_sec(chrono::seconds(6));
    // 无精度损失, 可以进行隐式类型转换
    chrono::time_point<chrono::high_resolution_clock, chrono::milliseconds> time_point_ms(time_point_sec);
    print_ms(time_point_ms);    // 6000 ms

    time_point_ms = chrono::time_point<chrono::high_resolution_clock, chrono::milliseconds>(chrono::milliseconds(6789));
    // error，会损失精度，不允许进行隐式的类型转换
    //chrono::time_point<chrono::high_resolution_clock, chrono::seconds> sec(time_point_ms);
    // 显示类型转换,会损失精度。6789 truncated to 6000
    time_point_sec = std::chrono::time_point_cast<chrono::seconds>(time_point_ms);
    print_ms(time_point_sec); // 6000 ms
}
```

**注意事项**：关于时间点的转换如果没有没有精度的损失可以直接进行隐式类型转换，如果会损失精度只能通过显示类型转换，也就是调用 time_point_cast 函数来完成该操作

##### C++文件系统filesystem   

filesystem主要用来处理文件目录，也就是文件夹创建和删除，遍历，不是用来读写文件

###### path类：

路径处理

###### file_status类

文件状态一些判断

###### directory_entry类

文件路口

###### directory_iterator类

文件目录的遍历

filesystem中成员函数

```c++
//判断path是否存在  不存在返回false 存在返回true
bool exists(file_status stat) noexcept;
bool exists(const path& pval);
bool exists(const path& pval, error_code& ec) noexcept;
//构建目录的  只能构建一层目录
bool create_directory(const path& pval);
bool create_directory(const path& pval, error_code& ec) noexcept;
bool create_directory(const path& pval, const path& attr);
bool create_directory(const path& pval, const path& attr, error_code& ec) noexcept;
//构建目录，可以构建 a/b/c 这种目录
bool create_directories(const path& pval);
bool create_directories(const path& pval, error_code& ec) noexcept;
```

##### C++网络基础   20:05 开始

 [C++网络基础.pdf](C++网络基础.pdf) 

**了解计算机网络**

**了解ip地址**

![](C:\Users\ASUS\Pictures\博客图片\image-20211102134143168(1).png)

| ip地址分类 | 范围                      |
| ---------- | ------------------------- |
| A类 (0)    | 0.0.0.0~127.255.255.255   |
| B类(10)    | 128.0.0.0~191.255.255.255 |
| C类(110)   | 192.0.0.0~223.255.255.255 |
| D类(1110)  | 224.0.0.0~239.255.255.255 |
| E类(11110) | 240.0.0.0~247.255.255.255 |

回送地址(本机测试地址): 127.0.0.1 这个ip一般用来测试本地的服务和和客户端程序

查看本机的ip地址: ipconfig

**了解域名**

域名是ip地址的另一种表示方式

cmd:Nslookup 把域名解析为ip地址

**了解端口号**

常用端口： SQL server 1433   MySQL  3306

**OSI模型**

 ![image-20211102205432930](C:\Users\ASUS\Pictures\博客图片\image-20211102144608863(1).png)

应用层:  老板

表示层: 老板的助理

会话层:  给助理跑腿

传输层: 传达信息到位

网络层: 邮局来拿水果的人

数据链路层: 打包水果的人

物理层: 具体运输到客户

**SOCKET**

头文件：

```C++
#include <winsock2.h>
#pragma comment(lib,"WS2_32.lib")
```

初始化套接字:



1.WSAStartup() 

2.closesocket();

2.WSACleanup();









