~~~c++
#include <iostream>
using namespace std;

class UnionSet {
  UnionSet(int n) n : (n) {
      father = new int[n + 1];
      cnt = new int[n + 1];
      for(int i = 0; i < n; i  ++) {
          father[i] = i;// 最开始， 自己就是自己的老板
          cnt[i] = 1;
      }
  }
  int find(int x) {
      return father[x] = (father[x] == x ? x : find(father[x]))
          //比如return  a = 9; 返回的就是9
  }  
  void merge(int x, int y) {
      int fa = find(x), fb = find(y);
      if(fa == fb) return;
      cnt[fb] += cnt[fa];
      father[fa] = fb; 
  }
int *father, *cnt;
int n ;
};
~~~



547省份数量

200 岛屿数量 (把二维数组转化为一维数组)

990等式方程的可满足性

684冗余连接

128最长连续序列 （用到了hash)，比如查3的时候就判断 2 和 4是否出现过，merge下标

947移除最多的同行或同列石头：借助两个哈希表，连通完成后，用总石头数减去当前集合数=要移除的石头数

1202.交换字符串中的元素 ： 维护n个小顶堆( priority_queue<char, vector<int>,  greater<int>) ph[n] )，遍历一遍数组，把能够连通的节点放入同一个小顶堆中， 出队的时候，每次弹出最小值