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

