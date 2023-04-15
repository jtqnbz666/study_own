#include <bits/stdc++.h>
using namespace std;
int flag = 0;
class Queue {
    public:
    Queue(int n) : arr(new int[n]), head(0), tail(0), capcity(n), cnt(0) {}
    void push(int val) {
        if(full()) {
            cout << "队列满了 " << endl;
            return ;
        }
        tail = tail % 10;
        arr[tail] = val; 
        tail ++;
        cnt ++;
        
    }
    int pop() {
        if(empty()) {
            cout << "队列为空" << endl;
            return 0;
        }
        head %= 10;
        cnt --;
        return arr[head++];
    }
    bool full() {
        if(cnt == capcity) {
            return true;
        }
        return false;
    }
    bool empty() {
        if(cnt == 0) {
            return true;
        }
        return false;
    }
    int front(){ 
        return arr[head];
    }
    int Size() {
        return cnt;
    }
    void out() {
        
        for(int i = 0, k = head; i < cnt; i ++ ) {
            cout << arr[k] << " ";
            k ++;
            if(k == capcity) k = 0;
        } 
        
        cout << endl;
    }
    int head;
    int tail;
    int *arr;
    int capcity;
    int cnt;
};
int main() {
    Queue mq(10);
    string op;
    int val;
    
    while(cin >> op >> val) {
       if(op == "push") {
            mq.push(val);
            mq.out();
       }
       if(op == "front") {
            cout << mq.front() << endl;
            mq.out();
       }
       if(op == "size") {
            cout << mq.Size() << endl;
            mq.out();
       }
       if(op == "pop") {
            cout << mq.pop() << endl;
            mq.out();
       }
    }
    return 0;
}