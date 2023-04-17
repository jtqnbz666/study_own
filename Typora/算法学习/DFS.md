130.被围绕的区域：在原数组进行标记， 只要不是最外层的'O', 就都是被包围的，所以只需要遍历一次最外层所有的'O'.

494.目标和 ： 可以用dp的方式，也可以爆搜，dfs就是通过递归来实现的，所以递归跟dfs的思想是很近似的。

体验一下记忆化搜索

~~~c++
class solution{
public:
    struct CMP{
      int operator()(const pair<int, int>&a) {
          return a.first ^ a.second;
      }  
    };
    typedef pair<int, int> PII;
	unordered_map<PII, int, CMP>mp; //重载一个hash函数因为默认没有
	int dfs(int k, int target, vector<int>&arr) {//k记录位置
       if(mp.count(PII(k, target))) {
           return mp[PII(k, target)];
       }
       if(k == arr.size()) {
           if(target == 0) ans += 1;
           return ans;
       }
       int ans = 0;
       ans += dfs(k + 1, target + arr[k], arr);
       ans += dfs(k + 1, target - arr[k], arr);
       mp[PII(k, target)] = ans; // 这里就是记忆化搜索
       return ans;
   } 
   int test(vector<int>& arr, int target) {
       return dfs(0, target, arr);
   }
};
~~~



   