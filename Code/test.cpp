#include <iostream>
using namespace std;
class A {
	public:
	virtual void test() {}
	A(){}
	int a;
};
class B : public  A{
	public:
	B() {}
};
int main() {
	// B b;
	// A* a = dynamic_cast<A*>(&b);

	A a;
	B* b = dynamic_cast<B*>(&a);
	cout << b;
	return 0;
}