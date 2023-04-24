#include <iostream>
using namespace std;
class Fa {
public:
    //如果去掉这个virtual， 下方dynamic进行下行转换报错
	virtual void hha() {
		cout << "fa_hha" << endl;
	}
	
};

class Son : public Fa {
public:
	void hha () {
		cout << "son_hha" << endl;
	}
	void test() {
		cout << "son_test" << endl;
	}
    int ss = 10; //如果子指父， 那么会出问题，因为父类没有这个对象空间
};
int main() {
    
//1.子指父
Fa *fa = new Fa();
Son* s = (Son*)fa;	//子转父，必须显式转换，不然报错
//Son *s = (Son*)new Fa();
Son* s1 = static_cast<Son*>(fa);
    
//如果Fa中没有虚函数，下边这行会报错，而static_cast不会
//因为dynamic_cast为动态类型检查，需要检查运行时需要的信息，也就是虚表
Son* s2 = dynamic_cast<Son*>(fa);
s->hha();	//fa_hha，说明子转父这种情况用的是父类的虚表
s->test();	//son_test
cout << s->ss << endl; // 0  这里可以看出来出问题了，因为父类没有ss空间
cout << s1->ss << endl; //0
cout << s2->ss << endl; //编译不会出错，运行时段错误
static_cast<Fa*>(s)->hha();//fa_hha
//static_cast<Fa*>(s)->test(); //报错，因为Fa类中没有test
//所以子类指向父类是不安全的。

Son* s3 = new Son();
cout << s3->ss << endl; // 10；
    
    
//2.父指子
Fa* ff = new Son();
static_cast<Son*>(ff)->hha(); //son_hha
static_cast<Son*>(ff)->test();//son_test
Fa* f1 = static_cast<Fa*>((Son*)fa);
Fa* f2 = dynamic_cast<Fa*>((Son*)fa);
return 0;
}