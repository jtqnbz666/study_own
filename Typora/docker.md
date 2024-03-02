动态看容器内存变化

Docker stats 容器id

常用参数

~~~
-e:环境变量
-d:后台运行
-v:挂载
-p:端口映射
-it: i表示交互式，t表示terminal终端
~~~

如果因为修改容器内配置导致容器无法启动

~~~
1.修改前先备份, 
docker cp <容器ID>:/fluentd/etc/fluent.conf .
2.把备份的再导回去
docker cp fluent.conf <容器ID>:/fluentd/etc/fluent.conf
~~~

构建镜像，并且配合一些脚本(fluentd举例)

Dockerfile

~~~~
FROM fluent/fluentd:v1.14.2-debian-1.0
USER root
RUN gem install fluent-plugin-<your-plugin-name>:<version> --no-document
USER fluent
~~~~

~~~
1.构建镜像
docker build -t my-fluentd .
2.运行镜像
docker run -d --name my-fluentd-container my-fluentd
~~~

进入容器

~~~shell
# root 身份进入
docker exec -it -u root 7c498fa1dab8 sh (sh或者/bin/bash情况不同用的不一样)
~~~

**查看容器详情**

~~~shell
docker inspect fluentd 查看某个容器的详情(比如想看文件挂载相关就搜mounts)
~~~

查看所有容器(包括异常停止的 (docker ps看不到停止的))

~~~shell
docker container ls -a | grep dev2
~~~

### zookeeper

~~~shell
docker run -d --name zookeeper -p 2181:2181 -t docker.hoxigames.com/zookeeper:1.0
~~~

### kafka

~~~shell
# 先跑起来zookeeper
set kafkaIp 192.168.0.104
docker run -d --name kafka -p 9092:9092 -e KAFKA_BROKER_ID=0 -e KAFKA_ZOOKEEPER_CONNECT=$kafkaIp:2181 -e KAFKA_ADVERTISED_LISTENERS=PLAINTEXT://$kafkaIp:9092 -e KAFKA_LISTENERS=PLAINTEXT://0.0.0.0:9092 -t docker.hoxigames.com/kafka:1.0
~~~

### mongo

~~~shell
docker run -p 27017:27017 --name mongodb -d mongo    //启动容器， 第一个端口号是host的，第二个是container中的
~~~

**进入mongo容器方式**

~~~shell
docker exec -it mongodb	mongosh  admin   //mongodb是容器名，mongosh是sh， admin是用户表(不加也可以)，默认还有config和local

docker exec mongodb bash  	//表示执行一次mongodb容器中的bash命令， 加上 -it 表示绑定， 相当于就能进入mongodb容器的bash终端
有时候可能没有bash，就使用sh

#启动mongo容器
docker start mongodb	
~~~

### mysql

~~~shell
328项目:
docker run --name mysql -v /Users/a123/.deploy-tool/dev/mysql:/var/lib/mysql -p 3306:3306  -e MYSQL_ROOT_PASSWORD=123456     -e MYSQL_DATABASE=project328   -d docker.hoxigames.com/mysql:8.0 --default-authentication-plugin=mysql_native_password


// 自测
docker run --name=mysql-server -p 3306:3306 -v mysql-data:/var/lib/mysql -e MYSQL_ROOT_PASSWORD=123456 -d mysql  //运行mysql,  最后这个mysql表示镜像名(比如hoxi的时候是docker.hoxigames.com/mysql:5.7)

docker exec -it mymysql mysql  -u root -p  //连接mysql
~~~

### redis

~~~shell
#-d表示后台运行
docker run --name conn-redis -v ~/.deploy-tool/dev/conn-redis:/data -p 6381:6379 -d docker.hoxigames.com/redis:6.2.4-alpine3.13

docker run  --name data-redis -v ~/.deploy-tool/dev/data-redis:/data -p 6380:6379 -d  docker.hoxigames.com/redis:6.2.4-alpine3.13
~~~

**持久化数据**

~~~shell
docker run -it -v mongo_volume:/data/db/mongo -p 27017:27017 --name mongodb -d mongo   mongo_volume是挂载在本机的位置， /data/db/mongo是容器内部存储信息的位置。
~~~

**查看日志**

~~~shell
docker logs 容器名
持续看
docker logs 8fd5abf06c6d --tail 10 -f
docker logs xxxx --timestamps > a.log // 加上时间戳
~~~

**docker tag** 

~~~
执行了
1.docker tag  docker.hoxigames.com/project328/user-data-service-image:test docker.hoxigames.com/project328/user-data-service-image:26dbf1561434-dcfe52aa20dd
2.docker tag  docker.hoxigames.com/project328/user-data-service-image:test jt:v1

结果:
docker images
REPOSITORY                                                              TAG                         IMAGE ID       CREATED         SIZE
docker.hoxigames.com/project328/user-data-service-image                 26dbf1561434-dcfe52aa20dd   77fd8871a9af   5 hours ago     163MB
docker.hoxigames.com/project328/user-data-service-image                 test                        77fd8871a9af   5 hours ago     163MB
jt

注意点：
1.执行docker build的时候如果镜像名字不带tag，那默认是latest，问题就是每一次docker build都会覆盖上一次(因为都是latest)
2.他们的imageID是一样的，但他们是独立的，删除其中一个不会影响别的， 如果想要不同的imageID那就得使用docker build 镜像名:tag
~~~



**docker网络相关**

~~~
docker上的容器如果不指定网络，默认和本机上一个网络中。如果给docker容器指定了network，那么本地就跟docker容器不在同一个网段内了，docker和本机就不能进行通讯了，但是docker中放到同一个network的容器可以相互通信。
~~~

**拷贝docker里面的文件到本地**

~~~
docker cp <容器名称或ID>:<容器内文件路径> <本地目标路径>
然后配合scp搞到本地, 

scp deploy@47.95.6.108:~/divideLevel.json .
或者上传服务器
scp divideLevel.json  deploy@47.95.6.108:~/
~~~

**停掉所有9005开头的docker容器**

~~~
containers=$(docker ps -aq --filter "name=^/9005")
docker stop $containers
~~~

**docker基础学习**

~~~
docker教程： https://pengfeixc.com/tutorial/docker/docker-guide
		https://docker.easydoc.net/doc/81170005/cCewZWoN/AWOEX9XM

docker官网镜像：https://hub.docker.com/

下载好docker后去设置中配置引擎加上一行
 "registry-mirrors":["https://mirror.ccs.tencentyun.com"],
~~~

