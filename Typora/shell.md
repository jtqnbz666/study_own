## 写shell脚本

 cat  config.ini.new | awk '!/^#F/' | egrep -n "FIELD" | tail -n
  5| cut -d '|' -f1 | awk '($1>142)' | sort   > b.txt.sort

awk这里表示忽略以#F开头的行， egrep -n表示显示行号，cut -d ‘|’ -f1 表示以'|' 分割 后拿到第一段的数据

```shell
#/bin/bash

oldfile="$1"
newfile="$2"

 cat  $1 | awk '!/^#FIELD00=pdate/' | egrep -n "FIELD" | tail -n 1| cut -d '|' -f1 | sort  > a.txt.sort
str=`cat a.txt.sort | cut -d ':' -f1`
echo "老版本:"
echo "-------------------"
cat a.txt.sort
echo  "-------------------\n\n\n"

 cat  $2 | awk '!/^#FIELD00=pdate/' | egrep -n "FIELD" | tail -n 10| cut -d '|' -f1 | awk  -va=$str '($1>a)' | sort > b.txt.sort

echo "新版本"
echo "-------------------"
cat b.txt.sort
echo "-------------------"

```

小细节

```shell
当看到` ` 或者 $( ) 
表示要在里面进行一系列操作
#循环指定路径下的文件
for file in `ls /etc`
或者
for file in $(ls /etc)

2.
shell中返回0表示true， 其他非0值都是false
if echo "test"
then
	echo "yes"
# 这里会输出yes， 但其实echo "test" 执行成功返回的结果是0
```



readlink配合dirname获取当前路径名

```shell
readlink 是用来找符号链接所指向的位置的
例如
readlink -f /usr/bin/awk
结果为
/usr/bin/gawk #因为/usr/bin/awk是软链接

readlink -f /home/jt/log
结果依然是 /home/jt/log ，因为没有软连接



```

获取当前路径方法

```shell
#!/bin/bash
path=$(dirname $0)
path2=$(readlink -f $path)
#可以换成path2=$(readlink -f $(dirname $0))
echo path2
sh path.sh
/home/software


#直接这样也行
#CURDIR=$(dirname $(readlink -f $0))
#cd $CURDIR
```

字符串拼接, 单引号双引号区别

```shell
当你不想转义也不想取变量的值时可以单引号
如 echo '$name\"' # $name\" 它会原样输出


your_name="runoob"
# 使用双引号拼接
greeting="hello, "$your_name" !"
greeting_1="hello, ${your_name} !"
echo $greeting  $greeting_1
#hello, runoob ! hello, runoob !

# 使用单引号拼接
greeting_2='hello, '$your_name' !'#注意这里必须要''
greeting_3='hello, ${your_name} !'
echo $greeting_2  $greeting_3
#hello, runoob ! hello, ${your_name} !


```

字符串输出

```shell
name="jt"
echo $name
echo ${name} 
#以上两种都可以， 但最好加上{}
比如
echo "I am $jtwhoareyou" # 错误方式
echo "I am ${jt}whoareyou"

```

字符串截取

```shell
str="jtisa great boy"
echo ${str:1:4} #输出tisa
```

字符串查找

```shell
str="jtwhiov89 is better"
echo `expr index ${str} ib` #输出5, 先找到谁就输出多少
```

获取字符串长度

```shell
str="abcd"
echo ${#str}
```



多行注释

```shell
:<<EOF
...
...
EOF 

可以把EOF换成! 或者 ’
```

$类

```shell
$? 获得上一个进程的状态码(0表示成功， 1表示失败)
#如果上一句执行的是 echo "666" ,返回0则表示成功.
#如果上一句是一个函数，可以获得它的返回值

$$ 获取当前进程ID

$! 获取上一个进程ID，#比如在脚本中穿插了其他脚本，或者使用了echo等命令， 这些都是其他进程做的

$# 获得参数列表的总个数
 
$@ 和 $* 表示输入的参数列表
#遍历输入参数
cnt = 1
#这里要不要双引号都可以，如果是$*就不能要双引号
for i in "$#"  
do 
	echo "number of $cnt parameter is : $i"
	((cnt++))
done

$* 和 $@区别
相同点: 都是引用所有参数
不同点：如果都不加"" 。就没有区别，如果加了，比如说现在传参 1 2 3, 则"*"表示传入了一个参数"1 2 3", 而"@"同样是表示传递了"1" "2" "3" 三个参数
```

