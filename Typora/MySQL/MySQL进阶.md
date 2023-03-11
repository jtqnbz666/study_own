## 一、约束条件

约束条件        默认			 非空   		唯一
  关键字		default		not  null      unique  key

​		自增长					主键					外键
auto_increment		primary  key		foreign  key

1.非空约束(not	null)

~~~mysql
#not  null限制一个字段它的值能不能为空,insert的时候必须赋值
create table if not exists tb(id int not null,name varchar(20));
insert into table values(1,"小江");
insert into table(name) values("小江")#这样是错误的。因为有非空约束not null
alter table tb modify id int not null default 0;#这样就可以


~~~



2.唯一约束(unique	key)

~~~mysql
#唯一约束保证了字段值不能重复
alter table tb modify id int not null unique key;
~~~



3.主键约束(primary key)

~~~mysql
#主键保证表格记录的唯一性
#非空(not null)  还要有  唯一约束(unique  key)。
#primary key= not null +unique key;

#要注意的问题
#1：如果表中没有数据可以直接
alter table tb modify id int primary key；
#2：若有数据就要根据实际情况在后边追加约束。
 alter table tb modify id int primary key auto_increment;
~~~

4.自增长约束(auto_increment)   

~~~mysql
#自动编号，必须和主键组合使用，一个表里面只能有一个自增长。

#意思也就是auto_increment只能用在主键上。(下边展示两个错误语句)
create table tb(id int auto_increment ,name varchar(20));#因为无主键
alter table tb modify id int auto_increment;#若先前未定义主键也是错的。
create table tb(id int primary key auto_increment ,name varchar(20));#正确方法
#默认情况下，起始值为1，每次增量为1。

#使用全字段插入是要注意的问题：
insert into tb values("小江");# 这样是错误的。
#正确方法(default , null)
insert into tb values(default,"小桃");
insert into tb values(null,"老铁");
#插入记录时，从最大值的id往后插。
#比如现在最大id是10，使用delete 删除掉id=10,在此插入时，默认是从11开始了
~~~

5：默认约束(default)

~~~mysql
#设置一个初始值，插入数据时，如果不赋值，就使用默认值
#举例：
insert into tb values(18,default,default);#全字段插入
#默认值可能是一些特定数字，但如如果这里的default如果换成null，呈现在表格上的就是null
~~~



6.外键约束()



~~~mysql
#学生和学生详情之间的关系：a表放置学生普通信息，b表放置铭感信息
create table b(id int ,name varchar(20),a_id int ,foreign key(a_id) references a(id));
alter table 表名 add constraint `别名` foreign key(自身字段名) references 外表(外表字段名)
alter table 表名 add constraint `c_s` foreign key(col_id) references college(id) on delete set null;
#这里补充一下有三种 on delete方式
#1.on delete set null//父子都删，置为空就行
#2.on delete cascade//父子得相关联数据都删了
#3.on delete no_action或者on delete restrict//若没有清除关联前不允许删除。


#删除外键约束（如下图所示）
#使用show create table 表名
#使用语句  alter table 表名 drop foreign key 红色框里的内容;

~~~
![](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\1650868709660.png)
![1639739751389](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\1639739751389.png)

## 二、表关系

1.一对多

创建学院表：

```mysql
 create table college(
    -> id int primary key auto_increment,
    -> name varchar(20) not null);
    
 insert into college values
 	-> (1,"计算机学院"),
 	-> (2,"旅游管理学院"),
 	-> (3,"体育学院"),
 	-> (4,"商学院");
```



学院--学生

~~~mysql
create table student(
	-> id int primary key auto_increment,
    -> name varchar(20) not null,
    -> col_id int,
    #这里是一对多,所以col_id 不需要唯一约束。
    -> constraint `c_s` foreign key(col_id) references college(id)#这里注意外键是加在学院身上的
    -> on delete set null);
~~~



2.一对一

单个学生--学生个人信息

~~~mysql
 create table student_detail(
    -> id int primary key auto_increment,
    -> phone varchar(20),
    -> stu_id int unique key,
    # 这里是一对一,所以stu_id需要唯一约束。
    -> constraint `sd_s` foreign key(stu_id) references student(id)#这里注意外键是加在学生身上的
    -> on delete cascade);
~~~



3.多对多（需要维护一个中间表）

学生---课程

课程表：

~~~mysql
 create table course(
    -> id int primary key auto_increment,
    -> name varchar(20) not null);
    
 insert into course(name) values
    -> ("C++"),
    -> ("java"),
    -> ("python"),
    -> ("C#");
~~~

中间表:(维系学生和课程间的关系)

~~~mysql
 create table sel(
    -> stu_id int,
    -> cours_id int,
    -> primary key(stu_id,cours_id),#注意这里设置了两个主键.
     #称做联合主键。
    -> constraint `s_s` foreign key(stu_id) references student(id),#第一个外键
    -> constraint `s_c` foreign key(cours_id) references course(id));#第二个外键
~~~

![](C:\Users\ASUS\Pictures\博客图片\QQ图片20211217205338.png)

## 三、范式

1.不可多表头

![](C:\Users\ASUS\Pictures\博客图片\QQ图片20211217205826.png)

2.在1的基础上，依赖分离，形成表关系



![](C:\Users\ASUS\Pictures\博客图片\QQ图片20211217205842.png)







3.在2的基础上实现非主属性在整个数据库中只出现一次。

![](C:\Users\ASUS\Pictures\博客图片\QQ图片20211217210141.png)