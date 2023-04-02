docker教程： https://pengfeixc.com/tutorial/docker/docker-guide
		https://docker.easydoc.net/doc/81170005/cCewZWoN/AWOEX9XM

docker pull mongo 

docker run -p 27017:27017 --name mongodb -d mongo    //启动容器， 第一个端口号是host的，第二个是container中的

docker exec -it mongodb	mongosh  admin   //mongodb是容器名，mongosh是sh， admin是用户表(不加也可以)，默认还有config和local

docker exec mongodb bash  	//表示执行mongodb容器中的bash命令， 加上 -it 表示绑定， 相当于就能进入mongodb日期的bash终端

docker start mongodb	启动容器

持久化数据
docker run -it -v mongo_volume:/data/db/mongo -p 27017:27017 --name mongodb -d mongo   mongo_volume是挂载的名字， /data/db/mongo是具体路径