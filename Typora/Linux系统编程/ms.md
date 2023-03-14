



**linux三剑客** awk(利用正则做一些操作)  grep(查询文本信息) sed(替换文本信息)

 netstat  通过端口号查一些信息，或者服务名查端口号

lsof -i:4369  通过端口查看这个进程的相关信息 

~~~
（文本统计）
b.txt 内容
http://www.baidu.com/index.html
https://www.atguigu.com/index.html
http://www.sina.com.cn/1024.html
https://www.atguigu.com/2048.html
http://www.sina.com.cn/4096.html
https://www.atguigu.com/8192.html



cat b.txt | cut -d '/' -f3 | sort | uniq -c | sort -nr
# cut -d "/" -f3  用"/"作为分隔符，截取第个3字段
# sort 第一次排序
# uniq -c 显示该行重复次数
# sort -nr 按照数值(n)从大到小排(r逆序)

最终结果
3 www.atguigu.com
2 www.sina.com.cn
1 www. baidu.com


（ ip 连接数统计并排序）
netstat -an | grep ESTABLISHED | awk '{print $5}' | cut -d ":" -f1 | sort -n | uniq -c | sort -nr
~~~



ps -aux | wc -l  统计进程数量



awk

~~~shell
test.txt内容, 124为空行
1  
2
3  ilovehh
4
5  you support

打印test.txt文件内容的每一行的最后一个字符串，如果只有一个字符串，那么就直接打印出来
awk '{print $NF}' test.txt # ${NF-1}则是倒数第二个
#ilovehh 
#support

输出每行数据的列数 
awk '{print NF}' # 0 0 1 0 3 每行一个数字

输出每行数据的行号
awk '{print NR}' # 1 2 3 4 5 每行一个数字

打印每行全部内容 
awk '{print $0}' #'{print $1}'每行第一列内容

定义变量
awk -v x="jt" -v y=666 '{print x, y}'


常用命令
cat test.txt | awk 'NF%2==0' #过滤掉奇数行


~~~





scp命令

```
scp * -r ubuntu@49.234.61.220:/home/ubuntu
```



静态库编译：

```c
g++ -c add.cpp
ar rc libadd.a add.o
g++ main -L. -ladd
```

动态库编译

```C
g++ -fPIC -shared add.cpp -o libadd.so
cp libadd.so /usr/lib
g++ main -L. -ladd
```



多线程gdb调试

```c
b ：break 断点
方式1： b + 行号，方式2 b + 函数名
方式3: 若需要断点的地方不在主源文件，而在某个其他源文件中， 比如需要在其他源文件的 void test(int a);
这一行断点， 则直接 b void test(int )即可(我发现含有IM::BaseDefine等作用域参数的函数不能使用这个方法)
方式4：同样是在其他文件中， b test.cpp:22   即表示在test.cpp中22行断点

d ： disable  清除断点 
d + 具体断点数字

c : continue
继续执行， 直到遇到阻塞(比如cin >>) 或者下一个断点

r : run
一般刚调试时，需要先设置好断点， 然后 r 表示运行程序

n ：一行一行执行
s : 一行一行执行，但若是一个函数，会进入这个函数

p : print 打印	
p + 具体的变量		//可以看到打印出类的private 变量

bt ： 查看当前栈的情况

多线程：
info break ：查看断点情况
info thread : 查看有哪些线程
thread + 数字 : 表示进入某个线程
thread apply 数字 命令 : 如 thread apply 2 n  
表示线程2中，执行下一行， 如果你直接n ，则当前你在哪个线程就执行哪个线程的下一行。

set scheduler-locking off : 运行全部线程
set scheduler-locking on : 只运行当前线程
因为很多线程同时运行可能不方便查看信息

```

