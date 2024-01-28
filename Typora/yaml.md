-的作用:表示一个字段的多个取值

~~~
containers:
  - name: nginx
    image: nginx:latest
  - name: mysql
    image: mysql:latest
~~~



**锚点和引用**

~~~yaml
# 第一种：
prod:
 driverClassName: &alis com.mysql.Dirver
dev:
 driverClassName: *alis
# 效果
"prod": {
  "driverClassName": "com.mysql.Dirver"
},
"dev": {
  "driverClassName": "com.mysql.Dirver"
}

# 第二种:
prod: &alis
 driverClassName:  com.mysql.Dirver
dev:
 driverClassName: *alis
# 效果:
"prod": {
    "driverClassName": "com.mysql.Dirver"
  },
  "dev": {
    "driverClassName": {
      "driverClassName": "com.mysql.Dirver"
    }
  }
 
# 第三种，配合 <<
prod: &alis
 driverClassName:  com.mysql.Dirver

dev:
 <<: *alis
# 效果
"prod": {
  "driverClassName": "com.mysql.Dirver"
},
"dev": {
  "driverClassName": "com.mysql.Dirver"
}
~~~





|- 和 | 区别

~~~yaml
message: 
  This is a
  multiline
  string.
# 输出：
"This is a multiline string."

message: |- 
    This is a
    multiline
    string.
# 输出
"This is a\nmultiline\nstring."

message: |
  This is a
  multiline
  string.
# 输出
"This is a\nmultiline\nstring.\n"
~~~

json

~~~json
{
"id":1,
"name":"jt",
"job":{
	"name": "程序"
}
}
~~~

yaml(更精简)

~~~yaml
# 字段后边必须要有空格
name: jt
age: 20
job:
	 程序
~~~

yaml数组

~~~yaml
# 方法一
job: [c++, go开发]
# 方法二
job:
- c++
- go开发
~~~

yaml基本语法

~~~
1.大小写敏感
2.使用缩紧表示层级关系
3.缩进不允许使用tab， 只允许空格
4.缩进的空格数不足，只要相同层级的元素左对齐即可
5.字段后边必须要有空格
~~~

学习工具：yaml在线编辑