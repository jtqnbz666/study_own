#include <iostream>
using namespace std;

template<class _Ty>
class Myvector {
public:
    Myvector() {
        memory = new _Ty[10];
    }
    Myvector(int n) {
        size = n;
        memory = new _Ty[size];
    }
    _Ty& operator[] (int idx) {
        return memory[idx];
    }
    class iterator {
        public:
        iterator(_Ty* pMove = nullptr) : pMove(pMove) {}
        void operator= (iterator object) {
            this->pMove = object.pMove;
        }
        bool operator!= (iterator object) {
            return this->pMove != object.pMove;
        }
        iterator& operator++ (int) {
            this->pMove++;
            return *this;
        }
        _Ty operator* () {
            return *this->pMove;
        }
        
        _Ty* pMove;
    };
    iterator begin() {
        return iterator(memory);
    }
    iterator end() {
        return iterator(memory + size);
    }
    int size;
    _Ty* memory;
};

int main() {
Myvector<int> dp(5);
for(int i = 0; i < 5; i ++) {
    dp[i] = i;
}
Myvector<int>::iterator iter = dp.begin();
for(iter; iter != dp.end(); iter++) {
    cout << *iter << '\t';
}


return 0;
}
