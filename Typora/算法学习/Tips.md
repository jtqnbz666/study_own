1.string 中的find函数

~~~c++
#include <iostream>
#include <string>
using namespace std;
int main()
{
    string aa="+-*/";
    string bb="+";
   	if(aa.find(bb))//注：如果没有在aa中找到bb那么返回的结果是-1，if条件仍然满足，所以如果这样写的话，就算没找到if条件也是满足的（因为不是0）
        if(aa.find(bb)>=0)//也是不正确的
      //  if(aa.find(bb)!=-1)或者if(aa.find(bb)!=npos)才正确
           
  //static const size_t npos = -1;           
 //string::npos的值是-1并不准确，string::npos的值是无符号型类型的，其值是(unsigned int)(-1)，也就是4294967295，但是和int 类型的-1比较结果是true,所以使用的时候，认为其值是int类型的-1不会影响结果.
}
~~~

2.c++中排序＋去重

~~~c++
   	sort(arr.begin(), arr.end());
    auto it=unique(arr.begin(), arr.end());
    arr.erase(it, arr.end());

arr.erase(unique(arr.begin(),arr.end()),arr.end());
~~~



3实现int和string类型的转换

```C++
#include <iostream>
#include <sstream>
using namespace std;
int main()
{
	stringstream stream;//创建一个字符流对象
	int n;
	string str;
	stream << "123";
	stream >> n;
	cout << n << endl;
	stream.clear();		//这步很关键，每一次使用后都要清空。
	stream.str(" ");//这句话也是清空的作用。
	stream << 123;
	stream >> str;
	cout << str.c_str();
	
	
    	//还有一种方式可以将String 或者 const char *类型转换为int型
    //使用了atoi()函数。参数类型必须是const char*类型。
	const char* s = "1234";
	string Str("666");
	int intstr = atoi(Str.c_str());
	int ints = atoi(s);
	cout << intstr << "\t" << ints;

	system("pause");
	return 0;
	//string-->int
         stoi();
}
} 
         //int --->  string.
//1.itoa()函数有3个参数：第一个参数是要转换的数字，第二个参数是要写入转换结果的目标字符串，第三个参数是转移数字时所用的基数。
//2.string str=to_string(int x);
```

3.使用sscanf和sprintf实现

```c++
 char str[] = "15.455";
     int i;
     float fp;
     sscanf( str, "%d", &i ); // 将字符串转换成整数 i = 15
     sscanf( str, "%f", &fp ); // 将字符串转换成浮点数 fp = 15.455000

如果要写跨平台的程序，请用sprintf。是Windows平台下扩展的，标准库中有sprintf，功能比这个更强，用法跟printf类似：
char str[255];
sprintf(str, “%x”, 100); //将100转为16进制表示的字符串。


//输出8进制 = 014
printf("%o",number);

```

4.大写小写字符的转换函数

~~~c++
//大写转小写
string s="Hellword";
tolower(s[0]);
//小写转大写
toupper(s[0]);
~~~

5.substr的两种用法

~~~c++
#include<iostream>
using namespace std;
int main()
{
   string str="hello Word";
   str.substr(0,2);//从0开始截取两个；
   str.substr(1);//从第二个(下标1)开始截取后边所有。
}
~~~

6.vector 将数组复制一遍

~~~c
  vector<int> nextGreaterElements(vector<int>& nums) {
        // 拼接一个新的nums
        vector<int> nums1(nums.begin(), nums.end());
        nums.insert(nums.end(), nums1.begin(), nums1.end());
~~~

7. vector<string>数组初始化

   ~~~c
   vector<string>chess(n, string(n, '.'));
   //注意这是一维数组，但因为是string类型，所以可以这样。
//可以把string类型的内部理解为多个char元素
   vector<vector<int>>chess2(n, vector<int>(n, 0));
   //二维整型地图。
   ~~~
   
   

8.在一个3*3矩阵找是否重复

~~~c
int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = startRow; i < startRow + 3; i++) { // 判断9方格里是否重复
        for (int j = startCol; j < startCol + 3; j++) {
            if (board[i][j] == val ) {
                return false;
            }
        }
    }
//val是要找的数字，
~~~

