#include <iostream>
using namespace std;

struct Node{
    int expire;
    int id;
};

bool operator<(const Node& lht, const Node& rht) {
    if(lht.expire == rht.expire) {
        return lht.id < rht.id;
    }
    return lht.expire < rht.expire;
}

template<class _Ty>
class Myless{
public:
    bool operator()(const _Ty& a, const _Ty& b) {
        return a < b;
    }
};

template<class _Ty, class _Pr = less<>>
void BublleSort(_Ty arr[], int size) {
    for(int i = 0; i < size; i ++) {
        for(int j = 0; j < size - 1 - i; j ++) {
            if(!_Pr()(arr[j], arr[j + 1])) {
                _Ty tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int main() {
int arr[5] = {1,3,2,4,9};
BublleSort<int, Myless<int>>(arr, 5);
Node arr1[3];
for(int i = 0; i < 3; i ++) {
    arr1[i].expire = 100 / (i + 1);
    arr1[i].id = i;
}
BublleSort<Node, Myless<Node>>(arr1, 3);
for(auto v : arr) {
    cout << v << '\t';
}

cout << endl;
for(auto v : arr1) {
    cout << v.expire << '\t';
}
    return 0;
}
