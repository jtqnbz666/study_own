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



lc95不同的搜索树II，需要列出每一种情况

思路：和lc96(不同的搜索树)结合理解 , 与**括号生成**那题有相似之处，思考方法很有意思    

对于括号生成左右两边都是括号，没有特殊区分，但对于搜索树，区分了节点号，所以不能像括号生成那样做。比如说3个节点，对于括号生成来说，不管取那三个节点的括号得到的结果都一样，而对于搜索树来说，你可以选123，456，789 ... 它们生成的树状结构虽然一样，但具有编号

比如以三个节点为例子(n == 3)

```c++
for(int i = 1; i  <= 3; i ++) {
vector<int>l = dfs(0, i - 1);//第一次循环会返回一个空节点 
vector<int>r = dfs(i + 1, 3);//第一次循环会返回2种情况,一种2为根,一种3为根
}
//这三个节点遍历完就可以得到5种情况
```

完整代码

```c++
class Solution {
public:
    vector<TreeNode*> dfs(int l, int r){
        vector<TreeNode*>ans;
        if(l > r) {
            ans.push_back(nullptr);
            return ans;
        }
        for(int i = l; i <= r; i ++) {
            vector<TreeNode*>left = dfs(l, i - 1);
            vector<TreeNode*>right = dfs(i + 1, r);
            for(auto v1 : left) {
                for(auto v2 : right) {
                    TreeNode* node = new TreeNode(i, v1, v2);
                    ans.push_back(node);
                }
            }
        }
        return ans;
    }
    vector<TreeNode*> generateTrees(int n) {
        return dfs(1, n);
    }
};
```

