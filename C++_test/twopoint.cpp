#include <iostream>
using namespace std;



int main() {
    int a = 6;
    int *t = &a;
    int **tt = &t;
    cout << "t:" << t << "&t:" << &t << " tt:" << tt;

    return 0;
}
