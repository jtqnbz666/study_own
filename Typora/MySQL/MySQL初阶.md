**DML(Data Manipulation Language)**数据操纵语言，对数据库中的数据进行一些简单操作，如insert、delete、update、select等。DML操作是可以手动控制事务的开启、提交和回滚的。

**DDL(Data Definition Language)**数据定义语言，对数据库中的某些对象(例如database、table)进行管理，如create、alter和drop。DDL操作是隐性提交的，不能rollback。



##  1.数据库分类

关系型数据库(sql)，非关系型数据库(nosql)

mysql:关系型数据据库，二维表格，库，表，数据，格式固定，安全性好

Redis,MongoDB:非关系型数据库，灵活，语法不通用

### mysql

查看数据库：show databases;

进入某个数据库:use mysql;

判断当前在哪个数据库里:select database();

查看当前用户:select user();

### 1.库级

~~~mysql
#展示库
show databases;
#查看当前所使用的数据库
select database();
#创建库
create database laotie;
create database if not exists laotie;
#使用库
use laotie;
#删除库
drop database laotie;
drop database if exists laotie;
~~~

### 2.表级

~~~mysql
#展示表
show table;
show table from mysql;#展示mysql库里的表
#创建表
create table student(name varchar(20),age int,sex char(10));
create table if not exists student(name varchar(20),age int,sex char(10));

CREATE TABLE emps SELECT * FROM test.emp;(这种方式是复制表，所以约束条件复制不过来(除了非空约束NULL)，比如主键primary key 的属性)
#展示表结构
desc student;
show columns from student;
describe student;
#展示标准sql语句 ，创建表的信息
show create table/function/procdure student;  #加个\G可以更方便的查看信息
#展示标准sql语句 ，创建表的信息
show table/function/procdure status like '..%S';
#删除表
drop table student;

#创建一个新表和原来的一样
CREATE TABLE mystudent LIKE student;

#从另一张表中，查询到然后插入
INSERT INTO mystudent SELECT * FROM student WHERE sex='男';
INSERT INTO mystudent(name,age,sex) SELECT * FROM student WHERE sex='女';
~~~

### 3.表结构

~~~mysql
#修改表名
alter table new_table rename to old_table;
#修改字段名
alter table oldtable  change id stu_id int;
#修改字段类型
alter table oldtable  modify stu_id tinyint;
#添加字段
alter table old_table add age tinyint;
alter table old_table add(aa int,bb int,cc int);#添加多个
#删除字段
alter table old_table drop bb;#删除单个
alter table old_table drop aa,drop cc,drop stu_id;#删除多个
#表名和字段名尽量避免修改。
~~~

### 4.数据

~~~mysql
#增删改查
#查看表数据
select * from student;
#增
insert into student values("小江",18);#插入一行数据
insert into student values("小王",19),("小慧",20);#插入多行数据
insert into student(name) values("小涛");#指定字段插入。

#查
select *from student;#全字段查询
select name from student;#指定字段查询
select name,age from student;
#取别名(as和' '可以省略)
select name as '姓名',age as '年龄' from student;
select name as '姓名',age as '年龄'，age*2 as'两倍年龄' from student;

select *from student where age>14;
select *from student where name like'小_';
select *from student where name like'小%';

#用到and或者or
select *from student where age=18 or age=19;
select *from student where name="小江" and age=18;

#改
update student set age=20;#修改全部数据
update student set age=19 where name="小江";#修改指定条件数据
update student set name="江江",age=12 where name="小江";#修改多个字段。
update student set name="江江",age=12;#修改全部数据
#注意：一定要带where条件，不然就会修改全部。

#删
#条件删除，删除的是符合条件的全部数据。
delete from student where age=13;
#全部删除
delete from student; #可以使用where，会触发触发器，需要扫描表中的每一行然后根据条件删，删除内容。不删结构 ， 是DML类型语句支持事务回滚和提交， 不释放空间
#注意：一定要带where条件，不然就是删库路

truncate student;#删除内容。不删结构，不会触发触发器，更快，不能使用where，是DDL类型语句不支持事务回滚和提交 ， 会释放空间

truncate删除数据后重新写数据会从1开始，而delete删除数据后只会从删除前的最后一行续写；内存空间上，truncate省空间

drop table 表名;#结构内容一起删，不触发触发器，不能使用where，是DDL类型语句不支持事务回滚和提交

#like 的使用， 一个'_'代表一个字符， '%'代表任意数目个字符
#例如：查询姓名为四个字的员工

SELECT * FROM emp WHERE ename LIKE '____';
# 查询姓名最后一位是S的员工
SELECT * FROM emp WHERE ename LIKE '%S';

~~~

## 2.聚合函数(统计函数)

> SQL 允许对表中的数据进行计算，将一列数据作为一个整体，进行纵向计算。

| 函数名        | 作用                               |
| ------------- | ---------------------------------- |
| MAX(column)   | 返回某列的最低值（没有则返回NULL） |
| MIN(column)   | 返回某列的最高值（没有则返回NULL） |
| COUNT(column) | 返回某列的行数（不包括 NULL 值）   |
| COUNT(*)      | 返回被选列行数（包括NULL）         |
| SUM(column)   | 求和                               |
| AVG(column)   | 求平均值                           |

其中 COUNT 函数可用于任何数据类型 (因为它只是计数)，而 SUM 、AVG 函数都只能对数值类型做计算，MAX 和 MIN 可用于数值、字符串或是日期时间数据类型。

## 3.1条件查询

> 我们知道从 MySQL 表中使用 SELECT 语句来查询数据，如需有条件地从表中选取数据，可将 WHERE 子句添加到 SELECT 语句中。

**语法**

```mysql
SELECT 字段名 FROM 表名 WHERE 条件;
```

#### **运算符表**

| 关系运算符 | 功能     |
| ---------- | -------- |
| >          | 大于     |
| \>=        | 大于等于 |
| <          | 小于     |
| <=         | 小于等于 |
| =          | 等于     |
| <> 或 !=   | 不等于   |

| 逻辑运算符   | 功能                         |
| ------------ | ---------------------------- |
| AND 或 &&    | 并且（多个条件同时成立）     |
| OR  或  \|\| | 或者（多个条件任意成立一个） |
| NOT 或 ！    | 非，不是                     |

| 其他             | 功能                                       |
| ---------------- | ------------------------------------------ |
| BETWEEN...AND... | 在某个范围之间（含最小、最大值）           |
| IN(...)          | 在in之后的列表中的值，多选一               |
| LIKE             | 模糊匹配（_匹配单个字符，%匹配任意个字符） |
| IS [NOT] NULL    | 是 NULL                                    |

#### 

## 

## 3.2分组查询

语法：

~~~mysql
SELECT 分组字段/聚合函数 FROM 表名 [WHERE 条件] GROUP BY 分组字段 [HAVING 分组后条件];
~~~

#### WHERE和HAVING区别

- WHERE：是在分组之前使用(可以没有GROUP BY)，不允许使用统计函数；
- HAVING：是在分组之后使用(必须结合GROUP BY)，允许使用统计函数。

![image-20220305211810042](C:/Users/ASUS/Desktop/MySql%E8%AF%BE%E4%BB%B6/04%20%E6%95%B0%E6%8D%AE%E6%9F%A5%E8%AF%A2/assets/image-20220305211810042.png)

注意事项：
情况一：如果语句后边**没有** GROUP BY，但使用了聚合函数（COUNT,SUM 等），
，那么所有其他任何字段都不能在from前使用了

例：

~~~mysql
select count(*) from emp ;对的
select sal,count(*) from emp;错的
~~~

情况二：如果语句后边**有** GROUP BY，那么from前可以使用聚合函数，
同时还有使用GROUP BY后边出现过的字段，如果没出现过的不可以使用。

例：

~~~mysql
select job,count(*) from emp GROUP BY job;对的
select job,sal ,count(*) from emp GROUP BY job; 错的 （多了一个sal)
~~~

### 实际使用

**注意：where 后不能使用聚合函数(当然别名也不能用)，但可以用字段**

1. 显示不同职位薪资少于等于1500的人数，并且该职位人员数大于2人,

~~~mysql
select job,count(*) total from emp where sal<=1500 GROUP BY job having count(*)>2;

//where 后不能使用聚合函数(当然别名也不能用)，
如 where total > 2;

~~~

2. 显示非销售人员(salesman)工作名称以及从事同一工作的员工的月工资的总和，并且要满足从事同一工作的员工的月工资合计大于等于**5000**。

```mysql
select job,sum(sal) from emp where job!='salesman' group by job having sum(sal)>=5000;
#第二种方式
select job,sum(sal) as total from emp where job!='salesman' group by job having total>=5000;
```

### 细节： 

**以上出现的聚合函数count (*) 和sum(sal) 的作用都是在分组后生效的。**

## 4.排序查询

1. 根据姓名对员工进行排序(升序)

```mysql
SELECT * FROM emp ORDER BY ename;
```

2. 查询入职时间大于1981年6月6日的员工信息，并按照入职时间降序排列

```mysql
SELECT * FROM emp WHERE hiredate>'1981-6-6' ORDER BY hiredate DESC;
```

3. **根据工作职位，进行升序排列，职位相同的情况下，再根据入职时间降序排列**

```mysql
SELECT * FROM emp ORDER BY job ASC,hiredate DESC;
```

4. 查询工资不在1000到2000之间的员工的姓名和工资，工资按降序排列

```mysql
SELECT * FROM emp WHERE sal NOT BETWEEN 1000 AND 2000 ORDER BY sal DESC;
```

5. 按工作职位进行分组，查看每个工作职位的人数，工作职位按升序排列

```mysql
SELECT job,COUNT(*) FROM emp GROUP BY job ORDER BY job ASC;
```



## 5.分页查询

> 对于比较多的数据，如果在一个页面全部显示，查看起来会眼花缭乱。如果能进行分页显示，将更加快捷、清新的浏览！

![image-20220307162625907](C:/Users/ASUS/Desktop/MySql%E8%AF%BE%E4%BB%B6/04%20%E6%95%B0%E6%8D%AE%E6%9F%A5%E8%AF%A2/assets/image-20220307162625907.png)

**语法**

```mysql
SELECT 字段名 FROM 表名 LIMIT 起始索引,查询记录数;
```

**特点**

- 起始索引从0开始，起始索引 = (查询页码 - 1) * 每页显示记录数;
- 如果查询的是第一页数据，起始索引可以省略，直接简写为LIMIT 10。

**查询需求**

1. 查询第一页员工数据，每页展示10条记录

```mysql
select ename from emp limit 0,10;
```

2. 查询工资大于1500的员工数据，按工资升序排列，并分页，每页展示5条数据

```mysql
select * from emp where sal>1500 order by sal asc limit 5;
```

## 6.函数

MySQL 函数会对传递进来的参数进行处理，并返回一个处理结果，也就是返回一个值。

