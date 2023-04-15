#include <iostream>
#include <stdio.h>
using namespace std;

int &test() {
	int a = 10;
	return a;
}
int main()
{
	int b = test();
	cout << b << endl;
	return 0;
}