在脚本中执行另一个脚本

```shell
sh -x ./lingyige.sh $1 $2
```

数组操作

```shell
arr["jt"]="666"
arr["wh"]="888"

单独输出某一个的内容
echo "jt的值为;${arr["jt"]}"
打印全部内容
echo "数组元素为: ${arr[*]}" # ${arr[@]}也行
打印所有的主健
echo "数组的健为: ${!arr[*]}" 
获取数组的长度
echo "数组的长度为: ${#arr[*]}"
```

关系运算符

```shell
-eq 检测两个数是否相等
-ne 不相等
-gt 大于 greater than
-lt 小于 less than
-ge 大于等于 greater equal
-le 小于等于 less equal
a=10
b=20
if [ $a -eq $b ] # 需要注意的是[ 这里必须要有空格
then
	echo "$a 等于 $b"
else 
	echo "$a 不等于 $b"
fi
```

字符串运算符

```shell
= 相等
!= 不相等
-z 检测字符串长度是否为0
-n 检测字符串长度是否不为0
$  检测字符串是否不为0， 不为0返回true
a="abc"
b="efg"

if [ $a = $b ] #注意这里是= 或 == 都可以
then
	echo "$a = $b"
fi

if [ -z $a ]
then
	echo "-z $a 字符串为空"
else 
	echo "-z $a 字符串不为空"
fi

if [$a]
then
	echo "$a :字符串不为空"
else
	echo "$a :字符串为空"
fi
```

数组操作

```shell
定义数组
arr=(1 2 3 4 5)  以空格分开
或者
arr[0]=1
...

读取数组
${数组名[下标]}
打印第一个元素
echo ${arr[0]}

打印所的元素
echo ${arr[@]} 或者 echo ${arr[*]}

获取数组的长度
length=${#arr[@]} 或者 ${#arr[*]}

获取单个元素的长度
lenght=${#arr[具体下标]}
```



布尔运算符(与 或)

```shell
!	非运算
-o 或运算
-a 与运算
[ !false ] 返回true
[ $a -lt 20 -o $b -gt 100]

&& 逻辑与 #如果前者成立，后者才会执行，前者不成立直接结束
判断文件是否存在，存在则创建另一个文件
ls /tmp/abc && touch /tmp/abc/jt

|| 逻辑或 #前者成立，后者就不执行了，前者不成立，后者执行
判断文件是否存在，不存在则创建，存在则什么也不做

-a和&& 作用都是判断是否都成立， 区别在于： 对于&&如果前者不成立，就不会执行后者了， 而-a 是都要执行完，才判断是否都成立

-o和|| -o 前后都会执行， || 前者执行成功则不执行后者
```



文件检测运算符

```shell
-x file 检测文件是否可执行
-e file 检测文件(包括目录)是否存在

file="/bin/test.txt"
if [-e file] 
then
	echo "文件可读"
else 
	echo "文件不可读"
fi
```

source file, sh filename ./filename区别

```shell
1.当shell脚本具有可执行权限时，用sh filename与./filename执行脚本是没有区别得。./filename是因为当前目录没有在PATH中，所有"."是用来表示当前目录的。
2.sh filename 重新建立一个子shell，在子shell中执行脚本里面的语句，该子shell继承父shell的环境变量，但子shell新建的、改变的变量不会被带回父shell，除非使用export。
3.source filename：这个命令其实只是简单地读取脚本里面的语句依次在当前shell里面执行，没有建立新的子shell。那么脚本里面所有新建、改变变量的语句都会保存在当前shell里面。
1.新建一个test.sh脚本，内容为:A=1
2.然后使其可执行chmod +x test.sh
3.运行sh test.sh后，echo $A，显示为空，因为A=1并未传回给当前shell
4.运行./test.sh后，也是一样的效果
5.运行source test.sh 或者 . test.sh，然后echo $A，则会显示1，说明A=1的变量在当前shell中
```

[] 和 (())的区别