9.算法中遇到用hash计数时，指定排序方法，需要先将hash存储到vector中再对vector进行指定排序，参考leetcode 501（二叉搜索树中众数），牛客(nc97  TOPk问题）, leetcode 332(重新安排行程)

~~~C
 vector<vector<string> > topKstrings(vector<string>& strings, int k) {
        map<string, int>kk;
        for(int i = 0; i < strings.size(); i ++) {
            kk[strings[i]] ++;
        }
        vector<pair<string, int>>vec(kk.begin(), kk.end());
        sort(vec.begin(), vec.end(), [&](const pair<string, int>&A, const pair<string, int>&B) {
        if(A.second == B.second) return A.first < B.first;
        return A.second > B.second;
    });
        vec.resize(k);
        vector<vector<string>>tt;
        for(auto iter = vec.begin(); iter != vec.end(); iter ++) {
            tt.push_back({iter->first, to_string(iter->second)});
        }
        return tt;
    }
~~~

10. dp中的几种情况

~~~c
求有几种方法： dp[j] += dp[j - coins[i]]; //需要初始化dp[0] = 1;
求最大能装的物品个数: dp[j] = max(dp[j], dp[j - coins[i]] + 1);	
// 上下两种方法都需要初始化dp[0] = 0,其他位置初始化为很大的数；
求最小能装的物品个数: dp[j] = min(dp[j], dp[j - coins[i]] + 1);
求能装的最大价值: dp[j] = max(dp[j], dp[j - w[i]] + value[i]);
~~~

11.容器的使用

~~~c
map，unordered_map ， vector可以用auto来遍历

对于priority_queue<int, vector<int>, cmp> 
push入， pop出， top()取顶

对于map:
用count 和 find(键) != end()查找是否存在

map.erase() //可以是iter迭代器，也可也是主键

map.insert(make_pair(A,B));

对于queue
front访问对头， back访问队尾， size 求大小，empty判空， push加入队尾，pop弹出队首

queue是一个适配器， 底层是deque，deque的实现既不是单纯的数组，也不是链表。

对于vector
队尾back()， 队首front(), push_back, pop_back()

对于reverse函数需要注意
reverse(idx1, idx2) ,idx2是没有算的，左闭右开
~~~

12.string 类型的初始化

~~~c

string field(field_reply->str, field_reply->len);
string value(value_reply->str, value_reply->len);
//这个ret_value是map
ret_value.insert(make_pair(field, value));

//这个ret_value 是string，追加，在原来的基础上增加
ret_value.append(reply->str, reply->len);
~~~

13.快排使用场景总结 

~~~c
若是在数组中，就用最中间的数字来分割即可
若在链表中，就遍历链表，找到最大值和最小值，求出平均数，以平均数来分割
~~~

14.二维数组输入

~~~c++
char arr[5][10];
	int n, m;
	while ((cin >> n >> m && n != EOF)) {
		memset(arr, 0, sizeof(arr));
		for (int i = 0; i < n; i++) {
			cin >> arr[i];
		}
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				cout << arr[i][j];
			}
		}
	}
	
	/*int n = 0;
	cin >> n;
	vector<string>tmp(n);
	for (int i = 0; i < n; i++) {
		cin >> tmp[i];
	}
	for (auto v : tmp) {
		cout << v << endl;
	}*/
~~~

15.进制转换

~~~c++
#include <iostream>
using namespace std;


int main() {
	// 十进制转二进制
	/*int n;
	cin >> n;
	int arr[100];
	int i = 0;
	while (n) {
		arr[i++] = n % 2;
		n /= 2;
	}
	for (int j = i - 1; j >= 0; j--) {
		cout << arr[j];
	}*/

// 2进制转10进制
	/*string b_str;
	cin >> b_str;
	int val = 0;
	for (int i = b_str.size() - 1; i >= 0; i--) {
		val += (b_str[i] - '0') * pow(2, b_str.size() - i - 1);
	}
	cout << val;*/

	//16进制转10进制
	/*string h_str;
	cin >> h_str;
	int val = 0;
	for (int i = h_str.size() - 1; i >= 0; i--) {
		if (h_str[i] >= 'A' && h_str[i] <= 'F') {
			val += (h_str[i] - 'A' + 10) * pow(16, h_str.size() - i - 1);
		}
		else {
			val += (h_str[i] - '0') * pow(16, h_str.size() - i - 1);
		}
		
	}
	cout << val;*/
	

	//10进制转16进制
	/*int n;
	cin >> n;
	char arr[100];
	int i = 0;
	while (n) {
		arr[i++] = n % 16 >= 10 ? n % 16 - 10 + 'A' : n % 16 + '0';
		n /= 16;
	}
	for (int j = i - 1; j >= 0; j--) {
		cout << arr[j];
	}*/
	return  0;
}
~~~

16.vector数组定义理解

~~~c++
若是一维数组 vector<int>tmp， 具体类型自己定
若是二维数组 vector<vector<int>>tmp(n, vector<int>(n, 0));
对于二维数组，我们在使用的时候都是tmp[i][j], 类型同样是自己定。
vector<vector<int>>tt(3);
    tt.push_back({1,2,3});
    tt.push_back({1,2,3});
    cout << tt[0].size() << endl;  //0
    cout << tt[0].size(); // 3
    for(auto v : tt) {
        for(auto v1 : v) {
            cout << v1 << " ";
        }
        cout << endl;
    }
//打印结果
0
3


1 2 3 
1 2 3 
~~~

17.max不能在里面放其他函数

~~~c++
正确：
int k = ((Max - 1) * (n + 1) + m);
int k2 = tasks.size();
return max(k, k2);

错误：
return max((Max - 1) * (n + 1) + m, tasks.size());
~~~

