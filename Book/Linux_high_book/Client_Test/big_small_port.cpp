#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
using namespace std;
void test(){
    union{
        short val;
        char union_bytes[sizeof(short)];
    } test;
    test.val = 0x0102;
    if(test.union_bytes[0] == 1 && test.union_bytes[1]==2){
        cout << "big endian!" << endl; //网络字节序
    } else if (test.union_bytes[0] == 2 && test.union_bytes[1] == 1){
        cout << "small endian !" << endl;//主机字节序
    } else {
        cout << "error！ " << endl;
    }
}
int main(){
    test();
    return 0;
}
