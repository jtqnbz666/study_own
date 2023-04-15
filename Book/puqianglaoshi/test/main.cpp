#include <iostream>
#include "sub.h"
#include "add.h"

using namespace std;
int main() {
    int c;
    int a, b;
    cout << "规则:" << "输入1 为加法， 2 为减法!!" << endl;
    while(cin >> c) {
        cout << "请输入两个数";
    switch(c) {
       case 1 : cin >> a >> b;add(a, b); break;
       case 2 : cin >> a >> b;sub(a, b); break;
       
       }
    
    }
    return 0;
}
