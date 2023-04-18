#include <bits/stdc++.h>
using namespace std;
#define MAX_N 1000
int arr[MAX_N + 5], cnt = 0;

void push(int x) {
    arr[cnt ++] = x;
    int idx = cnt - 1;
    int tmp = arr[idx];
    while(idx > 0 && arr[(idx - 1) / 2] < tmp) { //idx>0则一定有父，因为以0为下标开始的
        arr[idx] = arr[idx / 2];
        idx = (idx - 1) / 2;
    }
    arr[idx] = tmp;
}
int size() {
    return cnt;
}

void pop() {
    if(size() == 0) return;
    arr[0] = arr[cnt - 1];
    int tmpval = arr[0];
    int idx = 0;
    cnt --;
    while(idx * 2 + 1 <= cnt - 1) { // 保证一定有子节点
        int tmpidx = idx;
        if(arr[idx * 2 + 1] > arr[tmpidx]) tmpidx = tmpidx * 2 + 1;
        if((idx * 2 + 2) <= cnt - 1 && arr[idx * 2 + 2] > arr[tmpidx]) tmpidx += 1;
        if(tmpidx == idx) break;
        arr[idx] = arr[tmpidx];
        idx = tmpidx;
    }
    arr[idx] = tmpval;
}

int top(){
    return arr[0];
}

int main() {
    int op, val;
    while(cin >> op) {
        switch(op) {
            case 1: {
                cin >> val; push(val);
            }break;
            case 2: {
                cout << "top:" << top();
                pop();
            }break;
        }
       
    }

}