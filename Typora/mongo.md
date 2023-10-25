有时候mongo只能用127.0.0.1

启动不了可能是因为本地有多个mongo服务，得删掉多余的

启动mongo：brew services start mongodb-community@6.0

mongo和mysql区别：mysql是规范的，结构化很清楚， mysql中可以多表联合查询，mongo数据通常存储在单个集合中，使用嵌入式文档将其分隔开，没有联合的概念

 mongosh/mongod运行命令
use database_name

show dbs 	查看所有数据库
db		查看当前数据库
db.dropDatabase() 删除当前数据库
show tables  查看所有集合(表)



插入一条数据

db.info.insert( { "姓名" : "江涛" "age" : 12}) //info是表的名字，没有就自动创建

mongoDB中一条记录的主键为 " _id " 字段，如果不显式添加，则会默认为你加上

插入多条数据

~~~mongo
var myEmploy = 
[
{
"name" : "jt",
"age" : 18
},
{
"name" : "wh",
"age" : 19
},
];

db.info.insert(myEmploy);
~~~



查询数据（json的方式）

db.info.find().forEach(printjson)   // 必须是forEach不能是foreach



查询指定条件的数据

db.info.find({"name" : "jt"}).forEach(printjson)
db.info.find({"age": {$gt : 12}}).forEach(printjson)  //>=12岁的人