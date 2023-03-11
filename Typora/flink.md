

输入 java -version 看是否有环境

输入mvn -version  看是否有环境

通过如下命令来创建项目

~~~
mvn archetype:generate \    
-DarchetypeGroupId=org.apache.flink \
-DarchetypeArtifactId=flink-quickstart-java \    
-DarchetypeVersion=1.6.1 \    
-DgroupId=my-flink-project \    
-DartifactId=my-flink-project \    
-Dversion=0.1 \    
-Dpackage=myflink \    
-DinteractiveMode=false

需要注意，不行的话就把 \ 去掉，合并成一行
~~~



理解flink中的api的用法

![img](https://img-blog.csdnimg.cn/20201005164707339.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FwcGxleXVjaGk=,size_16,color_FFFFFF,t_70)