MySQL 包含了大量并且丰富的函数，咱们讲解几十个常用的，剩下的比较罕见的函数我们可以到「[MySQL 参考手册](https://dev.mysql.com/doc/refman/8.0/en/functions.html)」查询。

### 6.1字符串函数

| 函数                                                         | 作用                                                 |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| UPPER(列\|字符串)                                            | 将字符串每个字符转为大写                             |
| LOWER(列\|字符串)                                            | 将字符串每个字符转为小写                             |
| CONCAT(str1,str2,...)                                        | 将所有字符串连接成一个字符串                         |
| REPLACE(列\|字符串,新字符串)                                 | 使用新字符串替换旧字符串                             |
| LENGTH(列\|字符串)                                           | 求字符串长度                                         |
| SUBSTR(列\|字符串,开始点[长度范围，从1开始])                 | 字符串截取                                           |
| LEFT(str,len)                                                | 获取字符串左边len个字符组成的字符串                  |
| RIGHT(str,len)                                               | 获取字符串右边len个字符组成的字符串                  |
| MID(str,pos,len)                                             | 获取字符串中从pos(第几个)位置开始，长度为len的字符串 |
| ASCII(字符)                                                  | 返回与指定字符对应的十进制整数                       |
| CHAR(数字)                                                   | 返回与整数对应的字符                                 |
| RPAD(列\|字符串,长度,填充字符)<br>LPAD(列\|字符串,长度,填充字符) | 用指定的字符在字符串右或左填充                       |
| LTRIM(字符串)、RTRIM(字符串)                                 | 去掉字符串左或右的空格                               |
| TRIM(列\|字符串)                                             | 去掉字符串左右空格                                   |
| INSTR(列\|字符串,要查找的字符串,开始位置,出现位置)           | 查找一个子字符串是否在指定的位置上出现               |

### 6.2字符串函数

注意：字符串函数和聚合函数相比：

聚合函数中

~~~mysql
select count(*) from emp ;对的
select sal,count(*) from emp;错的
~~~

字符串函数中

~~~mysql
select substr(ename,3) from emp ;对的
select ename,substr(ename,3) from emp;对的
~~~

相同点：

~~~mysql
在where 后边不能使用函数来进行判断比较，函数的别名当然也不行
举例：
select substr(ename,3) test from emp
where test != "ITH";

但普通函数可以和聚合函数一样使用having来判断
select ename , substr(ename,3) test from emp 
group by ename having test != "ITH";
~~~



1. 把员工姓名首字母大写

```mysql
SELECT CONCAT(UPPER(LEFT(ename,1)),LOWER(SUBSTR(ename,2))) FROM emp;
```

2. 查询出姓名长度是5的所有员工信息

```mysql
SELECT * FROM emp WHERE CHAR_LENGTH(ename)=5;
```

3. 查询出员工姓名前三个字母是‘JAM’的员工

```mysql
SELECT * FROM emp WHERE SUBSTR(ename,1,3)='JAM';
```

4. 查询所有员工信息，但是不显示姓名的前三个字母

```mysql
SELECT ename 原始姓名,SUBSTR(ename,3) FROM emp;
# 只显示后三个字符
SELECT ename 原始姓名,SUBSTR(ename,-3) FROM emp;
```

5. 填充

```mysql
SELECT LPAD('Maye',10,'*'),RPAD('Maye',10,'*'),
LPAD(RPAD('Maye',10,'*'),16,'*');
```

![image-20220310142242551](C:/Users/ASUS/Desktop/MySql%E8%AF%BE%E4%BB%B6/05%20%E5%87%BD%E6%95%B0/assets/image-20220310142242551.png)

### 6.3数学函数

| 函数                      | 作用                                                         |
| ------------------------- | ------------------------------------------------------------ |
| ROUND(数字[,保留位数])    | 根据指定的保留位数对小数进行四舍五入，如果不指定保留位数，则小数点之后的数字全部进行全部四舍五入 |
| TRUNCATE(数字[,截取位数]) | 保留指定位数的小数，如果不指定，则表示不保留                 |
| CEIL(数字)，CEILING(数字) | 对小数向上取整                                               |
| FLOOR(数字)               | 对小数向下取整                                               |
| RAND([n])                 | 随机返回0到1.0之间的小数，如果指定n每次产生的就都是重复的    |

```mysql
select round(127.127,val);
#这个val决定保留几位小数
#如果val是负数，比如此时是-1，则结果是130，它会把整数部分的最后一位变成0，并四舍五入上去
select round(153.123,val);
#如果val是-1得到的是150，-2则200，把整数部分的最后两位变成0，并四舍五入上去

truncate 和select很像，但是不进行四舍五入
select TRUNCATE(153,-2);#把最后两个变成0 ，得到的是100
```





通过数据库的函数，生成一个六位数字的随机验证码。

```mysql
SELECT LPAD(TRUNCATE(RAND()*1000000,0),6,'0');
```

### 6.4日期和时间函数

| 函数                                                         | 作用                                 |
| ------------------------------------------------------------ | ------------------------------------ |
| CURDATE()，CURRENT_DATE()                                    | 返回当前日期                         |
| CURTIME()，CURRENT_TIME()                                    | 返回当前时间                         |
| NOW()，CURRENT_TIMESTAMP()，LOCALTIME()，<br>SYSDATE()，LOCALTIMESTAMP() | 返回会当前日期和事件                 |
| YEAR(日期)                                                   | 返回日期中的年份                     |
| MONTH(日期)                                                  | 返回日期中的月份[1,12]               |
| DAY(日期)                                                    | 返回日期中的天[1,31]                 |
| HOUR(时间)                                                   | 返回时间中的小时                     |
| MINUTE(时间)                                                 | 返回实践中的分钟                     |
| SECOND(时间)                                                 | 返回时间中的秒                       |
| DAYOFWEEK(日期)                                              | 返回星期几，1星期日，2星期一         |
| WEEKDAY(日期)                                                | 返回星期几，0星期一，1星期二         |
| DAYOFYEAR(日期)                                              | 计算指定日期是本年第几天             |
| DAYOFMONTH(日期)                                             | 计算指定日期是本月第几天             |
| QUARTER(日期)                                                | 计算指定日期是第几季度               |
| TIME_TO_SEC(time)                                            | 将指定时间转换为秒                   |
| SEC_TO_TIME(sec)                                             | 将以秒为单位的时间转换为时分秒的格式 |
| DATEDIFF(d1,d2)                                              | 计算两个日期相隔的天数               |
| ADDDATE(date,n)                                              | 计算指定日期加上n天后的日期          |
| SUBDATE(date,n)                                              | 计算指定日期减去n天后的日期          |
| ADDTIME(time,n)                                              | 计算指定时间加上n秒后的时间          |
| SUBTIME(time,n)                                              | 计算指定时间减去n秒后的时间          |
| DATE_FORMAT(date,f)                                          | 按格式返回日期                       |
| TIME_FORMAT(time,f)                                          | 按格式返回时间                       |

### 6.5条件判断函数

| 函数           | 作用                                               |
| -------------- | -------------------------------------------------- |
| IF(expr,v1,v2) | 如果表达式expr成立，返回结果v1；否则，返回结果v2。 |
| IFNULL(v1,v2)  | 如果v1的值不为NULL，则返回v1，否则返回v2           |
| CASE           | 分支(详见下面)                                     |

- `IF(expr,v1,v2)`

```mysql
SELECT IF(1>0,'true','false');
->true
```

- `IFNULL(v1,v2)`

```mysql
SELECT IFNULL(NULL,'maye');
->maye
```

- `CASE`

  **语法1：**

```mysql
CASE
	WHEN expr1 THEN V1
	WHEN expr2 THEN V2
	...
	ELSE vn
END	
```

```mysql
SELECT CASE 
　　WHEN 1 > 0
　　THEN '1 > 0'
　　WHEN 2 > 0
　　THEN '2 > 0'
　　ELSE '3 > 0'
　　END
->1 > 0
```

**语法2：**

```mysql
CASE expr 
　　WHEN e1 THEN v1
　　WHEN e1 THEN v1
　　...
　　ELSE vn
END
```

```mysql
SELECT CASE sal 
　　WHEN 5000 THEN '我是大老板'
　　WHEN 800 THEN '我是小弟'
ELSE '你是谁'
END
```



## 系统信息函数

> 系统信息函数用来查询MySQL数据库的系统信息。

| 函数                                                         | 作用                        |
| ------------------------------------------------------------ | --------------------------- |
| VERSION()                                                    | 返回数据库的版本号          |
| CONNECTION_ID()                                              | 返回服务器的连接数          |
| DATABASE()、SCHEMA                                           | 返回当前数据库名            |
| USER()、SYSTEM_USER()、SESSION_USER()、CURRENT_USER()、CURRENT_USER | 返回当前用户                |
| CHARSET(str)                                                 | 返回字符串str的字符集       |
| COLLATION(str)                                               | 返回字符串str的字符排列方式 |

### 6.6加密函数

> 加密函数是MySQL用来对数据进行加密的函数.

| 函数     | 作用                                                         |
| -------- | ------------------------------------------------------------ |
| MD5(str) | 对字符串str进行散列，可以用于一些普通的不需要解密的数据加密，如密码 |
| sha1     | 对字符串str进行散列，可以用于一些普通的不需要解密的数据加密，如密码，比MD5更安全 |

### 6.7其他函数

### 1.进制转换

| 函数                      | 作用                                |
| ------------------------- | ----------------------------------- |
| BIN(x)                    | 返回x的二进制                       |
| HEX(x)                    | 返回x的十六进制                     |
| OCT(x)                    | 返回x的八进制                       |
| CONV(x,from_base,to_base) | 将from_base进制的x，转成to_base进制 |

```mysql
SELECT BIN(10),HEX(10),OCT(10),CONV(16,16,10);
->1010 A 12 22
```

### 2.IP地址转换

| 函数              | 作用                         |
| ----------------- | ---------------------------- |
| INET_ATON(IP)     | 将点分十进制的IP地址转为数字 |
| INET_NTOA(number) | 将数字形式的IP转为点分十进制 |

```mysql
SELECT INET_ATON('192.168.0.1')
    ->3232235521
SELECT INET_NTOA(3232235521)
    ->192.168.0.1
```



## 多表关系

项目开发中，在进行数据库表结构设计时，会根据业务需求及业务模块之间的关系，分析并设计表结构，由于业务之间相互关联，所以各个表结构之间也存在着各种联系，基本上分为三种：

- 一对多（多对一）
- 多对多
- 一对一



### 一对多(多对一)

案例：部门与员工的关系

关系：一个部门对应多个员工，一个员工对应一个部门

实现：在多的一方建立外键，关联另一方的主键

![image-20220311141247079](C:/Users/ASUS/Desktop/MySql%E8%AF%BE%E4%BB%B6/06%20%E5%A4%9A%E8%A1%A8%E6%9F%A5%E8%AF%A2/assets/image-20220311141247079.png)

### 多对多

案例：学生与课程的关系

关系：一个学生可以选修多门课程，一门课程也可以供多个学生选择

实现：建立第三张中间表，中间表至少包含两个外键，分别关联两方主键

![image-20220311134035938](C:/Users/ASUS/Desktop/MySql%E8%AF%BE%E4%BB%B6/06%20%E5%A4%9A%E8%A1%A8%E6%9F%A5%E8%AF%A2/assets/image-20220311134035938.png)



### 一对一

案例：用户与用户详细的关系

关系：一对一关系，多用于单表拆分，将一张表的基础字段放在一张表中，其他详情字段放在另一张表中，以提升操作效率。

![image-20220311134456593](C:/Users/ASUS/Desktop/MySql%E8%AF%BE%E4%BB%B6/06%20%E5%A4%9A%E8%A1%A8%E6%9F%A5%E8%AF%A2/assets/image-20220311134456593.png)

实现：在任意一方加入一个外键，关联另一方的主键，并且设置外键为唯一约束

![image-20220311134608853](C:/Users/ASUS/Desktop/MySql%E8%AF%BE%E4%BB%B6/06%20%E5%A4%9A%E8%A1%A8%E6%9F%A5%E8%AF%A2/assets/image-20220311134608853.png)



## 多表查询

前面我们使用的查询，只是对单表进行查询，在具体的应用中，经常需要实现在一个查询语句中显示多张数据表的数据，这就是所谓的多表联合查询。

在具体实现连接操作时，首先将两个或两个以上的表按照某个条件连接起来，然后再查询到所要求的数据记录。

连接查询分为**交叉连接**、**内连接**、**外连接**查询三种方式。

### 交叉连接

> 交叉连接不带WHERE子句，它返回被连接的两个表所有数据行的笛卡尔积

- 查询员工及员工所在的部门信息

```mysql
SELECT * FROM emp,dept;　
SELECT * FROM emp CROSS JOIN dept;　
```

如果不加条件直接进行查询，则数据条数是两个表记录条数的乘积，这种结果我们称之为 **笛卡尔乘积**。

> 笛卡尔乘积公式 : A表中数据条数  * B表中数据条数 = 笛卡尔乘积

如果两张表的数据量都很大，那么这种庞大时很可怕的，所以现在必须想办法消除掉笛卡尔积的无效记录。

想要消除笛卡尔积的无效记录，需要使用关联字段。

**范例：**利用等值条件来处理笛卡尔积

```mysql
SELECT * FROM emp,dept WHERE emp.deptno=dept.deptno;
```



### 内连接

//主要必须是显示才能用ON ，也可以在ON后继续用WHERE ,但不可以用WHERE 直接 替代ON，隐式不可以用ON 

> 在表关系的笛卡尔积数据记录中，保留表关系中所有匹配的数据记录，舍弃不匹配的数据记录。按匹配的条件可以分成等值连接和不等值连接。

有两种，显式的和隐式的，返回连接表中符合连接条件和查询条件的数据行。（所谓的链接表就是数据库在做查询形成的中间表）。

- 隐式内连接

```mysql
SELECT * FROM 表1，表2 WHERE 条件;
```

- 显示内连接（使用关键字INNER  JOIN）

```mysql
SELECT * FROM 表1 [INNER] JOIN 表2 ON 条件; 
```



#### 等值连接

> 在连接条件中使用等于号(=)运算符比较被连接列的列值，

- 查询员工及员工部门信息

```mysql
#隐式连接
SELECT * FROM emp e,dept d WHERE e.deptno=d.deptno;
#显示连接
SELECT * FROM emp e INNER JOIN dept d ON e.deptno=d.deptno;
```

- 等值连接可以使用USING来自动关联两表中相同的列

```mysql
SELECT * FROM emp INNER JOIN dept USING(deptno);
```



#### 非等值连接

> 在连接条件使用除等于运算符以外的其它比较运算符比较被连接的 列的列值。这些运算符包括>、>=、<=、<、!>、!<和<>等。 

- 查询员工工资级别

```mysql
#隐式连接
SELECT * FROM emp e,salgrade s WHERE e.sal BETWEEN s.losal AND s.hisal;
#显示连接
SELECT * FROM emp e INNER JOIN salgrade s ON  e.sal BETWEEN s.losal AND s.hisal;
```



### 外连接    

不用加OUTER 也行

> 在表关系的笛卡尔积中，不仅保留表关系中所有匹配的数据记录，而且还保留部分不匹配的记录。按照保留不匹配条件数据记录来源可以分为左外连接(LEFT OUTER JOIN)、右外连接(RIGHT OUTER JOIN)和全外连接(FULL OUTER JOIN)。

外连接使用语法如下：

```mysql
SELECT * FROM 表1 LEFT|RIGHT|FULL [OUTER] JOIN 表2 ON 条件; 
```



#### 左外连接

> 在表关系的笛卡尔积中，出了选择相匹配的数据记录，还包含关联**左边表**中不匹配的数据记录。

插入一条部门编号为NULL的数据(关于插入后面会讲)

```mysql
INSERT INTO emp(empno,ename,job,mgr,hiredate,sal,comm,deptno) VALUES(6666,'顽石','teacher',7369,CURDATE(),800,100,NULL);
```

查询员工及对应的部门信息（没有部门的员工也**显示**出来，没有员工的部门**不显示**）

```mysql
SELECT * FROM emp e LEFT OUTER JOIN dept d ON e.deptno=d.deptno; 左外连接

SELECT * FROM emp LEFT JOIN dept ON emp.deptno = dept.deptno WHERE dept.deptno IS NULL;也不是左连接

```



#### 右外连接

> 在表关系的笛卡尔积中，出了选择相匹配的数据记录，还包含关联**右边表**中不匹配的数据记录。

查询员工及对应的部门信息（没有部门的员工**不显示**，没有员工的部门**显示**）

```mysql
SELECT * FROM emp e RIGHT OUTER JOIN dept d ON e.deptno=d.deptno; 右外连接
SELECT  * FROM emp RIGHT JOIN dept ON emp.deptno=dept.deptno WHERE emp.deptno is NULL;也不是右连接
```



#### 全连接

> 在表关系的笛卡尔积中，出了选择相匹配的数据记录，还包含关联**左右两边表**中不匹配的数据记录。

查询员工及对应的部门信息（没有部门的员工**显示**，没有员工的部门**显示**）

```mysql
#mysql不支持全连接，但是可以用过集合运算来实现
可以通过几种方式来组合实现 全连接， 使用Union
例如：
SELECT * FROM emp e RIGHT OUTER JOIN dept d ON e.deptno=d.deptno
union
SELECT * FROM emp e left OUTER JOIN dept d ON e.deptno=d.deptno where d.deptno != null;
```



### 自连接

> 自连接就是指表与其自身进行连接

查询每个员工对应的领导姓名。

```mysql
SELECT e.ename,e.mgr,me.ename 领导 FROM emp e,emp me WHERE e.mgr=me.empno;
```

## 七种JOINS实现

![](C:\Users\ASUS\Pictures\博客图片\QQ图片20220811000803.png)

## 集合运算

MySQL支持并集运算。



> 并集即两个集合所有部分

![](C:\Users\ASUS\Pictures\博客图片\QQ图片20220811003618.png)

**UNION DISTINCT** 

1. UNION 会删除重复行
2. 相同的行在结果中只出现一次。

```mysql
SELECT * FROM emp
UNION   DISTINCT #加不加DISTINCT都可以，默认的
SELECT * FROM emp WHERE deptno=10;
```



**UNION ALL**

1. UNION ALL 不会删除重复行
2. 相同的行在结果中可能出现多次

```mysql
SELECT * FROM emp
UNION ALL
SELECT * FROM emp WHERE deptno=10;
```

**要求**

(1)输入的查询不能包含ORDER BY字句，可以为整个集合运算结果选择性地增加一个ORDER BY字句；

```mysql
SELECT * FROM emp 
UNION  
SELECT * FROM emp WHERE deptno =10 
ORDER BY ename;#这里直接用ename ，因为ORDER BY 是在她们生成的临时表中查询的，不能指定 emp.ename;

#取别名
SELECT emp.ename 'name',emp.empno 'id'  FROM emp
UNION ALL
SELECT emp.ename ,emp.empno  FROM emp WHERE deptno=10
ORDER BY 'id';

```



(2)两个查询必须包含相同的列数；

```mysql
正确：
SELECT emp.ename FROM emp
UNION 
SELECT emp.deptno FROM emp;

错误：
SELECT emp.ename,emp.comm FROM emp
UNION 
SELECT emp.deptno FROM emp;
```



(3)相应列必须具有兼容的数据类型。兼容个的数据类型：优先级较低的数据类型必须能隐式地转换为较高级的数据类型。比如输入的查询1的第一列为int类型，输入的查询2的第一列为float类型，则较低的数据类型int类型可以隐式地转换为较高级float类型。如果输入的查询1的第一列为char类型，输入的查询2的第一列为datetime类型，则会提示转换失败：从字符串转换日期和/或时间时，转换失败；

```mysql
#很好理解，从前边的 连接 方式 ，比如全连接   ，并不是 只对一个表才有用， 只要你满足上下UNION的两个表查出来的数据列一样就行
```



(4)集合运算结果中列名由输入的查询1决定，如果要为结果分配结果列，应该在输入的查询1中分配相应的别名；

(5)集合运算时，对行进行比较时，集合运算认为两个NULL相等；



## 多表查询练习

1. 查询出雇佣日期在1981年的所有员工的编号、姓名、雇佣日期、工作、领导姓名、雇佣月工资、雇佣年工资(基本工资+奖金)，工资等级、部门编号、部门名称、部门位置，并且求这些员工的月基本工资在1500~3500之间，将最后的结果按照年工资的降序排列，如果年工资相等，则按照工作进行排序。

**需求很多，逐步分析**

- 确定所需要的数据表
  - emp：编号、姓名、雇佣日期，工作、月工资、年薪
  - emp：领导姓名
  - dept：部门编号、名称、位置
  - salgrade：工资等级
- 确定一致的关联字段
  - 员工和领导：e.mgr=e1.empno
  - 员工和部门：e.deptno=dept.deptno
  - 员工和工资等级：e.sal BETWEEN s.losal AND s.hisal

**步骤一：**查询出所有在1981年雇佣的雇员编号、姓名、御用日期、工作、月工资、年工资，并且月薪在1500~3500之间。只需要emp单张表即可。

```mysql
SELECT e.empno,e.ename,e.hiredate,e.sal,(e.sal+IFNULL(e.comm,0))*12 年薪
FROM emp e
WHERE DATE_FORMAT(e.hiredate,'%Y')='1981' AND e.sal BETWEEN  1500 AND 3500;
```

**步骤二：**加入领导信息，使用自身关联。

```mysql
SELECT e.empno,e.ename,e.hiredate,e.sal,(e.sal+IFNULL(e.comm,0))*12 年薪,m.ename 领导
FROM emp e,emp m
WHERE DATE_FORMAT(e.hiredate,'%Y')='1981' AND e.sal BETWEEN  1500 AND 3500
AND e.mgr=m.empno;
```

**步骤三：**加入部门信息。

```mysql
SELECT e.empno,e.ename,e.hiredate,e.sal,(e.sal+IFNULL(e.comm,0))*12 年薪,m.ename 领导,d.deptno,d.dname,d.loc
FROM emp e,emp m,dept d
WHERE DATE_FORMAT(e.hiredate,'%Y')='1981' AND e.sal BETWEEN  1500 AND 3500
AND e.mgr=m.empno AND e.deptno=d.deptno;
```

**步骤四：**加入工资等级

```mysql
SELECT e.empno,e.ename,e.hiredate,e.sal,(e.sal+IFNULL(e.comm,0))*12 年薪,m.ename 领导,d.deptno,d.dname,d.loc,
s.grade
FROM emp e,emp m,dept d,salgrade s
WHERE DATE_FORMAT(e.hiredate,'%Y')='1981' AND e.sal BETWEEN  1500 AND 3500
AND e.mgr=m.empno AND e.deptno=d.deptno
AND e.sal BETWEEN s.losal AND s.hisal;
```

**步骤五：**排序

```mysql
SELECT e.empno,e.ename,e.hiredate,e.sal,(e.sal+IFNULL(e.comm,0))*12 年薪,m.ename 领导,d.deptno,d.dname,d.loc,
s.grade
FROM emp e,emp m,dept d,salgrade s
WHERE DATE_FORMAT(e.hiredate,'%Y')='1981' AND e.sal BETWEEN  1500 AND 3500
AND e.mgr=m.empno AND e.deptno=d.deptno
AND e.sal BETWEEN s.losal AND s.hisal
ORDER BY 年薪 DESC;

#
SELECT e.empno,e.ename,e.hiredate,e.job,e.sal,(e.sal+IFNULL(e.comm,0))*12 年薪 ,e1.ename ,d.deptno,d.dname,d.loc ，s.grade
FROM emp e,emp e1,dept d,salgrade s 
WHERE DATE_FORMAT(e.hiredate,'%Y')='1981' AND e.sal BETWEEN 1500 AND 3500 
AND e.mgr=e1.empno AND e.deptno=d.deptno 
AND e.sal BETWEEN s.losal AND s.hisal 
ORDER BY 年薪 DESC,e.job DESC;
```

# 子查询

## 子查询概念

子查询指一个查询语句嵌套在另一个查询语句内部的查询，这个特性从 MySQL 4.1 开始引入。

在特定情况下，一个查询语句的条件需要另一个查询语句来获取，内层查询（inner query）语句的查询结果，可以为外层查询（outer query）语句提供查询条件。

> 内层查询即子查询，外层查询即主查询，只是叫法不同而已。

### **查询需求**

- 查询公司之中工资最低的员工信息
  - 工资最低是多少？
  - 工资最低的员工是谁？

```mysql
SELECT * FROM emp WHERE sal=(SELECT MIN(sal) FROM emp);
```

### 子查询规范

1. 子查询必须放在小括号中
2. 子查询一般放在比较操作符的右边，以增强代码可读性
3. 子查询可以出现在几乎所有的SELECT字句中（如：SELECT、FROM、WHERE、ORDER BY、HAVING子句）



### 子查询分类

**根据子查询返回的数据分类**：

- 标量子查询(scalar subquery)：返回1行1列一个值
- 行子查询(row subquery)：返回的结果集是 1 行 N 列
- 列子查询(column subquery)：返回的结果集是 N 行 1列
- 表子查询(table subquery)：返回的结果集是 N 行 N 列

> 子查询可以返回一个标量(就一个值)、一个行、一个列或一个表，这些子查询分别称之为标量、行、列和表子查询。



**根据子查询和主查询之间是否有条件关联分类：**

- 相关子查询：两个查询之间有一定的条件关联(相互联系)
- 不相关子查询：两个查询之间没有条件关联(相互独立)



为了方便，对于在何处使用子查询给大家给出几点个人建议：

- 子查询出现在**WHERE**子句中：此时子查询返回的结果一般都是单列单行、单行多列、多行单列
- 子查询出现在**HAVING**子句中：此时子查询返回的都是单行单列数据，同时为了使用统计函数操作
- 子查询出现在**FROM**子句中：此时子查询返回的结果图一般都是多行多列，可以按照一张数据表(临时表)的形式操作



## 标量子查询

> 子查询返回的是单行单列的数据，就是一个值

- 查询出基本工资比ALLEN低的全部员工信息

```mysql
SELECT * FROM emp WHERE sal<(SELECT sal FROM emp WHERE eanme='ALLEN');
```

- 查询基本工资高于公司平均工资的全部员工信息

```mysql
SELECT * FROM emp WHERE sal>(SELECT AVG(sal) FROM emp);
```

- 查询出与ALLEN从事同一工作，并且基本工资高于员工编号为7521的全部员工信息

```mysql
SELECT * FROM emp WHERE job=(SELECT job FROM emp WHERE ename='ALLEN') 
AND sal>(SELECT sal FROM emp WHERE empno=7521)
#把ALLEN自己去掉
AND ename<>'ALLEN';#AND ename!='ALLEN'
```



## 单行子查询

> 子查询返回的是单行多列的数据，就是一条记录

- 查询与SCOTT从事统一工作且工资相同的员工信息

```mysql
SELECT * FROM emp WHERE (job,sal)=(SELECT job,sal FROM emp WHERE ename='SCOTT')
AND ename<>'SCOTT';
```

- 查询与员工编号为7566从事同一工作且领导相同的全部员工信息

```mysql
SELECT * FROM emp WHERE (job,mgr)=(SELECT job,mgr FROM emp WHERE empno=7566)
AND emp<>7566;
```





## 列子查询(多行子查询)

> 子查询返回的是多行单列的数据，就是一列数据。多行子查询也称为集合比较子查询，

在使用多行子查询需要使用多行比较操作符：

| 操作符 | 含义                                                         |
| ------ | ------------------------------------------------------------ |
| IN     | 等于列表中的任意一个                                         |
| ANY    | 需要和单行比较操作符一起使用(>、<、=、<>...)，与子查询结果中任何一个值比较，一个成立 |
| ALL    | 需要和单行比较操作符一起使用(>、<、=、<>...)，和子查询返回的所有值比较，同时成立 |
| SOME   | 实际上是ANY的别名，作用相同，一般用ANY                       |



### IN操作符

IN 运算符用来判断表达式的值是否位于给出的列表中；如果是，返回值为 1，否则返回值为 0。

**NOT IN** 的作用和 IN 恰好相反，NOT IN 用来判断表达式的值是否不存在于给出的列表中；如果不是，返回值为 1，否则返回值为 0。

- 查询出与每个部门中最低工资相同的员工信息
  - 按照部门分组，统计每个部门的最低工资
  - 根据最低工资查询出员工信息

```mysql
SELECT * FROM emp WHERE sal IN(SELECT MIN(sal) FROM emp GROUP BY deptno)
AND deptno IS NOT NULL;
```



### ANY操作符

ANY关键字是一个MySQL运算符，如果子查询条件中ANY的比较结果为TRUE，则它会**返回布尔值TRUE** 。

- 查询工资比任意管理工资高的员工信息
  - 查找出每个管理的薪资
  - 每个员工的薪资与每个管理的薪资比较

```mysql
SELECT * FROM emp WHERE sal >ANY(
SELECT MIN(sal) FROM emp WHERE job='MANAGER' GROUP BY deptno);
```



### ALL操作符

ALL关键字是一个MySQL运算符，如果子查询条件中ALL的比较结果为TRUE，则它会**返回布尔值TRUE** 。

- 案例同ANY操作符

~~~mysql
SELECT * FROM emp WHERE sal >all(
SELECT MIN(sal) FROM emp WHERE job='MANAGER' GROUP BY deptno);
~~~





## 表子查询

> 子查询返回的是多行多列的数据，就是一个表格
>
> 必须使用 IN、ANY 和 ALL 操作符对子查询返回的结果进行比较

- 在emp表中，得到与10号部门任何一个员工入职日期和领导相同的员工信息(用在where子句中)

```mysql
SELECT * FROM emp
WHERE (DATE_FORMAT(hiredate,'%Y'),mgr) IN (SELECT DATE_FORMAT(hiredate,'%Y') hiryear,mgr FROM emp WHERE deptno=10);
```

- 查询出每个部门的编号、名称、位置、部门人数、平均工资（用在from子句中）

```mysql
#以前学的多表联合查询
SELECT d.deptno,d.dname,d.loc,COUNT(e.deptno),ROUND(AVG(sal),2) FROM
dept d LEFT JOIN emp e
ON e.deptno=d.deptno
GROUP BY d.deptno,d.dname,d.loc;

#用子查询联合查询
SELECT dept.deptno,dept.dname,dept.loc,d.count,d.avgsal FROM dept LEFT JOIN 
(SELECT deptno,COUNT(*) count,AVG(sal) avgsal  FROM emp GROUP BY deptno) d
ON dept.deptno=d.deptno;

#不用LEFT JOIN
SELECT d.deptno,d.dname,d.loc,temp.count,temp.avgsal FROM dept d,
(SELECT deptno,COUNT(*) count,ROUND(AVG(sal),2) avgsal FROM emp GROUP BY deptno) temp
WHERE d.deptno=temp.deptno;
```

- 查询出所有在'SALES'部门工作的员工编号、姓名、基本工资、奖金、职位、雇佣日期、部门的最高和最低工资。(where和from子句同时使用)

```mysql
#1
SELECT e.empno,e.ename,e.sal,e.comm,e.job,e.hiredate,minsal,maxsal,e.deptno  
FROM emp e JOIN 
(SELECT deptno,MIN(sal) minsal,MAX(sal) maxsal FROM emp GROUP BY deptno) td
ON e.deptno=td.deptno AND e.deptno=(SELECT deptno FROM dept WHERE dname='SALES');

#2
SELECT e.empno,e.ename,e.sal,e.comm,e.job,e.hiredate,minsal,maxsal,e.deptno  
FROM emp e JOIN 
(SELECT deptno,MIN(sal) minsal,MAX(sal) maxsal FROM emp GROUP BY deptno
HAVING deptno=(SELECT deptno FROM dept WHERE dname='SALES')) td
ON e.deptno=td.deptno;
```

- 查询出比‘ALLEN’或‘CLARK’薪资多的所有员工的编号、姓名、基本工资、部门名称、领导姓名、部门人数。

```mysql
#隐式方式
SELECT e.empno,e.ename,e.sal,d.dname,me.ename 领导,temp.count FROM emp e,dept d,emp me,
(SELECT deptno,COUNT(deptno) count FROM emp e GROUP BY deptno) temp
WHERE e.deptno=d.deptno AND e.mgr=me.empno AND temp.deptno=e.deptno
AND e.sal >ANY(SELECT sal FROM emp WHERE ename IN('ALLEN','CLARK'))
AND e.ename NOT IN('ALLEN','CLARK');

#显示方式
SELECT e.empno,e.ename,e.sal,d.dname,me.ename 领导,temp.count 
FROM emp e 
JOIN dept d ON e.deptno=d.deptno
LEFT JOIN emp me ON e.mgr=me.empno
JOIN (SELECT deptno,COUNT(deptno) count FROM emp e GROUP BY deptno) temp ON temp.deptno=e.deptno
AND e.sal >ANY(SELECT sal FROM emp WHERE ename IN('ALLEN','CLARK'))
AND e.ename NOT IN('ALLEN','CLARK');
```



- 列出公司各个部门的经理(假设每个部门只有一个经理，job为‘MANAGER’)的姓名、薪资、部门名称、部门人数、部门平均工资。

```mysql
#隐式方式
SELECT e.ename,e.sal,d.dname,temp.count,temp.avgsal
FROM emp e,dept d,(SELECT deptno, COUNT(deptno) count,AVG(sal) avgsal FROM emp GROUP BY deptno) temp
 WHERE job='MANAGER' AND e.deptno=d.deptno AND temp.deptno=e.deptno;
 
 #显示方式
SELECT e.ename,e.sal,d.dname,temp.count,temp.avgsal
FROM emp e 
JOIN dept d ON e.deptno=d.deptno 
JOIN (SELECT deptno, COUNT(deptno) count,AVG(sal) avgsal FROM emp GROUP BY deptno) temp ON temp.deptno=e.deptno
 AND job='MANAGER';

```

- 查询出所有薪资高于公司平均薪资的员工编号、姓名、基本工资、职位、雇佣日期、所在部门名称、部门位置、上级领导姓名、工资等级、部门人数、平均工资、平均服务年限。

```mysql
#隐式方式
SELECT e.empno,e.ename,e.sal,e.job,e.hiredate,d.dname,d.loc,me.ename 领导,s.grade,temp.count,temp.avgsal,temp.avgyear
FROM emp e,dept d,emp me,salgrade s,(SELECT deptno,COUNT(deptno) count,AVG(sal) avgsal,AVG(TIMESTAMPDIFF(MONTH,hiredate,CURDATE())/12) avgyear FROM emp GROUP BY deptno) temp 
WHERE e.deptno=d.deptno AND e.sal>(SELECT AVG(sal) FROM emp)
AND e.mgr=me.empno
AND e.sal BETWEEN s.losal AND s.hisal
AND temp.deptno=e.deptno;

#显示方式
SELECT e.empno,e.ename,e.sal,e.job,e.hiredate,d.dname,d.loc,me.ename 领导,s.grade,temp.count,temp.avgsal,temp.avgyear
FROM emp e 
JOIN dept d ON e.deptno=d.deptno AND e.sal>(SELECT AVG(sal) FROM emp)
LEFT JOIN emp me ON e.mgr=me.empno
JOIN salgrade s ON e.sal BETWEEN s.losal AND s.hisal
JOIN (SELECT deptno,COUNT(deptno) count,AVG(sal) avgsal,AVG(TIMESTAMPDIFF(MONTH,hiredate,CURDATE())/12) avgyear FROM emp GROUP BY deptno) temp ON temp.deptno=e.deptno;

```

## 在HAVING子句中使用子查询

可以在on后继续用where，可以在having前(group by 前)用where，但不能用where直接代替on或者having

自己可以仔细想一下，join 和 group by 用在语句中的位置，就可以理解为什么了。

- 查询部门编号、员工人数、平均工资，并且要求这些部门的平均工资高于公司平均薪资。

```mysql
SELECT deptno,COUNT(deptno) cnt,AVG(sal) avgsal 
FROM emp 
GROUP BY deptno
HAVING avgsal>
(
	SELECT AVG(sal) FROM emp
);

```

- 查询出所有部门中平均工资最高的部门名称及平均工资

```mysql
SELECT e.deptno,d.dname,ROUND(AVG(sal),2) avgsal
FROM emp e,dept d
WHERE e.deptno=d.deptno
GROUP BY e.deptno
HAVING avgsal>
(
    #查询出所有部门平均工资中最高的薪资
	 SELECT MAX(avgsal) FROM 
		(SELECT AVG(sal) avgsal FROM emp GROUP BY deptno) AS temp	
    细节：
    //temp不能少
    
   	//不能select max(avg(sal)) from emp group by deptno
)

```



## 在SELECT字句中使用子查询

- 查询出公司每个部门的编号、名称、位置、部门人数、平均工资

```mysql
#1多表查询
SELECT d.deptno,d.dname,d.loc,COUNT(e.deptno),AVG(e.sal)
FROM emp e,dept d
WHERE e.deptno=d.deptno
GROUP BY e.deptno;
#2
SELECT d.deptno,d.dname,d.loc,temp.cnt,temp.avgsal
FROM dept d,(SELECT deptno,COUNT(deptno) cnt,AVG(sal) avgsal FROM emp GROUP BY deptno) temp
WHERE d.deptno=temp.deptno;
#3 关联子查询
SELECT d.deptno,d.dname,d.loc,
(SELECT COUNT(deptno) FROM emp WHERE deptno=d.deptno GROUP BY deptno) cnt,
(SELECT AVG(sal) FROM emp WHERE deptno=d.deptno GROUP BY deptno) avgsal
FROM dept d;

```

# 相关子查询

如果子查询的执行依赖外部查询，通常情况下都是因为子查询中的表用到了外部的表，并进行了条件关联，因此每执行一次外部查询，子查询都要重新计算一次，这样的子查询就成为关联子查询。

相关子查询按照一行接一行的顺序指针，主查询的每一行都指向一次子查询。

![image-20220321154622164](C:/Users/ASUS/Desktop/MySql%E8%AF%BE%E4%BB%B6/07%20%E5%AD%90%E6%9F%A5%E8%AF%A2/assets/image-20220321154622164.png)

## 查询需求

- 查询员工中工资大于本部门平均工资的员工的部门编号、姓名、薪资

```mysql

```



# 其他语句

## WITH字句

- 查询每个部门的编号、名称、位置、部门平均工资、人数

```mysql
-- 多表查询
SELECT d.deptno,d.dname,d.loc,AVG(e.sal) avgsal ,COUNT(e.deptno) cnt
FROM dept d,emp e
WHERE d.deptno=e.deptno
GROUP BY e.deptno;

-- 子查询
SELECT d.deptno,d.dname,d.loc,temp.avgsal,temp.cnt
FROM dept d,(
							SELECT deptno,AVG(sal) avgsal,COUNT(deptno) cnt
							FROM emp
							GROUP BY deptno
						)temp
WHERE d.deptno=temp.deptno;

-- 使用with
WITH temp AS(
							SELECT deptno,AVG(sal) avgsal,COUNT(deptno) cnt
							FROM emp
							GROUP BY deptno
						)
SELECT d.deptno,d.dname,d.loc,temp.avgsal,temp.cnt
FROM dept d,temp
WHERE d.deptno=temp.deptno;
									

```

- 查询每个部门工资最高的员工编号、姓名、职位、雇佣日期、工资、部门编号、部门名称，显示的结果按照部门编号进行排序

```mysql
-- 相关子查询
SELECT e.empno,e.ename,e.job,e.hiredate,e.sal,e.deptno,d.dname
FROM emp e,dept d
WHERE e.deptno=d.deptno
AND e.sal=(SELECT MAX(sal) FROM emp WHERE deptno=e.deptno)
ORDER BY e.deptno;
-- 表子查询
SELECT e.empno,e.ename,e.job,e.hiredate,e.sal,e.deptno,d.dname
FROM emp e,dept d,(SELECT deptno,MAX(sal) maxsal FROM emp GROUP BY deptno) temp
WHERE e.deptno=d.deptno
AND e.sal=temp.maxsal
AND e.deptno = temp.deptno
ORDER BY e.deptno;

```



## EXISTS/NOT EXISTS

在SQL中提供了一个exixts结构用于判断子查询是否有数据返回。如果子查询中有数据返回，exists结构返回true，否则返回false。

- 查询公司管理者的编号、姓名、工作、部门编号

```mysql
-- 多表查询
SELECT DISTINCT e.empno,e.ename,e.job,e.deptno
FROM emp e JOIN emp mgr
ON e.empno=mgr.mgr;
-- 使用EXISTS
SELECT e.empno,e.ename,e.job,e.deptno
FROM emp e
WHERE EXISTS (SELECT * FROM emp WHERE e.empno=mgr);

```

- 查询部门表中，不存在于员工表中的部门信息

```mysql
-- 多表查询
SELECT e.deptno,d.deptno,d.dname,d.loc
FROM emp e RIGHT JOIN dept d
ON e.deptno=d.deptno
WHERE e.deptno IS NULL;

-- 使用EXISTS
SELECT d.deptno,d.dname,d.loc
FROM  dept d
WHERE NOT EXISTS (SELECT deptno FROM emp WHERE deptno=d.deptno);

```





# **第12章 视图**

## 1. 视图概述

![image-20220421181315463](C:/Users/ASUS/Desktop/tools/MySql%E5%88%A0%E5%BA%93%E8%B7%91%E8%B7%AF-%E8%AF%BE%E4%BB%B6/12%20%E8%A7%86%E5%9B%BE/assets/image-20220421181315463.png)

### 1.1 为什么使用视图？

视图一方面可以帮我们使用表的一部分而不是整个表，另一方面也可以针对不同的用户顶置不同的查询视图。比如，针对公司的销售人员，我们只想给他看部分数据，而某些特殊的数据，比如采购价格，则不会提供给他。再比如，员工薪资是个敏感字段，那么只给某个级别以上的人员开放，其他人的查询视图中则不提供这个字段。

### 1.2 视图的理解

- 视图是一种`虚拟表` ，本身是`不具有数据` 的，占用很少的内存空间，它是 SQL 中的一个重要概念。
- 视图建立在已有表的基础上， 视图赖以建立的这些表称为**基表**。

![image-20220421182549882](C:/Users/ASUS/Desktop/tools/MySql%E5%88%A0%E5%BA%93%E8%B7%91%E8%B7%AF-%E8%AF%BE%E4%BB%B6/12%20%E8%A7%86%E5%9B%BE/assets/image-20220421182549882.png)

- 视图的创建和删除只影响视图本身，不影响对应的基表。但是当对视图中的数据进行增加、删除和修改操作时，基表中的数据会相应地发生变化，反之亦然。
- 向视图提供数据内容的语句为SELECT语句，可以将视图理解为存储起来的SELECT语句
- 视图，是向用户提供基表数据的另一种表现形式。通常情况下，小型项目的数据库可以不使用视图，但是在大型项目中，以及数据表比较复杂的情况下，视图的价值就凸显出来了，它可以帮助我们把经常查询的结果集放到虚拟表中，提升使用效率。理解和使用起来都非常方便。

## 2.创建视图

### **2.1 视图语法**

- 完整的创建视图语法

```mysql
CREATE [OR REPLACE] [ALGORITHM = {UNDEFINED | MERGE | TEMPTABLE}] 
VIEW 视图名称 [(字段列表)] 
AS 查询语句 
[WITH [CASCADED|LOCAL] CHECK OPTION]
```

- 超简版本

```mysql
CREATE VIEW 视图名称
AS 查询语句
```

### **2.2 创建单表视图**

**范例：**创建视图“v_emp1”，显示员工的姓名，工作以及薪资。

```mysql
CREATE VIEW v_emp1
AS
SELECT ename,job,sal FROM emps;
```

查询视图：

```mysql
SELECT * FROM v_emp1;
```

针对别名的处理，可以在创建视图的子查询中指定对应的别名

```mysql
CREATE VIEW v_emp1_1
AS
SELECT ename 姓名,job 工作,sal 薪资 FROM emps;
```

也可以在创建视图的视图名称后添加对应的别名字段

```mysql
CREATE OR REPLACE VIEW v_emp1_2(姓名,工作,薪资)
AS
SELECT ename,job,sal FROM emps;
```

我们在创建视图的时候还可以封装不是基表中存在的字段的情况

**范例：**创建视图“v_emp2”,显示每个部门的编号，以及平均工资

```mysql
CREATE VIEW v_emp2(deptno,avg_sal)
AS
SELECT deptno,AVG(sal) FROM emps GROUP BY deptno;
```

### **2.2 创建多表视图**

  上面是基于单表创建的视图，当然我们也可以基于多表查询的结果来封装为对应的视图。

**范例：**创建视图“v_emp_dept”，显示部门编号以及部门人数

```mysql
CREATE VIEW v_emp_dept
AS 
SELECT d.deptno,COUNT(d.deptno) dPersonNum
FROM emps e JOIN depts d
ON e.deptno=d.deptno GROUP BY d.deptno;
```

查询视图

```mysql
SELECT * FROM v_emp_dept;
```

当然别名的处理方式在多表中同样的适用。

### **2.3 基于视图创建视图**

  当我们创建好一张视图之后，还可以在它的基础上继续创建视图。

```mysql
CREATE VIEW v_sal_personNum
AS
SELECT v1.deptno,v1.avg_sal,v2.dPersonNum FROM v_emp2 v1 JOIN v_emp_dept v2 ON v1.deptno=v2.deptno;
```

查看视图

```mysql
select * from v_sal_personNum;
```

到这其实我们能够发现，视图的创建还是非常灵活的。

## **3.查看视图**

语法1：查看数据库的表对象、视图对象

```mysql
SHOW TABLES;
```

语法2：查看视图的结构

```mysql
DESC/DESCRIBE 视图名称;
```

语法3：查看视图的属性信息

```mysql
# 查看视图信息（显示数据表的存储引擎、版本、数据行数和数据大小等）
SHOW TABLE STATUS LIKE '视图名称';
```

执行结果显示，注释Comment为VIEW，说明该表为视图，其他的信息为NULL，说明这是一个[虚表](https://www.zhihu.com/search?q=虚表&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A2354022451})。

语法4：查看视图的详细定义信息

```mysql
SHOW CREATE VIEW 视图名称;
```

## **4.更新视图数据**

### **4.1 一般情况**

  MySQL支持使用INSERT、UPDATE和DELETE语句对视图中的数据进行插入、更新和删除操作。当视图中的数据发生变化时，数据表中的数据也会发生变化，反之亦然。

**范例：**通过视图v_emp1，把姓名为’WARD‘的员工的薪资改为1234。

```mysql
#更新视图的数据，基表中的数据也会修改
UPDATE v_emp1 SET sal=1234 WHERE name='WARD';
#更新基本中的数据，视图中的数据也会修改
UPDATE emps SET sal=1000 WHERE name='WARD';
```

**范例：**通过视图v_emp1，把姓名为’WARD‘的员工删掉

```mysql
DELETE FROM v_emp1 WHERE name='WARD';
```

### **4.2 不可更新的视图**

要使视图可更新，视图中的行和底层基本表中的行之间必须存在 一对一 的关系。另外当视图定义出现如

下情况时，视图不支持更新操作：

- 在定义视图的时候指定了“ALGORITHM = TEMPTABLE”，视图将不支持INSERT和DELETE操作；
- 视图中不包含基表中所有被定义为非空又未指定默认值的列，视图将不支持INSERT操作；
- 在定义视图的SELECT语句中使用了 JOIN联合查询 ，视图将不支持INSERT和DELETE操作；
- 在定义视图的SELECT语句后的字段列表中使用了 数学表达式 或 子查询 ，视图将不支持INSERT，也不支持UPDATE使用了数学表达式、子查询的字段值；
- 在定义视图的SELECT语句后的字段列表中使用 DISTINCT 、 聚合函数 、 GROUP BY 、 HAVING 、 UNION 等，视图将不支持INSERT、UPDATE、DELETE；
- 在定义视图的SELECT语句中包含了子查询，而子查询中引用了FROM后面的表，视图将不支持 INSERT、UPDATE、DELETE；

**范例：**通过视图v_emp2（统计平均薪资的视图），把20号部门平均薪资改为5000

```mysql
UPDATE v_emp2 SET avg_sal=5000 WHERE deptno=20;
-- The target table v_emp2 of the UPDATE is not updatable

DELETE FROM v_emp2 WHERE deptno=10;
-- The target table v_emp2 of the DELETE is not updatable

```

注意：虽然可以更新视图数据，但总的来说，视图作为虚拟表 ，主要用于方便查询 ，不建议更新视图的数据。**对视图数据的更改，都是通过对实际数据表里数据的操作来完成的。**

## **5.修改视图**

方式1：使用CREATE **OR REPLACE** VIEW 子句**修改视图**

```mysql
CREATE OR REPLACE VIEW v_emp1
AS 
SELECT ename,job,sal FROM emps WHERE deptno=20;

```

方式2：ALTER VIEW

```mysql
ALTER VIEW v_emp1
AS 
SELECT ename,job,sal FROM emps WHERE deptno=10;

```

## **6.删除视图**

删除视图只是删除视图的定义，并不会删除基表的数据。

删除视图的语法是：

```mysql
DROP VIEW IF EXISTS 视图名称;
DROP VIEW IF EXISTS 视图名称1,视图名称2,视图名称3,...;

```

举例：

```mysql
DROP VIEW v_emp1;

```

说明：基于视图a、b创建了新的视图c，如果将视图a或者视图b删除，会导致视图c的查询失败。这样的视图c需要手动删除或修改，否则影响使用。

# 第13章 存储过程与函数

MySql从5.0版本开始支持存储过程和函数，存储过程和函数能够将复杂的SQL逻辑封装在一起，应用程序无须关注存储过程和函数内部复杂的SQL逻辑，而只需要简单地调用存储过程和函数即可。

## 1. 存储过程概述

### 1.1概念

存储过程(Stroed Procedure)，它的思想很简单，他就是一组经过预先编译的SQL语句的封装。

存储过程预先存储在MySQL服务器上，需要执行的时候，客户端只需要向服务器发出调用存储过程的命令，服务器就可以把预先存储好的这一系列SQL语句全部执行。

**好处：**

- 简化操作，提高了SQL语句的重用性，减少了开发程序员的压力
- 减少操作过程中的失误，提高效率
- 减少网络传输量(客户端不需要把所有的SQL语句通过网络发送给服务器)
- 减少了SQL语句暴露在网上的风险，也提高了数据查询的安全性

## 2. 创建存储过程

### 2.1 语法分析

```mysql
CREATE PROCEDURE [IF NOT EXISTS] 存储过程名([IN|OUT|INOUT 参数名 参数类型,...])
[characteristics ...]
BEGIN
	存储过程体
END
```

> 存储过程名不能与已经存在的存储过程名重名。推荐存储过程名命名为procedure_xxx 或者 proc_xxx。

**参数分类**

存储过程的参数可以是IN、OUT、INOUT。

- IN 表示传入参数
- OUT 表示传出参数
- INOUT 表示即可以传入也可以传出
- 也可以不写参数
- 也可以多多个不同类型的参数

**charcateristics可选参数为：**

```mysql
LANGUAGE SQL
| [NOT] DETERMINISTIC
| {CONTAINS SQL|NO SQL|READS SQL DATA|MODIFIES SQL DATA}
| SQL DECURITY {DEFINER|INVOKER}
| COMMENT 'string'
```

- `LANGUAGE SQL` ：说明存储过程执行体是由SQL语句组成的，当前系统支持的语言为SQL。
- `[NOT] DETERMINISTIC`：指明存储过程执行的结果是否确定。DETERMINISTIC表示结果是确定的。每次执行存储过程时，相同的输入会得到相同的输出。NOT  DETERMINISTIC表示结果是不确定的，相同的输入可能得到不同的输出。如果没有指定任意一个值，默认为NOT DETERMINISTIC。
- `{ CONTAINS SQL | NO SQL | READS SQL DATA | MODIFIES SQL DATA }` ：指明子程序使用SQL语句的限制。
  - CONTAINS SQL表示当前存储过程的子程序包含SQL语句，但是并不包含读写数据的SQL语句； 
  - NO SQL表示当前存储过程的子程序中不包含任何SQL语句；
  - READS SQL DATA表示当前存储过程的子程序中包含读数据的SQL语句； 
  - MODIFIES SQL DATA表示当前存储过程的子程序中包含写数据的SQL语句。
  - 默认情况下，系统会指定为CONTAINS SQL。
- `SQL SECURITY {DEFINER | INVOKER}`：执行当前存储过程的权限，即指明哪些用户能够执行当前存储过程。
  - `DEFINER` 表示只有当前存储过程的创建者或者定义者才能执行当前存储过程；
  - `INVOKER` 表示拥有当前存储过程的访问权限的用户能够执行当前存储过程。
  - 如果没有设置相关的值，则MySQL默认指定值为DEFINER。
- `COMMENT 'string'`注释信息，可以用来描述存储过程。

4、存储过程体中可以有多条 SQL 语句，如果仅仅一条SQL 语句，则可以省略 BEGIN 和 END

编写存储过程并不是一件简单的事情，可能存储过程中需要复杂的 SQL 语句。

```mysql
BEGIN ... END ：中间包含了多个语句，每个语句都以（;）号为结束符。
DECLARE：DECLARE 用来声明变量，使用的位置在于 BEGIN ... END 语句中间，而且需要在其他语句使用之前进行变量的声明。
SET：赋值语句，用于对变量进行赋值。
SELECT… INTO：把从数据表中查询的结果存放到变量中，也就是为变量赋值。
```

5、需要设置新的结束标记

```mysql
DELIMITER 新的结束标记
```

因为MySQL默认的语句结束符号为分号‘;’。为了避免与存储过程中SQL语句结束符相冲突，需要使DELIMITER改变存储过程的结束符。

比如：“DELIMITER //”语句的作用是将MySQL的结束符设置为//，并以“END  //”结束存储过程。存储过程定义完毕之后再使用“DELIMITER ;”恢复默认结束符。DELIMITER也可以指定其他符号作为结束符。

如果使用的是 Navicat 工具，那么在编写存储过程的时候，Navicat 会自动设置 DELIMITER 为其他符号， 我们不需要再进行 DELIMITER 的操作。

> 当使用DELIMITER命令时，应该避免使用反斜杠（‘\’）字符，因为反斜线是MySQL的转义字符。  

```mysql
DELIMITER $

CREATE PROCEDURE 存储过程名(IN|OUT|INOUT 参数名 参数类型,...) 
[characteristics ...]
BEGIN
	sql语句1; 
	sql语句2;

END $
DELIMITER;
```

 

### **2.2** 代码举例

1. 创建存储过程proc_show_emps()，查看 emps 表的所有数据

```mysql
DELIMITER $
CREATE PROCEDURE proc_show_emps() 
BEGIN
	SELECT * FROM emps; 
END $
DELIMITER ;
```



2. 创建存储过程proc_emps_avgsal()，查看所有员工的平均工资

```mysql
DELIMITER //

CREATE PROCEDURE proc_emps_avgsal()
BEGIN
	SELECT AVG(sal) AS avg_sal FROM emps; 
END //

DELIMITER ;
```



3. 创建存储过程proc_emps_maxsal()，用来查看“emps”表的最高薪资值。

```mysql
CREATE PROCEDURE proc_emps_maxsal() 
LANGUAGE SQL
NOT DETERMINISTIC CONTAINS SQL
SQL SECURITY DEFINER
COMMENT '查看最高薪资' 
BEGIN
	SELECT MAX(sal) FROM emps; 
END //

DELIMITER ;
```

4. 创建存储过程proc_emps_minsal()，查看“emps”表的最低薪资值。并将最低薪资通过OUT参数“ms”   输出

```mysql
DELIMITER //

CREATE PROCEDURE proc_emps_minsal(OUT minsal DOUBLE) 
BEGIN
	SELECT MIN(sal) INTO minsal FROM emps; 
END //

DELIMITER ;
#调用方式
CALL proc_emps_minsal(@minsal);
SELECT @minsal;
```



5. 创建存储过程proc_someone_sal()，查看“emps”表的某个员工的薪资，并用IN参数empname   输入员工姓名。

```mysql
DELIMITER //

CREATE PROCEDURE proc_someone_sal(IN empname VARCHAR(20)) 
BEGIN
	SELECT sal FROM emps WHERE ename = empname; 
END //

DELIMITER ;

#调用方式1
CALL proc_someone_sal('WARD');
#调用方式2
SET @empname = 'WARD';
CALL proc_someone_sal(empname);
```



6. 创建存储过程proc_someone_sal2()，查看“emps”表的某个员工的薪资，并用IN参数empname 传入员工姓名，用OUT参数empsal传出员工薪资。

```mysql
DELIMITER //

CREATE PROCEDURE proc_someone_sal2(IN empname VARCHAR(20),OUT empsal DOUBLE) 
BEGIN
	SELECT salary INTO empsal FROM emps WHERE ename = empname; 
END //

DELIMITER ;
```



7. 创建存储过程proc_show_mgr_name()，查询某个员工领导的姓名，并用INOUT参数“empname”输入员工姓名，直接输出领导的姓名。

```mysql
DELIMITER //

CREATE PROCEDURE show_mgr_name(INOUT empname VARCHAR(20)) 
BEGIN
	SELECT ename INTO empname FROM emps
	WHERE empno = (SELECT mgr FROM emps WHERE ename=empname); 
END //

DELIMITER ;
```



# **3.** 调用存储过程

## **3.1** 调用格式

存储过程有多种调用方法。存储过程必须使用CALL语句调用，并且存储过程和数据库相关，如果要执行  其他数据库中的存储过程，需要指定数据库名称，例如CALL dbname.procname。

```mysql
CALL 存储过程名(实参列表)
```

 **格式：**

1、调用IN模式参数：

```mysql
CALL proc_show('值')
```

2、调用OUT模式参数：

```mysql
CALL proc_show(@name);
SELECT @name;
```

3、调用INOUT模式参数：

```mysql
SET @name=值;
CALL proc_show(@name);
SELECT @name;
```



## **3.2** 代码举例

**举例：**传入指定的部门编号，返回该部门人数

```mysql
DELIMITER //

CREATE PROCEDURE proc_count(IN dno INT,OUT num INT) 
BEGIN
	SELECT COUNT(*) INTO num FROM emps WHERE deptno = dno;
END //

DELIMITER ;
```

调用及查看结果：

```mysql
SET @cnt;
CALL proc_count(20,@cnt);
SELECT @cnt;
```

## **3.3** 如何调试

在 MySQL 中，存储过程不像普通的编程语言（比如 C++、Java 等）那样有专门的集成开发环境。因此，你可以通过 SELECT 语句，把程序执行的中间结果查询出来，来调试一个 SQL 语句的正确性。

调试成功之后，把 SELECT 语句后移到下一个 SQL 语句之后，再调试下一个 SQL 语句。这样 逐步推进 ，就可以完成对存储过程中所有操作的调试了。当然，你也可以把存储过程中的  SQL 语句复制出来，逐段单独调试。



# **4.** 存储函数的使用

前面学习了很多函数，使用这些函数可以对数据进行的各种处理操作，极大地提高用户对数据库的管理  效率。MySQL支持自定义函数，定义好之后，调用方式与调用MySQL预定义的系统函数一样。

## **4.1** 语法分析

学过的函数：LENGTH、SUBSTR、CONCAT等

语法格式：

```mysql
CREATE FUNCTION 函数名(参数名 参数类型,...)
RETURNS 返回值类型
[characteristics ...] 
BEGIN
	函数体	#函数体中肯定有 RETURN 语句
END
```

说明：

1、参数列表：指定参数为IN、OUT或INOUT只对PROCEDURE是合法的，FUNCTION中总是默认为IN参数。

2、RETURNS type 语句表示函数返回数据的类型；

> RETURNS子句只能对FUNCTION做指定，对函数而言这是 强制 的。它用来指定函数的返回类型，而且函数体必须包含一个 RETURN value 语句。

3、characteristic 创建函数时指定的对函数的约束。取值与创建存储过程时相同，这里不再赘述。

4、函数体也可以用BEGIN…END来表示SQL代码的开始和结束。如果函数体只有一条语句，也可以省略BEGIN…END。

## **4.2** 调用存储函数

在MySQL中，存储函数的使用方法与MySQL内部函数的使用方法是一样的。换言之，用户自己定义的存  储函数与MySQL内部函数是一个性质的。区别在于，存储函数是 用户自己定义 的，而内部函数是MySQL的开发者定义的。

```mysql
SELECT 函数名(实参列表) 
```

## 4.3 代码举例

1. 创建存储函数，名称为fun_hiredate()，参数定义为空，该函数查询WARD的入职日期，并返回，数据类型为日期类型。

```mysql
DELIMITER //

CREATE FUNCTION fun_hiredate() 
RETURNS DATE 
DETERMINISTIC
CONTAINS SQL 
BEGIN
	RETURN (SELECT hiredate FROM emps WHERE last_name = 'WARD'); 
END //

DELIMITER ;
```

调用：

```mysql
 SELECT email_by_name();
```

2. 创建存储函数，名称为fun_getsal()，参数传入empno，该函数查询empno的sal，并返回，数据类型为SMALLINT。

```mysql
DELIMITER //

CREATE FUNCTION fun_getsal(eno INT) 
RETURNS VARCHAR(25)
DETERMINISTIC 
CONTAINS SQL 
BEGIN
	RETURN (SELECT sal FROM emps WHERE empno = eno); 
END //

DELIMITER ;
```

调用：

```mysql
SET @eno = 102; 
SELECT fun_getsal(@eno);
```

3. 创建存储函数fun_dept_count()，参数传入deptno，该函数查询deptno部门的员工人数，并返回，数据类型为整型。

```mysql
DELIMITER //

CREATE FUNCTION fun_dept_count(dno INT) 
RETURNS INT
LANGUAGE SQL
NOT DETERMINISTIC 
READS SQL DATA
SQL SECURITY DEFINER
COMMENT '查询部门平均工资' 
BEGIN
	RETURN (SELECT COUNT(*) FROM emps WHERE deptno = dno); 
END //
DELIMITER ;
```

调用：

```mysql
SET @deptno = 50;
SELECT fun_dept_count(@dept_id); 
```

**注意：**

 若在创建存储函数中报错`“ you might want to use the less safe log_bin_trust_function_creators variable ”`，有两种处理方法：

- 方式1：加上必要的函数特性“[NOT] DETERMINISTIC”和“{CONTAINS SQL | NO SQL | READS SQL DATA | MODIFIES SQL DATA}”

- 方式2：

  ```mysql
  SET GLOBAL log_bin_trust_function_creators = 1;
  ```

## 4.4 对比存储函数和存储过程

|          | **关键字** | **调用语法**    | **返回值**        | **应用场景**                     |
| -------- | ---------- | --------------- | ----------------- | -------------------------------- |
| 存储过程 | PROCEDURE  | CALL 存储过程() | 理解为有0个或多个 | 一般用于更新                     |
| 存储函数 | FUNCTION   | SELECT 函数()   | 只能是一个        | 一般用于查询结果为一个值并返回时 |

此外，**存储函数可以放在查询语句中使用，存储过程不行**。反之，存储过程的功能更加强大，包括能够  执行对表的操作（比如创建表，删除表等）和事务操作，这些功能是存储函数不具备的。



# 5. 存储过程和函数的查看、修改、删除

## **5.1** 查看

创建完之后，怎么知道我们创建的存储过程、存储函数是否成功了呢？

MySQL存储了存储过程和函数的状态信息，用户可以使用SHOW  STATUS语句或SHOW  CREATE语句来查看，也可直接从系统的information_schema数据库中查询。这里介绍3种方法。

<font style="color:red">1. 使用SHOW CREATE语句查看存储过程和函数的创建信息</font>

基本语法结构如下：

```mysql
SHOW CREATE {PROCEDURE | FUNCTION} 存储过程名或函数名
```

举例：

```mysql
SHOW CREATE FUNCTION db_13.fun_getsal;
```

 <font style="color:red">2. 使用 SHOW STATUS 语句查看存储过程和函数的状态信息</font>

基本语法结构如下：

```mysql
SHOW {PROCEDURE | FUNCTION} STATUS [LIKE 'pattern']
```

这个语句返回子程序的特征，如数据库、名字、类型、创建者及创建和修改日期。

[LIKE  'pattern']：匹配存储过程或函数的名称，可以省略。当省略不写时，会列出MySQL数据库中存在的所有存储过程或函数的信息。 举例：SHOW STATUS语句示例，代码如下：

```mysql
SHOW PROCEDURE STATUS LIKE 'SELECT%';
```

 <font style="color:red">2. 从information_schema.Routines表中查看存储过程和函数的信息</font>

MySQL中存储过程和函数的信息存储在information_schema数据库下的Routines表中。可以通过查询该表   的记录来查询存储过程和函数的信息。其基本语法形式如下：

```mysql
SELECT * FROM information_schema.Routines
WHERE ROUTINE_NAME='存储过程或函数的名' [AND ROUTINE_TYPE = {'PROCEDURE|FUNCTION'}];
```

说明：如果在MySQL数据库中存在存储过程和函数名称相同的情况，最好指定ROUTINE_TYPE查询条件来   指明查询的是存储过程还是函数。

举例：从Routines表中查询名称为proc_count的存储函数的信息，代码如下：

```mysql
SELECT * FROM information_schema.Routines
WHERE ROUTINE_NAME='count_by_id' AND ROUTINE_TYPE = 'FUNCTION';
```

## 5.2 修改

修改存储过程或函数，不影响存储过程或函数功能，只是修改相关特性。使用ALTER语句实现。

```mysql
ALTER {PROCEDURE | FUNCTION} 存储过程或函数的名 [characteristic ...]
```

其中，characteristic指定存储过程或函数的特性，其取值信息与创建存储过程、函数时的取值信息略有  不同。

```mysql
{ CONTAINS SQL | NO SQL | READS SQL DATA | MODIFIES SQL DATA }
| SQL SECURITY { DEFINER | INVOKER }
| COMMENT 'string'
```

- CONTAINS SQL ，表示子程序包含SQL语句，但不包含读或写数据的语句。
- NO SQL ，表示子程序中不包含SQL语句。
- READS SQL DATA ，表示子程序中包含读数据的语句。
- MODIFIES SQL DATA ，表示子程序中包含写数据的语句。
- SQL SECURITY { DEFINER | INVOKER }，指明谁有权限来执行。
  - DEFINER ，表示只有定义者自己才能够执行。
  - INVOKER ，表示调用者可以执行。
- COMMENT 'string' ，表示注释信息。

> 修改存储过程使用ALTER PROCEDURE语句，修改存储函数使用ALTER FUNCTION语句。但是，这两个语句的结构是一样的，语句中的所有参数也是一样的。

 

**举例1：**修改存储过程pro_coutn的定义。将读写权限改为MODIFIES  SQL  DATA，并指明调用者可以执行，代码如下：

```mysql
ALTER PROCEDURE CountProc 
MODIFIES SQL DATA
SQL SECURITY INVOKER ;
```

查询修改后的信息：

```mysql
SELECT specific_name,sql_data_access,security_type 
FROM information_schema.`ROUTINES`
WHERE routine_name = 'CountProc' AND routine_type = 'PROCEDURE';
```

结果显示，存储过程修改成功。从查询的结果可以看出，访问数据的权限（SQL_DATA_   ACCESS）已经变成MODIFIES SQL DATA，安全类型（SECURITY_TYPE）已经变成INVOKER。

**举例2：**修改存储函数CountProc的定义。将读写权限改为READS  SQL  DATA，并加上注释信息“getsal”，代码如下：

```mysql
ALTER FUNCTION fun_getsal 
READS SQL DATA
COMMENT 'getsal' ;
```

存储函数修改成功。从查询的结果可以看出，访问数据的权限（SQL_DATA_ACCESS）已经变成READS SQL DATA，函数注释（ROUTINE_COMMENT）已经变成FIND NAME。

## **5.3** 删除

删除存储过程和函数，可以使用DROP语句，其语法结构如下：

```mysql
DROP {PROCEDURE | FUNCTION} [IF EXISTS] 存储过程或函数的名
```

IF  EXISTS：如果程序或函数不存储，它可以防止发生错误，产生一个用SHOW  WARNINGS查看的警告。

举例：

```mysql
DROP PROCEDURE getsal;
DROP FUNCTION getsal;
```

 

# **6.** 关于存储过程使用的争议

尽管存储过程有诸多优点，但是对于存储过程的使用，**一直都存在着很多争议**，比如有些公司对于大型  项目要求使用存储过程，而有些公司在手册中明确禁止使用存储过程，为什么这些公司对存储过程的使用需求差别这么大呢？

## **6.1** 优点

<font style="color:rgb(135,0,0)">1、**存储过程可以一次编译多次使用**。</font>存储过程只在创建时进行编译，之后的使用都不需要重新编译，这就提升了 SQL 的执行效率。

<font style="color:rgb(135,0,0)">2、**可以减少开发工作量**。</font>将代码封装成模块，实际上是编程的核心思想之一，这样可以把复杂的问题拆解成不同的模块，然后模块之间可以  重复使用 ，在减少开发工作量的同时，还能保证代码的结构清晰。

<font style="color:rgb(135,0,0)">3、**存储过程的安全性强**。</font>我们在设定存储过程的时候可以 设置对用户的使用权限 ，这样就和视图一样具有较强的安全性。

<font style="color:rgb(135,0,0)">4、**可以减少网络传输量**。</font>因为代码封装到存储过程中，每次使用只需要调用存储过程即可，这样就减 少了网络传输量。

<font style="color:rgb(135,0,0)">5、**良好的封装性**。</font>在进行相对复杂的数据库操作时，原本需要使用一条一条的 SQL  语句，可能要连接多次数据库才能完成的操作，现在变成了一次存储过程，只需要 连接一次即可 。

## 6.2 缺点

基于上面这些优点，不少大公司都要求大型项目使用存储过程，比如微软、IBM  等公司。但是国内的阿里并不推荐开发人员使用存储过程，这是为什么呢？

> **阿里开发规范**
>
> 【强制】禁止使用存储过程，存储过程难以调试和扩展，更没有移植性。

存储过程虽然有诸如上面的好处，但缺点也是很明显的。

<font style="color:rgb(135,0,0)">1、**可移植性差**。</font>存储过程不能跨数据库移植，比如在 MySQL、Oracle 和 SQL Server 里编写的存储过程，在换成其他数据库时都需要重新编写。

<font style="color:rgb(135,0,0)">2、**调试困难**。</font>只有少数 DBMS  支持存储过程的调试。对于复杂的存储过程来说，开发和维护都不容易。虽然也有一些第三方工具可以对存储过程进行调试，但要收费。

<font style="color:rgb(135,0,0)">3、**存储过程的版本管理很困难**。</font>比如数据表索引发生变化了，可能会导致存储过程失效。我们在开发  软件的时候往往需要进行版本管理，但是存储过程本身没有版本控制，版本迭代更新的时候很麻烦。

<font style="color:rgb(135,0,0)">4、**它不适合高并发的场景**。</font>高并发的场景需要减少数据库的压力，有时数据库会采用分库分表的方式，而且对可扩展性要求很高，在这种情况下，存储过程会变得难以维护， 增加数据库的压力 ，显然就不适用了。

<font style="color:rgb(135,0,0)">**小结**：</font>

存储过程既方便，又有局限性。尽管不同的公司对存储过程的态度不一，但是对于我们开发人员来说，  不论怎样，掌握存储过程都是必备的技能之一。

# 第14章 变量、流程控制与游标

## 1. 变量

在MySQL数据库的存储过程和函数中，可以使用变量来存储查询或计算的中间结果数据，或者输出最终的结果数据。

在 MySQL 数据库中，变量分为 `系统变量`以及`用户自定义变量`。

### **1.1** 系统变量

#### **1.1.1** 系统变量分类

变量由系统定义，不是用户定义，属于服务器层面。启动MySQL服务，生成MySQL服务实例期间， MySQL将为MySQL服务器内存中的系统变量赋值，这些系统变量定义了当前MySQL服务实例的属性、特  征。这些系统变量的值要么是 编译MySQL时参数 的默认值，要么是 配置文件 （例如my.ini等）中的参数值。大家可以在官网[Server System Variables](https://dev.mysql.com/doc/refman/8.0/en/server-system-variables.html)中查看MySQL文档的系统变量。

系统变量分为**全局系统变量**（需要添加`global`关键字）以及会话系统变量（需要添加`session`关键字），有时也把全局系统变量简称为全局变量，有时也把会话系统变量称为local变量。**如果不写，默认是会话级别。**静态变量（在 MySQL 服务实例运行期间它们的值不能使用 set 动态修改）属于特殊的全局系统变量。

每一个MySQL客户机成功连接MySQL服务器后，都会产生与之对应的会话。会话期间，MySQL服务实例  会在MySQL服务器内存中生成与该会话对应的会话系统变量，这些会话系统变量的初始值是全局系统变  量值的复制。如下图：

![img](E:/Typora/MySQL/14%20%E5%8F%98%E9%87%8F%E3%80%81%E6%B5%81%E7%A8%8B%E6%8E%A7%E5%88%B6%E4%B8%8E%E6%B8%B8%E6%A0%87/assets/wps10.jpg)

 

- 全局系统变量针对于所有会话（连接）有效，但不能跨重启。
- 会话系统变量仅针对于当前会话（连接）有效。会话期间，当前会话对某个会话系统变量值的修改，不会影响其他会话同一个会话系统变量的值。
- 会话1对某个全局系统变量值的修改会导致会话2中同一个全局系统变量值的修改。

在MySQL中有些系统变量只能是全局的，例如  max_connections  用于限制服务器的最大连接数；有些系统变量作用域既可以是全局又可以是会话，例如 character_set_client  用于设置客户端的字符集；有些系统变量的作用域只能是当前会话，例如 pseudo_thread_id 用于标记当前会话的 MySQL 连接 ID。

#### **1.1.2** **查看系统变量**

- <font style="color:rgb(135,0,58)">**查看所有或部分系统变量**</font>

  ```mysql
  #查看所有全局变量
  SHOW GLOBAL VARIABLES;
  
  #查看所有会话变量
  SHOW SESSION VARIABLES;
  #或
  SHOW VARIABLES;
  ```

  ```mysql
  #查看满足条件得部分系统变量
  SHOW GLOBAL VARIABLES LIKE '%标识符%';
  
  #查看满足条件的部分会话变量
  SHOW SESSION VARIABLES LIKE '%标识符%'
  ```

  **范例：**

  ```mysql
  SHOW GLOBAL VARIABLES LIKE 'admin_%';
  ```

- <font style="color:rgb(135,0,58)">**查看指定系统变量**</font>

作为 MySQL 编码规范，MySQL 中的系统变量以 两个“@” 开头，其中“@@global”仅用于标记全局系统变量，“@@session”仅用于标记会话系统变量。“@@”首先标记会话系统变量，如果会话系统变量不存在，  则标记全局系统变量。

```mysql
#查看指定的系统变量的值
SELECT @@global.变量名;

#查看指定的会话变量的值
SELECT @@session.变量名;
#或
SELECT @@变量名;
```

- <font style="color:rgb(135,0,58)">**修改系统变量的值**</font>

有些时候，数据库管理员需要修改系统变量的默认值，以便修改当前会话或者MySQL服务实例的属性、  特征。具体方法：

方式1：修改MySQL 配置文件 ，继而修改MySQL系统变量的值（该方法需要重启MySQL服务） 

方式2：在MySQL服务运行期间，使用“set”命令重新设置系统变量的值

```mysql
#为某个系统变量赋值
#方式1：
SET @@global.变量名=值;
#方式2：
SET GLOBAL 变量名=值;

#为某个会话变量赋值
#方式1：
SET @@session.变量名=值;
#方式2：
SET SESSION 变量名=值
```

**范例：**

```mysql
SELECT @@global.autocommit;
SET GLOBAL autocommit=0;
```

```mysql
SELECT @@session.tx_isolation;
SET @@session.tx_isolation='read-uncommitted';
```

```mysql
SET GLOBAL max_connections = 1000;
SELECT @@global.max_connections;
```

#### 1.1.3 补充：MySQL 8.0的新特性——全局变量的持久化

在MySQL数据库中，全局变量可以通过SET  GLOBAL语句来设置。例如，设置服务器语句超时的限制，可以通过设置系统变量max_execution_time来实现：

```mysql
SET GLOBAL MAX_EXECUTION_TIME=2000;
```

使用SET GLOBAL语句设置的变量值只会 临时生效 。 数据库重启 后，服务器又会从MySQL配置文件中读取变量的默认值。 MySQL 8.0版本新增了 SET PERSIST 命令。例如，设置服务器的最大连接数为1000：

```mysql
SET PERSIST global max_connections = 1000;
```

MySQL会将该命令的配置保存到数据目录下的 mysqld-auto.cnf 文件中，下次启动时会读取该文件，用其中的配置来覆盖默认的配置文件。

举例：

查看全局变量max_connections的值，结果如下：

```mysql
mysql> show variables like '%max_connections%';
+------------------------+-------+
| Variable_name	| Value  |
+------------------------+-------+
| max_connections	| 151  |
| mysqlx_max_connections | 100|
+------------------------+-------+
2 rows in set, 1 warning (0.00 sec)
```

设置全局变量max_connections的值：

```mysql
mysql> set persist max_connections=1000; 
Query OK, 0 rows affected (0.00 sec)
```

`重启MySQL服务器 `，再次查询max_connections的值：

```mysql
mysql> show variables like '%max_connections%';
+------------------------+-------+
| Variable_name	| Value |
+------------------------+-------+
| max_connections	| 1000 |
| mysqlx_max_connections | 100	|
+------------------------+-------+
2 rows in set, 1 warning (0.00 sec)
```



### 1.2 用户变量

#### 1.2.1 用户变量分类

用户变量是用户自己定义的，作为 MySQL 编码规范，MySQL 中的用户变量以 一个“@” 开头。根据作用范围不同，又分为`会话用户变量`和`局部变量`。

- 会话用户变量：作用域和会话变量一样，只对当前连接会话有效。
- 局部变量：只在 BEGIN 和 END 语句块中有效。局部变量只能在存储过程和函数中使用。

注意：在 MySQL 5.0以前的版本中是大小写敏感的，因此要注意（在 MySQL 5.0以后已经不区分大小写了）。

#### 1.2.2 会话用户变量

- 变量的定义

```mysql
#方式1："= 或 ":=
SET @用户变量=值
SET @用户变量:=值

#方式2：":=" 或 INTO关键字
SELECT @用户变量 :=表达式[FROM等子句];
SELECT 表达式 INTO @用户变量 [FROM等子句];
```

- 查看自定义变量的值

```mysql
SELECT @用户变量;
```

**范例：**

```mysql
SET @n1 =1;
SET @n2 :=3;
SET @sum := @n1+@n2;
SELECT @sum;
```

```mysql
SELECT @num := COUNT(*) FROM emps;
SELECT @num;
```

```mysql
SELECT AVG(sal) INTO @avgsal FROM emps;
SELECT  @avgsal;
```

```mysql
#查看某个未定义的变量时，将得到NULL值
SELECT @maye;
```

#### 1.2.3 局部变量

定义：可以使用 DECLARE 语句定义一个局部变量作用域：仅仅在定义它的 BEGIN ... END 中有效

位置：只能放在 BEGIN ... END 中，而且**只能放在第一句**

```mysql
BEGIN
	#声明局部变量
	DECLARE 变量1 数据类型 [DEFAULT 默认值];
	DECLARE 变量2,变量3,... 数据类型 [DEFAULT 默认值];
	
	#为局部变量赋值
	SET 变量1 = 值;
	SELECT 字段 INTO 变量2 [FROM 子句];
	
	#查看局部变量的值
	SELECT 变量1,变量2,变量3;
END	
```

##### 1. 定义变量

```mysql
DECLARE 变量名 类型 [DEFAULT 值]; # 如果没有DEFAULT子句，初始值为NULL
```

**范例：**

```mysql
DECLARE num INT DEFAULT 100;
```

##### 2.变量赋值

 方式1：一般用于赋简单的值

```mysql
SET 变量=值;
SET 变量:=值;
```

方式2：一般用于赋表中的字段值

```mysql
SELECT 字段名或表达式 INTO 变量名 FROM 表;
```

##### 3.使用变量

```mysql
SELECT 局部变量名;
```

**范例1：**声明局部变量，并分别赋值为emps表中empno为7369的ename和sal。

```mysql
CREATE PROCEDURE set_value() 
BEGIN
	DECLARE emp_name VARCHAR(25); 
	DECLARE em_sal DOUBLE(10,2);

	SELECT ename,sal INTO emp_name,emp_sal 
	FROM emps
	WHERE empno = 7369;

	SELECT emp_name,emp_sal; 
END;
```

**范例2：**声明两个变量，求和并打印 （分别使用会话用户变量、局部变量的方式实现）

```mysql
#方式1：使用用户变量
SET @m=1;
SET @n=2;
SET @sum = @m+@n;
SELECT @sum;
```

```mysql
#方式2：使用局部变量
CREATE PROCEDURE add_value()
BEGIN
	DECLARE m INT DEFAULT 1;
	DECLARE n INT DEFAULT 3;
	DECLARE sum INT;
	
	SET sum=m+n;
	SELECT sum;
END;
```

**范例3：**创建存储过程“diff_sal”查询某员工和他领导的薪资差距，并用IN参数eno接收员工编号，用OUT参数dif_sal输出薪资差距结果。

```mysql
CREATE PROCEDURE different_sal(IN eno INT,OUT dif_sal DOUBLE) 
BEGIN
	#声明局部变量
	DECLARE emp_sal,mgr_sal DOUBLE DEFAULT 0.0; 
	DECLARE mgr_no INT;

	SELECT sal INTO emp_sal FROM emps WHERE empno = eno; 	
	SELECT mgr INTO mgr_no FROM emps WHERE empno = eno; 
	SELECT sal INTO mgr_sal FROM emps WHERE empno = mgr_no;
	SET dif_sal = mgr_sal - emp_sal; 
END;

#调用
SET @emp_no = 102;
CALL different_salary(@emp_no,@diff_sal);

#查看
SELECT @diff_sal;
```



#### 1.2.4 会话用户变量与局部变量对比

|   变量类型   |       作用域        |        定义位置        |           语法            |
| :----------: | :-----------------: | :--------------------: | :-----------------------: |
| 会话用户变量 |   作用域当前会话    | 定义位置会话的任何地方 | 语法加@符号，不用指定类型 |
|   局部变量   | 定义它的BEGIN END中 |  BEGIN END的第一句话   |   不用加@，需要指定类型   |

 

## 2. 定义条件与处理程序

定义条件是事先定义程序执行过程中可能遇到的问题， 处理程序定义了在遇到问题时应当采取的处理方式，并且保证存储过程或函数在遇到警告或错误时能继续执行。这样可以增强存储程序处理问题的能力，避免程序异常停止运行。

说明：定义条件和处理程序在存储过程、存储函数中都是支持的。

### 2.1 案例分析

**案例分析：**创建一个名称为“UpdateData”的存储过程。代码如下： 

```mysql
alter table emps modify sal smallint not null;

CREATE PROCEDURE UpdateData() 
BEGIN
	SET @x = 1;
	UPDATE emps SET sal = NULL WHERE ename = 'WARD'; 
	SET @x = 2;
	UPDATE emps SET sal = 200 WHERE ename = 'WARD'; 
	SET @x = 3;
END ;
```

调用存储过程：

```mysql
mysql> CALL UpdateData();
ERROR 1048 (23000): Column 'sal' cannot be null

mysql> SELECT @x;
+------+
| @x	|
+------+
|	1 |
+------+
1 row in set (0.00 sec)
```

可以看到，此时@x变量的值为1。结合创建存储过程的SQL语句代码可以得出：在存储过程中未定义条件  和处理程序，且当存储过程中执行的SQL语句报错时，MySQL数据库会抛出错误，并退出当前SQL逻辑，  不再向下继续执行。

### 2.2  定义条件

定义条件就是给MySQL中的错误码命名，这有助于存储的程序代码更清晰。它将一个错误名字和错误条件关联起来。这个名字可以随后被用在定义处理程序的 DECLARE HANDLER 语句中。

定义条件使用DECLARE语句，语法格式如下：

```mysql
DECLARE 错误名称 CONDITION FOR 错误码(或错误条件)
```

错误码的说明：

- `MySQL_error_code` 和 `sqlstate_value` 都可以表示MySQL的错误。
  - MySQL_error_code是数值类型错误代码。
  - sqlstate_value是长度为5的字符串类型错误代码。
- 例如，在ERROR 1048 (23000)中，1048是MySQL_error_code，'23000'是sqlstate_value。
- 例如，在ERROR 1146（42S02）中，1146是MySQL_error_code，'42S02'是sqlstate_value。

**范例1：**定义“Field_Not_Be_NULL”错误名与MySQL中违反非空约束的错误类型是“ERROR  1048  (23000)”对应。

```mysql
#使用MySQL_error_code
DECLARE Field_Not_Be_NULL CONDITION FOR 1048;

#使用sqlstate_value
DECLARE Field_Not_Be_NULL CONDITION FOR SQLSTATE '23000';
```

**范例2：**定义"ERROR 1148(42000)"错误，名称为command_not_allowed。

```mysql
#使用MySQL_error_code
DECLARE command_not_allowed CONDITION FOR 1148;

#使用sqlstate_value
DECLARE command_not_allowed CONDITION FOR SQLSTATE '42000';
```

### 2.3 定义处理程序

可以为SQL执行过程中发生的某种类型的错误定义特殊的处理程序。定义处理程序时，使用DECLARE语句  的语法如下：

```mysql
DECLARE 处理方式 HANDLER FOR 错误类型 处理语句;
```

- <font style="color:rgb(135,0,58)">**处理方式**</font>：处理方式有3个取值：CONTINUE、EXIT、UNDO。
  - <font style="color:rgb(237,105,99)">`CONTINUE`</font> ：表示遇到错误不处理，继续执行。
  - <font style="color:rgb(237,105,99)">`EXIT`</font> ：表示遇到错误马上退出。
  - <font style="color:rgb(237,105,99)">`UNDO`</font> ：表示遇到错误后撤回之前的操作。MySQL中暂时不支持这样的操作。
- <font style="color:rgb(135,0,58)">**错误类型：**</font>（即条件）可以有如下取值：
  - <font style="color:rgb(237,105,99)">`SQLSTATE '字符串错误码'`</font> ：表示长度为5的sqlstate_value类型的错误代码；
  - <font style="color:rgb(237,105,99)">`MySQL_error_code`</font> ：匹配数值类型错误代码；
  - <font style="color:rgb(237,105,99)">`错误名称`</font> ：表示DECLARE ... CONDITION定义的错误条件名称。
  - <font style="color:rgb(237,105,99)">`SQLWARNING`</font> ：匹配所有以01开头的SQLSTATE错误代码； 
  - <font style="color:rgb(237,105,99)">`NOT FOUND`</font> ：匹配所有以02开头的SQLSTATE错误代码；
  - <font style="color:rgb(237,105,99)">`SQLEXCEPTION`</font> ：匹配所有没有被SQLWARNING或NOT FOUND捕获的SQLSTATE错误代码；

<font style="color:rgb(135,0,58)">**处理语句：**</font>：如果出现上述条件之一，则采用对应的处理方式，并执行指定的处理语句。语句可以是  像“ SET 变量 = 值 ”这样的简单语句，也可以是使用 BEGIN ... END 编写的复合语句。

定义处理程序的几种方式，代码如下：

```mysql
#方法1：捕获sqlstate_value
DECLARE CONTINUE HANDLER FOR SQLSTATE '42S02' SET @info = 'NO_SUCH_TABLE';

#方法2：捕获mysql_error_value
DECLARE CONTINUE HANDLER FOR 1146 SET @info = 'NO_SUCH_TABLE';

#方法3：先定义条件，再调用
DECLARE no_such_table CONDITION FOR 1146;
DECLARE CONTINUE HANDLER FOR NO_SUCH_TABLE SET @info = 'NO_SUCH_TABLE';

#方法4：使用SQLWARNING
DECLARE EXIT HANDLER FOR SQLWARNING SET @info = 'ERROR';

#方法5：使用NOT FOUND
DECLARE EXIT HANDLER FOR NOT FOUND SET @info = 'NO_SUCH_TABLE';

#方法6：使用SQLEXCEPTION
DECLARE EXIT HANDLER FOR SQLEXCEPTION SET @info = 'ERROR';
```

### 2.4 案例解决

在存储过程中，定义处理程序，捕获sqlstate_value值，当遇到MySQL_error_code值为1048时，执行

CONTINUE操作，并且将@proc_value的值设置为-1。

```mysql
CREATE PROCEDURE UpdateDataNoCondition() 
BEGIN
	#定义处理程序
	DECLARE CONTINUE HANDLER FOR 1048 SET @proc_value = -1;

	SET @x = 1;
	UPDATE emps SET sal = NULL WHERE ename = 'WARD'; 
	SET @x = 2;
	UPDATE emps SET sal = 200 WHERE ename = 'WARD'; 
	SET @x = 3;
END ;
```

调用过程：

```mysql
mysql> CALL UpdateDataWithCondition(); Query OK, 0 rows affected (0.01 sec)

mysql> SELECT @x,@proc_value;
+------+-------------+
| @x	| @proc_value |
+------+-------------+
|	3 |	-1 |
+------+-------------+
1 row in set (0.00 sec)
```

**范例：**创建一个名称为“InsertDataWithCondition”的存储过程，代码如下。

在存储过程中，定义处理程序，捕获sqlstate_value值，当遇到sqlstate_value值为23000时，执行EXIT操   作，并且将@proc_value的值设置为-1。

```mysql
#准备工作
CREATE TABLE depts 
AS
SELECT * FROM test.dept;

ALTER TABLE depts
ADD CONSTRAINT uk_dept_name UNIQUE(deptno);
```

```mysql
CREATE PROCEDURE InsertDataWithCondition() 
BEGIN
	DECLARE duplicate_entry CONDITION FOR SQLSTATE '23000' ; 
	DECLARE EXIT HANDLER FOR duplicate_entry SET @proc_value = -1;

	SET @x = 1;
	INSERT INTO depts(dname) VALUES('测试'); 
	SET @x = 2;
	INSERT INTO depts(dname) VALUES('测试');
	SET @x = 3;
END ;
```

调用存储过程：

```mysql
mysql> CALL InsertDataWithCondition(); Query OK, 0 rows affected (0.01 sec)

mysql> SELECT @x,@proc_value;
+------+-------------+
| @x	| @proc_value |
+------+-------------+
|	2 |	-1 |
+------+-------------+
1 row in set (0.00 sec)
```

## 3. 流程控制

解决复杂问题不可能通过一个 SQL 语句完成，我们需要执行多个 SQL 操作。流程控制语句的作用就是控制存储过程中  SQL  语句的执行顺序，是我们完成复杂操作必不可少的一部分。只要是执行的程序，流程就分为三大类：

- 顺序结构 ：程序从上往下依次执行
- 分支结构 ：程序按条件进行选择执行，从两条或多条路径中选择一条执行
- 循环结构 ：程序满足一定条件下，重复执行一组语句

针对于MySQL 的流程控制语句主要有 3 类。注意：只能用于存储程序。

- 条件判断语句 ：IF 语句和 CASE 语句
- 循环语句 ：LOOP、WHILE 和 REPEAT 语句
- 跳转语句 ：ITERATE 和 LEAVE 语句

### 3.1 分支结构之 IF

IF 语句的语法结构是：

```mysql
IF 表达式1 
	THEN 操作1
[ELSEIF 表达式2 THEN 操作2] 
...
[ELSE 操作N]
END IF
```

根据表达式的结果为TRUE或FALSE执行相应的语句。这里“[]”中的内容是可选的。 

- 特点：① 不同的表达式对应不同的操作

​                   ② 使用在begin end中

- **范例1：**

```mysql
IF val IS NULL
THEN SELECT 'val is null';
ELSE SELECT 'val is not null';	
END IF
```

**范例2：**声明存储过程“update_sal_by_eno”，定义IN参数eno，输入员工编号。判断该员工 薪资如果低于2000元并且入职时间超过5年，就涨薪500元；否则就不变。

```mysql
CREATE PROCEDURE update_sal_by_eno1(IN eno INT) 
BEGIN
	DECLARE emp_sal DOUBLE; 
	DECLARE hire_year DOUBLE;

	SELECT sal INTO emp_sal FROM emps WHERE empno = eno;

	SELECT DATEDIFF(CURDATE(),hiredate)/365 INTO hire_year 
	FROM emps WHERE empno = eno;

	IF emp_sal < 2000 AND hire_year > 5
	THEN UPDATE emps SET sal = sal + 500 WHERE empno = eno; 
	END IF;
END ;
```

**范例3：**声明存储过程“update_sal_by_eno2”，定义IN参数eno，输入员工编号。判断该员工  薪资如果低于3000元，就更新薪资为3000元；薪资如果大于等于3000元且低于5000的，但是奖金 比例为NULL的，就更新奖金比例为0.01；其他的涨薪100元。

```mysql
DELIMITER //
CREATE PROCEDURE update_sal_by_eno2(IN eno INT) 
BEGIN
	DECLARE emp_sal DOUBLE; 
	DECLARE emp_comm DECIMAL(5,2);

	SELECT sal INTO emp_sal FROM emps WHERE empno = eno; 
	SELECT comm INTO emp_comm FROM emps WHERE empno = eno;

	IF emp_sal < 3000
		THEN UPDATE emps SET sal = 3000 WHERE empno = eno;
		SELECT '1';
	ELSEIF emp_sal < 5000 AND emp_comm IS NULL
		THEN UPDATE emps SET comm = 0.01*sal WHERE empno = eno;
		SELECT '2';
	ELSE
		UPDATE emps SET sal = sal + 100 WHERE empno = eno; 
		SELECT '3';
	END IF;
END //
DELIMITER //
```

### 3.2 **分支结构之** CASE

CASE 语句的语法结构1：

```mysql
#情况一：类似于switch 
CASE 表达式
WHEN 值1 THEN 结果1或语句1(如果是语句，需要加分号) 
WHEN 值2 THEN 结果2或语句2(如果是语句，需要加分号)
...
ELSE 结果n或语句n(如果是语句，需要加分号)
END [case]（如果是放在begin end中需要加上case，如果放在select后面不需要）
```

CASE 语句的语法结构2：

```mysql
#情况二：类似于多重if 
CASE
WHEN 条件1 THEN 结果1或语句1(如果是语句，需要加分号)
WHEN 条件2 THEN 结果2或语句2(如果是语句，需要加分号)
...
ELSE 结果n或语句n(如果是语句，需要加分号)
END [case]（如果是放在begin end中需要加上case，如果放在select后面不需要）
```

**范例1：**使用CASE流程控制语句的第1种格式，判断val值等于1、等于2，或者两者都不等。

```mysql
CASE val
	WHEN 1 THEN SELECT 'val is 1'; 
	WHEN 2 THEN SELECT 'val is 2';
	ELSE SELECT 'val is not 1 or 2'; 
END CASE;
```

**范例2：**使用CASE流程控制语句的第2种格式，判断val是否为空、小于0、大于0或者等于0。

```mysql
CASE
	WHEN val IS NULL THEN SELECT 'val is null'; 
	WHEN val < 0 THEN SELECT 'val is less than 0';
	WHEN val > 0 THEN SELECT 'val is greater than 0'; 
	ELSE SELECT 'val is 0';
END CASE;
```

**范例3：**声明存储过程“update_salary_by_eno4”，定义IN参数eno，输入员工编号。判断改员工所属部门，如果在10号部门，工资加500；如果在20号部门，工资加600；如果在30号部门，工资加700;其他部门加300。

```mysql
CREATE PROCEDURE update_salary_by_eno4(IN eno INT) 
BEGIN
	DECLARE dept_no,emp_sal INT; 

	SELECT sal,deptno INTO emp_sal,dept_no FROM emps WHERE empno = eno; 

	CASE deptno
	WHEN 10
		THEN UPDATE emps SET sal=emp_sal+500 WHERE empno = eno; 
	WHEN 20
		THEN UPDATE emps SET sal=emp_sal+600 WHERE empno = eno; 
	WHEN 30
		THEN UPDATE emps SET sal=emp_sal+700 WHERE empno = eno;
	ELSE 
		UPDATE emps SET sal=emp_sal+300 WHERE empno = eno;	
	END CASE;
END //
```

**范例4：**声明存储过程pro_sal_grade，定义IN参数eno，输入员工编号。判断该员工的薪资等级，如果在[700,1200]之间，为等级一；如果在[1201，1400]之间，为等级二；如果在[1401,2000]之间，为等级三；如果在[2001,3000]之间，为等级四；如果在[3001,9999]之间，为等级五；

```mysql
CREATE PROCEDURE sal_grade(IN eno INT) 
BEGIN
	DECLARE emp_sal INT; 

	SELECT sal INTO emp_sal FROM emps WHERE empno = eno;

	CASE 
		WHEN emp_sal>=700 AND emp_sal<=1200
			THEN SELECT '等级一'; 			
		WHEN emp_sal>=1201 AND emp_sal<=1400 
			THEN SELECT '等级二';		
		WHEN emp_sal>=1401 AND emp_sal<=2000 	
			THEN SELECT '等级三';
		WHEN emp_sal>=2001 AND emp_sal<=3000 
			THEN SELECT '等级四';
		WHEN emp_sal>=3001 AND emp_sal<=999 
			THEN SELECT '等级五';
	END CASE;
END //
```

### 3.3 循环结构之 LOOP

LOOP循环语句用来重复执行某些语句。LOOP内的语句一直重复执行直到循环被退出（使用LEAVE子  句），跳出循环过程。

LOOP语句的基本格式如下：

```mysql
[loop_label:] LOOP
		循环执行的语句
END LOOP [loop_label] 
```

其中，loop_label表示LOOP语句的标注名称，该参数可以省略。

**范例1：**使用LOOP语句进行循环操作，id值小于10时将重复执行循环过程。

```mysql
DECLARE id INT DEFAULT 0;
add_loop:LOOP
	SET id = id +1;
	IF id >= 10 
	THEN LEAVE add_loop; 
	END IF;

END LOOP add_loop;
```

**范例2：**当市场环境变好时，公司为了奖励大家，决定给大家涨工资。声明存储过程“update_sal_loop()”，声明OUT参数num，输出循环次数。存储过程中实现循环给大家涨薪，薪资涨为原来的1.1倍。直到全公司的平均薪资达到8000结束。并统计循环次数。

```mysql
CREATE PROCEDURE update_sal_loop(OUT num INT) 
BEGIN
	DECLARE avg_sal DOUBLE; 
	DECLARE loop_count INT DEFAULT 0;

	SELECT AVG(sal) INTO avg_sal FROM emps;

	label_loop:LOOP
		IF avg_sal >= 8000 
		THEN LEAVE label_loop; 
		END IF;

		UPDATE emps SET sal = sal * 1.1; 
		SET loop_count = loop_count + 1;
		SELECT AVG(sal) INTO avg_salary FROM emps; 
	END LOOP label_loop;

SET num = loop_count; 
END ;
```

### 3.4 循环结构之 WHILE

WHILE语句创建一个带条件判断的循环过程。WHILE在执行语句执行时，先对指定的表达式进行判断，如  果为真，就执行循环内的语句，否则退出循环。WHILE语句的基本格式如下：

```mysql
[while_label:] WHILE 循环条件 DO
	循环体
END WHILE [while_label];
```

while_label为WHILE语句的标注名称；如果循环条件结果为真，WHILE语句内的语句或语句群被执行，直   至循环条件为假，退出循环。

**范例1：**WHILE语句示例，i值小于10时，将重复执行循环过程，代码如下：

```mysql
CREATE PROCEDURE test_while()
BEGIN
	DECLARE i INT DEFAULT 0;

	WHILE i < 10 DO 
		SET i = i + 1;
	END WHILE;

	SELECT i;
END ;
#调用
CALL test_while();
```

**范例2：**市场环境不好时，公司为了渡过难关，决定暂时降低大家的薪资。声明存储过程“update_salary_while()”，声明OUT参数num，输出循环次数。存储过程中实现循环给大家降薪，薪资降为原来的90%。直到全公司的平均薪资达到3000结束。并统计循环次数。

```mysql
CREATE PROCEDURE update_sal_while(OUT num INT) 
BEGIN
	DECLARE avg_sal DOUBLE ;
	DECLARE while_count INT DEFAULT 0;
	SELECT AVG(sal) INTO avg_sal FROM emps; 
	WHILE avg_sal > 3000 DO
		UPDATE emps SET sal = sal * 0.9; 
		SET while_count = while_count + 1;
		SELECT AVG(sal) INTO avg_sal FROM emps; 
	END WHILE;

	SET num = while_count; 
END //
```

### 3.5 循环结构之 REPEAT

REPEAT语句创建一个带条件判断的循环过程。与WHILE循环不同的是，REPEAT 循环首先会执行一次循环，然后在 UNTIL 中进行表达式的判断，如果满足条件就退出，即 END REPEAT；如果条件不满足，则会就继续执行循环，直到满足退出条件为止。

REPEAT语句的基本格式如下：

```mysql
[repeat_label:] REPEAT
		循环体的语句
UNTIL 结束循环的条件表达式
END REPEAT [repeat_label]
```

repeat_label为REPEAT语句的标注名称，该参数可以省略；REPEAT语句内的语句或语句群被重复，直至expr_condition为真。

**范例1：**

```mysql
CREATE PROCEDURE test_repeat() 
BEGIN
	DECLARE i INT DEFAULT 0;

	REPEAT
		SET i = i + 1; 
	UNTIL i >= 10
	END REPEAT;

	SELECT i;
END ;
```

**范例2：**当市场环境变好时，公司为了奖励大家，决定给大家涨工资。声明存储过程“update_salary_repeat()”，声明OUT参数num，输出循环次数。存储过程中实现循环给大家涨薪，薪资涨为原来的1.15倍。直到全公司的平均薪资达到9000结束。并统计循环次数。

```mysql
CREATE PROCEDURE update_salary_repeat(OUT num INT) 
BEGIN
	DECLARE avg_sal DOUBLE ;
	DECLARE repeat_count INT DEFAULT 0;
	
	SELECT AVG(sal) INTO avg_sal FROM emps; 
	
	REPEAT
		UPDATE emps SET sal = sal * 1.15;
        SET repeat_count = repeat_count + 1;
		SELECT AVG(sal) INTO avg_sal FROM emps; 
	UNTIL avg_sal >= 9000
	END REPEAT;

	SET num = repeat_count; 
END //
```

**对比三种循环结构：**

1、这三种循环都可以省略名称，但如果循环中添加了循环控制语句（LEAVE或ITERATE）则必须添加名称。

2、 LOOP：一般用于实现简单的"死"循环 WHILE：先判断后执行 REPEAT：先执行后判断，无条件至少执行一次

### 3.6 跳转语句之 LEAVE语句

LEAVE语句：可以用在循环语句内，或者以 BEGIN 和 END 包裹起来的程序体内，表示跳出循环或者跳出程序体的操作。如果你有面向过程的编程语言的使用经验，你可以把 LEAVE 理解为 break。

基本格式如下：

```mysql
LEAVE 标记名
```

其中，label参数表示循环的标志。LEAVE和BEGIN ... END或循环一起被使用。

**范例1：**创建存储过程 “leave_begin()”，声明INT类型的IN参数num。给BEGIN...END加标记名，并在BEGIN...END中使用IF语句判断num参数的值。

- 如果num<=0，则使用LEAVE语句退出BEGIN...END； 
- 如果num=1，则查询“emps”表的平均薪资； 
- 如果num=2，则查询“emps”表的最低薪资； 
- 如果num>2，则查询“emps”表的最高薪资。

IF语句结束后查询“emp”表的总人数。

```mysql
CREATE PROCEDURE leave_begin(IN num INT) 
	begin_label: BEGIN
		IF num<=0
			THEN LEAVE begin_label; 
		ELSEIF num=1
			THEN SELECT AVG(sal) FROM emps; 
		ELSEIF num=2
			THEN SELECT MIN(sal) FROM emps; 
		ELSE
			SELECT MAX(sal) FROM emps; 
		END IF;

		SELECT COUNT(*) FROM emps; 
END ;
```

**范例2：**当市场环境不好时，公司为了渡过难关，决定暂时降低大家的薪资。声明存储过程“leave_while()”，声明OUT参数num，输出循环次数，存储过程中使用WHILE循环给大家降低薪资为原来薪资的90%，直到全公  司的平均薪资小于等于10000，并统计循环次数。

```mysql
CREATE PROCEDURE leave_while(OUT num INT)
BEGIN
	#
	DECLARE avg_sal DOUBLE;#记录平均工资
	DECLARE while_count INT DEFAULT 0; #记录循环次数

	SELECT AVG(sal) INTO avg_sal FROM emps; #① 初始化条件

	while_label:WHILE TRUE DO #② 循环条件

		#③ 循环体
		IF avg_sal <= 10000 
			THEN LEAVE while_label;
		END IF;

		UPDATE emps SET sal = sal * 0.9; 
		SET while_count = while_count + 1;

		#④ 迭代条件
		SELECT AVG(sal) INTO avg_sal FROM emps; 
	END WHILE;
	#赋值
	SET num = while_count; 
END ;
```



### 3.7 跳转语句之 ITERATE 语句

ITERATE语句：只能用在循环语句（LOOP、REPEAT和WHILE语句）内，表示重新开始循环，将执行顺序   转到语句段开头处。如果你有面向过程的编程语言的使用经验，你可以把 ITERATE 理解为  continue，意思为“再次循环”。

语句基本格式如下：

```mysql
ITERATE label
```

label参数表示循环的标志。ITERATE语句必须跟在循环标志前面。

**范例：** 定义局部变量num，初始值为0。循环结构中执行num + 1操作。

- 如果num < 10，则继续执行循环；
- 如果num > 15，则退出循环结构；

```mysql
CREATE PROCEDURE test_iterate() 
BEGIN
	DECLARE num INT DEFAULT 0;

	my_loop:LOOP
		SET num = num + 1;

		IF num < 10
			THEN ITERATE my_loop; 
		ELSEIF num > 15
			THEN LEAVE my_loop; 
		END IF;

		SELECT '顿开教育：让每个学员都学有所成'; 
	END LOOP my_loop;
END //
```



## 4. 游标

### 4.1 什么是游标（或光标）

虽然我们也可以通过筛选条件 WHERE 和 HAVING，或者是限定返回记录的关键字 LIMIT 返回一条记录， 但是，却无法在结果集中像指针一样，向前定位一条记录、向后定位一条记录，或者是`随意定位到某一条记录` ，并对记录的数据进行处理。

这个时候，就可以用到游标。游标，提供了一种灵活的操作方式，让我们能够对结果集中的每一条记录  进行定位，并对指向的记录中的数据进行操作的数据结构。**游标让** **SQL** **这种面向集合的语言有了面向过程开发的能力。**

在 SQL 中，游标是一种临时的数据库对象，可以指向存储在数据库表中的数据行指针。这里游标充当了指针的作用 ，我们可以通过操作游标来对数据行进行操作。

MySQL中游标可以在存储过程和函数中使用。

比如，我们查询了 emps数据表中工资高于1500的员工都有哪些：

```mysql
SELECT empno,ename,sal FROM emps WHERE sal > 1500;
```

![image-20220428165219836](E:/Typora/MySQL/14%20%E5%8F%98%E9%87%8F%E3%80%81%E6%B5%81%E7%A8%8B%E6%8E%A7%E5%88%B6%E4%B8%8E%E6%B8%B8%E6%A0%87/assets/image-20220428165219836.png)

这里我们就可以通过游标来操作数据行，如图所示此时游标所在的行是“7698”的记录，我们也可以在结果集上滚动游标，指向结果集中的任意一行。

### 4.2 使用游标步骤

游标必须在声明处理程序之前被声明，并且变量和条件还必须在声明游标或处理程序之前被声明。   如果我们想要使用游标，一般需要经历四个步骤。不同的 DBMS 中，使用游标的语法可能略有不同。

<font style="color:rgb(135,0,58)">**第一步，声明游标**</font>

在MySQL中，使用DECLARE关键字来声明游标，其语法的基本形式如下：

```mysql
DECLARE cursor_name CURSOR FOR select_statement;
```

这个语法适用于 MySQL，SQL Server，DB2 和 MariaDB。如果是用 Oracle 或者 PostgreSQL，需要写成：

```mysql
DECLARE cursor_name CURSOR IS select_statement;
```

 要使用 SELECT 语句来获取数据结果集，而此时还没有开始遍历数据，这里 select_statement 代表的是

SELECT 语句，返回一个用于创建游标的结果集。

比如：

```mysql
DECLARE cur_emp CURSOR FOR
SELECT empno,sal FROM emps;
```

```mysql
DECLARE cursor_dept CURSOR FOR 
SELECT deptno,dname, loc FROM depts ;
```

<font style="color:rgb(135,0,58)">**第二步，打开游标**</font>

打开游标的语法如下：

```mysql
OPEN cursor_name
```

当我们定义好游标之后，如果想要使用游标，必须先打开游标。打开游标的时候 SELECT  语句的查询结果集就会送到游标工作区，为后面游标的 逐条读取 结果集中的记录做准备。

```mysql
OPEN cur_emp ;
```

 <font style="color:rgb(135,0,58)">**第三步，使用游标（从游标中取得数据）**</font>

语法如下：

```mysql
FETCH cursor_name INTO var_name [, var_name] ...
```

这句的作用是使用 cursor_name 这个游标来读取当前行，并且将数据保存到 var_name 这个变量中，游标指针指到下一行。如果游标读取的数据行有多个列名，则在 INTO 关键字后面赋值给多个变量名即可。

注意：var_name必须在声明游标之前就定义好。

```mysql
FETCH cur_emp INTO emp_no, emp_sal ;
```

注意：**游标的查询结果集中的字段数，必须跟** **INTO**  **后面的变量数一致**，否则，在存储过程执行的时候，MySQL 会提示错误。

 <font style="color:rgb(135,0,58)">**第四步，关闭游标**</font>

```mysql
CLOSE cursor_name
```

有 OPEN 就会有 CLOSE，也就是打开和关闭游标。当我们使用完游标后需要关闭掉该游标。因为游标会占用系统资源 ，如果不及时关闭，**游标会一直保持到存储过程结束**，影响系统运行的效率。而关闭游标的操作，会释放游标占用的系统资源。

关闭游标之后，我们就不能再检索查询结果中的数据行，如果需要检索只能再次打开游标。

```mysql
CLOSE cur_emp; 
```

### 4.3 举例

创建存储过程“get_count_by_limit_total_salary()”，声明IN参数 limit_total_salary；声明OUT参数total_count。函数的功能可以实现累加薪资最高的几个员工的薪资值，直到薪资总和 达到limit_total_salary参数的值，返回累加的人数给total_count。

```mysql
CREATE PROCEDURE get_count_by_limit_total_salary(IN limit_total_sal INT,OUT total_count INT)
BEGIN
	#声明变量
	DECLARE sum_sal INT DEFAULT  0;		#记录累加的总工资
	DECLARE emp_count INT DEFAULT  0;	#记录循环总次数
	DECLARE emp_sal INT;							#当前员工的工资
	
	#声明游标
	DECLARE  cursor_emp CURSOR FOR SELECT sal FROM emp ORDER BY sal DESC;
	#打开游标
	OPEN cursor_emp;
	#使用游标
	WHILE sum_sal < limit_total_sal DO
			FETCH cursor_emp INTO emp_sal;
			SET sum_sal = emp_sal+sum_sal;
			SET emp_count = emp_count+1;
	END WHILE;
	
	#关闭游标
	CLOSE cursor_emp;
	#设置传出参数值
	SET total_count = emp_count;
END;
```

### 4.4 小结

游标是 MySQL 的一个重要的功能，为 逐条读取 结果集中的数据，提供了完美的解决方案。跟在应用层面实现相同的功能相比，游标可以在存储程序中使用，效率高，程序也更加简洁。

但同时也会带来一些性能问题，比如在使用游标的过程中，会对数据行进行加锁 ，这样在业务并发量大的时候，不仅会影响业务之间的效率，还会  消耗系统资源 ，造成内存不足，这是因为游标是在内存中进行的处理。

建议：养成用完之后就关闭的习惯，这样才能提高系统的整体效率。

# 第15章 触发器

在实际开发中，我们经常会遇到这样的情况：有 2 个或者多个相互关联的表，如商品信息和库存信息分别存放在 2 个不同的数据表中，我们在添加一条新商品记录的时候，为了保证数据的完整性，必须同时在库存表中添加一条库存记录。

这样一来，我们就必须把这两个关联的操作步骤写到程序里面，而且要用事务包裹起来，确保这两个操作成为一个原子操作 ，要么全部执行，要么全部不执行。要是遇到特殊情况，可能还需要对数据进行手动维护，这样就很容易忘记其中的一步 ，导致数据缺失。

这个时候，咱们可以使用触发器。**你可以创建一个触发器，让商品信息数据的插入操作自动触发库存数据的插入操作。**这样一来，就不用担心因为忘记添加库存数据而导致的数据缺失了。

 

## **1.** 触发器概述

MySQL从 5.0.2 版本开始支持触发器。MySQL的触发器和存储过程一样，都是嵌入到MySQL服务器的一段程序。

触发器是由 事件来触发 某个操作，这些事件包括INSERT、UPDATE、DELETE 事件。所谓事件就是指用户的动作或者触发某项行为。如果定义了触发程序，当数据库执行这些语句时候，就相当于事件发生了，就会自动激发触发器执行相应的操作。

当对数据表中的数据执行插入、更新和删除操作，需要自动执行一些数据库逻辑时，可以使用触发器来实现。

 

## **2.** 触发器的创建

### 2.1 创建触发器语法

创建触发器的语法结构是：

```mysql
CREATE TRIGGER 触发器名称
{BEFORE|AFTER} {INSERT|UPDATE|DELETE} ON 表名
FOR EACH ROW
触发器执行的语句块;
```

说明：

- <font style="color:rgb(232,105,0)">表名</font> ：表示触发器监控的对象。
- <font style="color:rgb(232,105,0)">BEFORE|AFTER</font>：表示触发的时间。BEFORE 表示在事件之前触发；AFTER 表示在事件之后触发。
- <font style="color:rgb(232,105,0)">INSERT|UPDATE|DELETE </font>：表示触发的事件。
  - INSERT 表示插入记录时触发； 
  - UPDATE 表示更新记录时触发； 
  - DELETE 表示删除记录时触发。
- <font style="color:rgb(232,105,0)">触发器执行的语句块 </font>：可以是单条SQL语句，也可以是由BEGIN…END结构组成的复合语句块。

**范例1：**

1、创建数据表

```mysql
CREATE TABLE users(number INT, money DECIMAL(10,2));
```

2、创建触发器

```mysql
CREATE TRIGGER t_insert BEFORE INSERT ON users
FOR EACH ROW SET @sum = @sum+1;
```

3、使用触发器

在使用触发器之前，先将累加器变量设置为零，执行一条INSERT语句，然后查看该变量的值。

```mysql
SET @sum = 0;
INSERT INTO users VALUES(1,35000);
SELECT @sum;
```

### 2.2 OLD 和 NEW

在触发器正文中，OLD 和 NEW 关键字使您能够访问受触发器影响的行中的列。 OLD 和 NEW 是 MySQL 对触发器的扩展；它们不区分大小写。

在 INSERT 触发器中，只能使用 NEW.col_name；没有旧行。在 DELETE 触发器中，只能使用 OLD.col_name；没有新行。在 UPDATE 触发器中，您可以使用 OLD.col_name 来引用行更新之前的列，使用 NEW.col_name 来引用行更新后的列。

以 OLD 命名的列是只读的。您可以参考它（如果您有 SELECT 权限），但不能修改它。如果您具有 SELECT 权限，则可以引用以 NEW 命名的列。在 BEFORE 触发器中，如果您具有 UPDATE 权限，您还可以使用 SET NEW.col_name = value 更改其值。这意味着您可以使用触发器来修改要插入新行或用于更新行的值。 （这样的 SET 语句在 AFTER 触发器中无效，因为行更改已经发生。）

在 BEFORE 触发器中，AUTO_INCREMENT 列的 NEW 值为 0，而不是实际插入新行时自动生成的序列号。

**范例1：**

1、插入触发器

```mysql
-- 插入只有NEW，没有OLD
CREATE TRIGGER t_insert_val BEFORE INSERT ON users
FOR EACH ROW 
BEGIN
	#在插入之前可以通过new访问字段，并修改值(在ALTER触发器中无效)
	SET NEW.money = NEW.money - 1000;
END; 

INSERT INTO users VALUES(1,2000)
```

2、删除触发器

```mysql
-- 创建一个保存已经删除的数据的表
CREATE TABLE users_del(number INT, money DECIMAL(10,2));

-- 删除只有OLD，没有NEW
CREATE TRIGGER t_delete_val BEFORE DELETE ON users
FOR EACH ROW
BEGIN
	#把删除的记录插入到users_del表
	INSERT INTO users_del(number,money) VALUES(OLD.number,OLD.money) ;
END;

DELETE FROM users WHERE number = 5;

SELECT * FROM users;
SELECT * FROM users_del;

DELETE FROM users;
DELETE FROM users_del;
```

3、更新触发器

```mysql
-- 更新有OLD，也有NEW（更新之前）
CREATE TRIGGER t_update_val BEFORE UPDATE ON users
FOR EACH ROW
BEGIN
	#把旧数据和新数据都插入users_del表
	INSERT INTO users_del(number,money) VALUES(OLD.number,OLD.money);
	INSERT INTO users_del(number,money) VALUES(NEW.number,NEW.money);
END;

UPDATE users SET money = 100 WHERE number = 6;
```



**举例2**：定义触发器“t_sal_check”，基于员工表“emps”的INSERT事件，在INSERT之前检查    将要添加的新员工薪资是否大于他领导的薪资，如果大于领导薪资，则报sqlstate_value为'HY000'的错误，从而使得添加失败。

```mysql
CREATE TRIGGER t_sal_check BEFORE INSERT ON emps
FOR EACH ROW
BEGIN
	DECLARE mgr_sal INT;
	#获取插入员工的领导的薪资
	SELECT sal INTO mgr_sal FROM emps WHERE empno = NEW.mgr;
	
	IF NEW.sal > mgr_sal THEN
		SIGNAL SQLSTATE 'HY000' SET MESSAGE_TEXT = '薪资高于领导薪资-禁止插入';
	END IF;
	
END;

INSERT INTO emps(empno,job,mgr,sal) VALUES(1,'打工人',7566,3400);
INSERT INTO emps(empno,job,mgr,sal) VALUES(2,'捡垃圾',7566,3300);
```

上面触发器声明过程中的NEW关键字代表INSERT添加语句的新记录。

 

## 3. 查看、删除触发器

### 3.1 查看触发器

查看触发器是查看数据库中已经存在的触发器的定义、状态和语法信息等。

方式1：查看当前数据库的所有触发器的定义

```mysql
SHOW TRIGGERS;
```

方式2：查看当前数据库中某个触发器的定义

```mysql
SHOW CREATE TRIGGER 触发器名
```

方式3：从系统库information_schema的TRIGGERS表中查询指定数据库的触发器的信息。

```mysql
SELECT * FROM information_schema.TRIGGERS WHERE trigger_schema='db_14';
```



### 3.2 删除触发器

- 触发器也是数据库对象，删除触发器也用DROP语句：
- 如果您删除一个表，该表的所有触发器也会被删除。

```mysql
DROP TRIGGER IF EXISTS 触发器名称;
```

 

## 4. 触发器的优缺点

### 4.1 优点

#### 1、触发器可以确保数据的完整性。

假设我们用 进货单头表 （demo.importhead）来保存进货单的总体信息，包括进货单编号、供货商编号、仓库编号、总计进货数量、总计进货金额和验收日期。

 ![img](E:/Typora/MySQL/15%20%E8%A7%A6%E5%8F%91%E5%99%A8/assets/wps42.png)

用 进货单明细表 （demo.importdetails）来保存进货商品的明细，包括进货单编号、商品编号、进货数量、进货价格和进货金额。



  ![img](E:/Typora/MySQL/15%20%E8%A7%A6%E5%8F%91%E5%99%A8/assets/wps43.png)

每当我们录入、删除和修改一条进货单明细数据的时候，进货单明细表里的数据就会发生变动。这个时候，在进货单头表中的总计数量和总计金额就必须重新计算，否则，进货单头表中的总计数量和总计金额就不等于进货单明细表中数量合计和金额合计了，这就是数据不一致。

为了解决这个问题，我们就可以使用触发器，**规定每当进货单明细表有数据插入、修改和删除的操作**  **时，自动触发** **2** **步操作：**

1） 重新计算进货单明细表中的数量合计和金额合计；

2） 用第一步中计算出来的值更新进货单头表中的合计数量与合计金额。

这样一来，进货单头表中的合计数量与合计金额的值，就始终与进货单明细表中计算出来的合计数量与  合计金额的值相同，数据就是一致的，不会互相矛盾。

#### 2、触发器可以帮助我们记录操作日志

利用触发器，可以具体记录什么时间发生了什么。比如，记录修改会员储值金额的触发器，就是一个很  好的例子。这对我们还原操作执行时的具体场景，更好地定位问题原因很有帮助。

#### 3、触发器还可以用在操作数据前，对数据进行合法性检查

比如，超市进货的时候，需要库管录入进货价格。但是，人为操作很容易犯错误，比如说在录入数量的时候，把条形码录进去了；录入金额的时候，看串了行，录入的价格远超售价，导致账面上的巨亏……   这些都可以通过触发器，在实际插入或者更新操作之前，对相应的数据进行检查，及时提示错误，防止错误数据进入系统。

### 4.2 缺点

#### 1、触发器最大的一个问题就是可读性差

因为触发器存储在数据库中，并且由事件驱动，这就意味着触发器有可能 不受应用层的控制 。这对系统维护是非常有挑战的。

比如，创建触发器用于修改会员储值操作。如果触发器中的操作出了问题，会导致会员储值金额更新失  败。我用下面的代码演示一下：

```mysql
mysql> update demo.membermaster set memberdeposit=20 where memberid = 2;
ERROR 1054 (42S22): Unknown column 'aa' in 'field list'
```

结果显示，系统提示错误，字段“aa”不存在。

这是因为，触发器中的数据插入操作多了一个字段，系统提示错误。可是，如果你不了解这个触发器，  很可能会认为是更新语句本身的问题，或者是会员信息表的结构出了问题。说不定你还会给会员信息表  添加一个叫“aa”的字段，试图解决这个问题，结果只能是白费力。

#### 2、相关数据的变更，可能会导致触发器出错。

特别是数据表结构的变更，都可能会导致触发器出错，进而影响数据操作的正常运行。这些都会由于触  发器本身的隐蔽性，影响到应用中错误原因排查的效率。

### 4.3 注意点

注意，如果在子表中定义了外键约束，并且外键指定了ON UPDATE/DELETE CASCADE/SET NULL子句，此时修改父表被引用的键值或删除父表被引用的记录行时，也会引起子表的修改和删除操作，此时基于子  表的UPDATE和DELETE语句定义的触发器并不会被激活。

例如：基于员工表（emp）的DELETE语句定义了触发器t_del_emps，而emp表的部门编号（deptno）字段定 义了外键约束引用了部门表（dept）的主键列部门编号（deptno），并且该外键加了“ON DELETE SET NULL”子句，那么如果此时删除父表（部门表dept）在子表员工表（emp） 有匹配记录的部门记录时，会引起子表员工表（emp）匹配记录的部门编号（deptno）修改为NULL，

但是此时不会激活触发器t_del_emps。只有直接对子表员工表（emps）执行DELETE语句时才会激活触发器t_del_emps。