```shell
a=10
b=20
对于 == 和 != 它们没有区别
if [ $a == $b ]	# = 或者 == 都可以
if (($a == $b)) #在((处可以不要空格，[必须要
如果写来挨着则
if [ ${a}==${b} ]
if ((${a}==${b}))

而其他的
if [ $a -gt $b ]
if (($a >= $b)) 这就是区别
```

echo方法

```shell
echo "is test" #输出is test
echo "\"is test\"" #输出 "is test"

使用read方法获取一行输入
read name #此时需要输入一行内容，就会赋值给name
echo "${name} is test" #如果输入jt, 这里就是jt is test

将显示结果定向到文件中
echo "It is a test" > myfile#追加内容就用 >> 

原样输出字符串，不转义，也不取变量
```

printf方法

```shell
需要显式的加上换行符
如 : echo "hello, jt"
	printf "hello, jt\n"
	
printf "%s\n" abc def #就算只有一个%s
#abc
#def

printf "%s and %d\n" # 如果没有参数，%s为NULL，%d为0

printf "%-3d" #有-表示左对齐， 没有表示右对齐，想一想99乘法表

```

[] 和 {} 

```shell
a=5
b=6
result=$[a+b]可以， 但不可以 ${a+b}
echo "$result" #11

但是
echo ${a} 和 echo $[a] 都是可以的
```

test操作

```shell
test命令用于检查某个条件是否成立
它可以对数值、字符和文件三个方面进行测试

实例：
#数值
num1=100
num2=100
if test $[num1] -eq $[num2]	# ${num1} 也一样
then
	echo "$[num1]和$[num2]相等" # ${num1} 也一样
fi

#字符串测试
str1="jt"
str2="wh"
if test $num1 = $num2
then
	echo "相等"
else
	echo "不相等"
fi

#文件测试
cd /bin
if test -e ./bash
then
	echo "文件已存在"
else 
	echo "文件不存在"
fi

测试2
cd /bin
if test -e ./notfile -o -e ./bash
then
	echo "至少存在一个文件"
else
	echo "一个都不存在"
fi
```

流程控制

```shell
# if 判断， 通常和test配合使用
num1=$[2*3]  #这种里面进行运算就不能用#{} , 
num2=$[1+5]  #其他时候[]和{}一样的效果
if test $[num1] -eq $[num2]
then
	echo "相等"
else
	echo "不相等"
fi

# if , elif 判断
if condition1
then
    command1
elif condition2 
then 
    command2
else
    commandN
fi


#for循环
for var in item1 item2 ... itemn
do
	echo "${val}"
done

也可写成一行
for val in item1 item2; do echo "${val}"; done;

#while语句
while condition
do
	command
done

#while遍历五个数
ans=1
while (($ans<=5))
do
	echo $ans
	let "ans++"
	# let命令用于执行一个或多个表达式，不用加$
done

#while循环输入
echo -n "请输入内容"
while read name
do
	echo "$name is good!"
done

#while无限循环
while :   #把 : 换成 true 也
do
	command
done
# 或者直接 for ((;;))
```

until循环

```shell
until和while刚好相反
a=0
until [ ! $a -lt 10 ]
do
	echo $a
	a=`expr $a + 1` # let "a++" 和这个操作一样作用
done
```

case ... esac 语句

```shell
echo "输入1~4数字"
read Num
case $Num in
	1) echo "选择了1"
	;;
	2) echo "选择了2"
	;;
	3) echo "选择了3"
	;;
	4) echo "选择了4"
	;;
esac

#break 跳出循环
while :
do
	echo -n "输入1~5之间数字"
	read Num
	case $Num in
		1|2|3|4|5) echo "你输入了$Num"
		;;
		*) echo "输入错误"
			break#把这里改成continue就不会推出循环，继续
		;;
	esac
done
```

函数操作 

```shell
functest(){
read a
read b
return $[a+b] #也可以return $(($a+$b))
}
```

shell输入/输出重定向

```shell
echo "jt" > file  #写到文件中去

wc -l < file #从文件中取出来
```

引入别的shell文件

```shell
#都可以
sh -x ./lingyige.sh
. ./lingyige.sh
source ./lingyige.sh

source不会新开一个shell程序，
sh -x 和 . 基本没有区别

子shell可以继承父shell的环境变量，但子shell不会返回环境变量给父shell， 除非使用export
```

