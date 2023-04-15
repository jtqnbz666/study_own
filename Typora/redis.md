




宏观思维（比如redis中的set，它要求的是无序唯一， 所以底层使用了**hash**，整数数组这两种数据结构来实现， zset 要求有序唯一， 底层使用了**跳表**和压缩列表的数据结构来实现）

大项目中使用keys * 会造成其他任务阻塞，因为数据量太大了，而且redis是单线程的**串行**模式，需要不断占用CPU去读取数据。

redis中命令的处理是 **单线程** **串行**的，所以当一条连接中的一个事务开始执行时，也就不会发生中途穿插别的连接的命令了，天然的隔离性，  而**mysql中是并发**(当连接数小于核心数就是并行)

使用scan替换，就像mysql中的游标
scan 0 match * count 1
0表示游标， 1 表示一个槽位，不一定只有一个数据， 可能这个槽位因为哈希冲突有多个数据

处于渐进式rehash阶段**不会**发生扩容缩容的情况

使用游标scan读数据的时候**可能发生**扩容或者缩容的情况，数据**一定不会**遗漏掉，但缩容可能会引起重复读取数据

redis内存分配器(jemalloc，内部是内存池)认为 <= 64字节  是小字符串， >64 是大字符串

跳表可以用来做分布式定时器。

为什么有些地方认为44是小字符串，有些认为是64， 其实是因为，64是包括了结构体(占用20字节)的部分， 而44是实际的字符串内容。

redis存储特性：
数据少的时候，为了节省空间， 采用压缩列表O(n);
数据多的时候，为了访问性能，采用其他更优结构(跳表，hash) ，做到O(1), O(logN)

维护窗口记录，比如游戏战绩(list)

~~~c
ltrim says 0 4
lrange says 0 -1
~~~

hash和 list配合实现购物车；

hash 和 string 存储字符串的区别

~~~c
hmset hash:10001 name mark age 18 sex male
# 与 string 比较
set hash:10001 '{["name"]:"mark",["sex"]:"male",["age"]:18}'
# 假设现在修改 mark的年龄为19岁
# hash：
hset hash:10001 age 19
# string:
get role:10001
# 将得到的字符串调用json解密，取出字段，修改 age 值
# 再调用json加密
set role:10001 '{["name"]:"mark",["sex"]:"male",["age"]:19}'
~~~

## 数据结构

string类型是二进制安全字符串， 如 char[20] = "hello\0", 它是以\0结束1的，而string类型是判断的长度，而不是以\0结束。类似std::string 

~~~
存储结构：
字符串长度小于等于20， 且能转成整数，则使用int存储
字符串长度小于等于44，则使用embstr存储
字符串长度大于44，则使用raw存储。
set jt 666
通过 OBJECT encoding jt来看到编码方式
通过 type jt 看类型
~~~



set 是一个无序集合，用来存储唯一性字段

~~~ 存储结构
存储结构：
元素都为整数且节点数量小于等于 512（set-max-intset-entries），则使用整数数组存储(排了序的数组 )；
元素当中有一个不是整数或者节点数量大于 512，则使用字典存储；

应用场景， 求QQ好友间的交集(共同好友)。 			SINTER  
A ,B是好友， 给A 推荐B的好友(可能认识的人)		 SDIFF
~~~

zset是一个有序集合, 	用来实现排行榜

~~~c
存储结构：
节点数量小于等于128(zset-max-ziplist-entries),且所有字符串长度小于等于64,则使用ziplist存储。
节点数量大于128或者有一个字符串长度大于64，则使用跳表(skiplist).
    
应用场景：排行榜，热榜

通过zset可以引申出很多功能：
1.延时队列：zset中包含key 和 member， 用member做消息命令， key 做到期时间，zset是有序(通过key排序)的，通过不断获取当前时间，来跟zset中的value对比，若超时了，就执行member这个消息。
2.分布式锁， 比如A,B,C,D同时去获取redis的锁，只有A获取到了，则B,C,D使用延时队列的方式，定时(随机一个时间)重新去获取锁，依靠延时队列来实现。也可也用redis中的发布订阅机制实现，
3.窗口限流：比如5分钟内只能点击10次， 通过时间的对比，维护一个5分钟内的所有点击数据的窗口， 计算这五分钟点击次数。2.我自己想的，点击的动作作为key，value为时间，每次点击都获取前边第九次点击的对象，将它的value与当前时间做比对，如果超过五分钟，那么此次点击有效并加入zset中，反之无视。
对于大量数据的情况，比如黑客攻击，就不能使用窗口限流，因为一个时间段内数据量可能很大， 需要使用漏斗限流
zrank，顺序 ； zrerank， 逆序
~~~



压缩列表：

~~~
第一个位置存储了最后一个数据的位置， 最后一个数据(比如第四个)存储了第三个数据的位置，一次类推，
是从最后一个位置开始遍历的。

为什么数据量大了就不用压缩列表了， 因为它效率变低了， 应该采取更高效的结构来存储
~~~

list是一个双向链表

~~~
链表首尾操作(删除和增加) 时间复杂度O(1);查找中间元素时间复杂度为O(n)
列表中数据是否压缩的依据：
1.元素长度小于48， 不压缩
2.元素压缩前后长度不超过8， 不压缩
如果满足条件，可以理解为节点的具体实现是压缩列表
~~~







redis pipeline  是 客户端提供的，而不是服务端。

~~~
实现了发多个请求，再同时收多个回应在pipeline中顺序处理， 而不是发一个等一个。 
send操作并不耗时， 只是将数据写到了连接(fd)的写缓冲区上， 真正耗时的是等待对方回应的过程。

~~~

## Redis事务和MySQL事务对比

~~~sqlite
MySQL中的事务是每一条都执行了的， 可以通过rollback回滚（通过undolog做逆运算回滚。）
Redis中的事务在未提交前是没有执行的，只是将命令放到了一个队列中，通过DISCARD取消这些命令，相当于rollback，区别在于一个是执行(mysql),一个只是记录操作(redis),Redis事务是一个单独的隔离操作：事务中的所有命令都会序列化、按顺序地执行。
事务在执行的过程中，不会被其他客户端发送来的命令请求所打断。
    
Redis中， MULTI 开启事务， discard取消事务， EXEC 执行事务，watch检测key的变动(是乐观锁实现方式（cas）)，分布式锁是悲观锁实现

redis中多客户同时设置键值时，必须要借助WATCH 来监测key，WATCH不能在事务中用， 需要在事务开启之前用。 当执行EXEC时，若用WATCH检测的key变化了， 则相当于是执行的DISCARD。

可以通过事务来实现一些redis没有提供的原子操作命令(配合watch)


~~~

**事务实现ZPOP**

zpop可以用在延时队列中

第一条连接中

![	](..\pic\1669257137058.png)

第二条连接中

![1669257147472](../pic/1669257147472.png)

**事务实现加倍操作**

![1669258639371](../pic/1669258639371.png)

## Lua脚本

在redis中是封装了一个lua虚拟机的， 但是是阉割版本的，如os库就被删了(不能调用sleep())， 在reids/deps/lua即可看到相应的库。

redis中lua脚本的执行是原子性的，当某个脚本正在执行的时候， 不会有其他命令或者脚本被执行。

![1669259215346](../pic/1669259215346.png)

![1669259507367](../pic/\1669259507367.png)

eval 不需要生成散列值， 将script 先编译成sha1 再执行   ， 测试使用

EVALSHA 散列值 参数个数  第一个参数 第二个参数 ...		 线上使用

使用lua脚本做热更新

~~~
先script flush 清除缓存， 在应用程序中程序加载script load 生成sha1，再去重新执行新任务
~~~

讲一下lua脚本在reids分布式锁中的作用。1.判断是不是， 2.删除，通过lua保证原子操作。

## ACID特性分析

事务中，如果有一条语句出错了， 后边的会**继续执行**。

**原子性**： 要么全部成功，要么全部失败
**一致性**： 若失败不影响数据库原始状态  ，  （对于一致性我们希望得到的是预期的一致性，而不是异常后的一致性， 比如说明明失败了，但对某个字段进行了修改，则不满足）

从第一句话可以分析出这两种特性都不满足。





**隔离性**：**事务的操作不被其他用户操作所打断**；redis的命令执行(单线程执行命令)是串行的，redis具备天然的隔离性。 

可能会有疑惑，明明影响了我刚开始看到的数据，为什么还具备隔离性呢， 仔细想想，mysql中查询语句是在一个事务**中**就能查出结果的，而redis中查数据的时候只有在事务开始前执行，如果是在事务中进行查询，它**只是被加入了待执行队列**，在提交的时候才会**真正**执行，所以这两者并不冲突，  所以自然的redis不存在mysql中的脏读幻读不可重复读问题。

面试问到**隔离性**需要分析两个问题。
若不想被别的连接影响期望结果，使用以下两种方式来实现

1.使用lua脚本实现完全隔离性，  2. 通过watch来监测 key是否发生改变，若改变，则EXEC命令会被当作DISCARD命令取消事务。

特别注意： 使用watch 和 事务在运行中出错继续执行是不一样的， 若用了watch，当key变化，EXEC命令会被当作DISCARD命令取消事务，根本就没执行， 而事务中语句有错继续执行又是另外一回事。





**持久性：**redis只有在aof持久化策略的时候，并且需要在redis.conf中appendfsync=always**才**具备持久性；实际项目中几乎不会使用aof持久化策略。



 **redis事务和lua脚本的比较：**
相同点：两者都实现了隔离性，如果执行期间出现运行错误，之前的执行不会回滚。
不同点：redis事务是基于乐观锁(比如使用watch)， lua脚本是基于redis的单线程执行命令， lua脚本可以**自定义逻辑**(比如获取中间结果后，使用if判断执行什么逻辑)，
Redis 使用了事务队列来预先将执行命令存储起来，并且会对其进行格式检查的，提前就知道命令是否可执行了。
比如jt:10001是string类型的key，如果你执行hset  jt:10001 wh ，这是错误的，因为wh 后边应该还需要一个val值才是正确的语法。

但像对string类型数据执行hashmap类型的操作是检测不出来的
将上边的hset  jt:10001 wh 换为 hset  jt:10001 wh 666，则可以加入事务队列，但实际上这条语句逻辑是有问题的。可以执行exec，这条语句无法成功执行，但并不影响事务中这条语句之后的命令继续执行。


**所以Redis 事务真正支持原子性的前提：开发者不要傻不拉几的写有逻辑问题的代码！**

## redis发布订阅

没有做持久化 用的少。

应用场景 ： 打印内存情况分析bug
比如在客户端publish(发布信息到redis)， 服务器作为订阅者向redis订阅这个信息。

## redis中的多线程

~~~c
int n = read(fd, buff, size);
msg = decode(buff, size); // redis io-threads
data = do_command(msg);
bin = encode(data, sz); // io-threads
send(fd, bin, sz1);
~~~

注意主线程只处理do_comman, 和 send ， 而其他线程去除了 解压缩和压缩的逻辑(decode, encode), 所以还是不需要加锁， fd通过roundrobin算法分配给其他线程， 这些线程去处理对应fd  中的信息的压缩和解压逻辑。

## redis协议

![1669268725565](../pic/1669268725565.png)

一般协议的制定规则，为了 **界定数据包** 

1.长度 + 二进制流
2.二进制流 + 特殊分隔符  如http

redis中执行   set hello world

~~~c
*3\r\n$3\r\nset\r\n$5\r\n\hello\r\n$5\r\nworld\r\n
~~~

是二进制安全字符串， 即set hello wor\r\nld也不会错， 因为是以开头的长度界定的

~~~c
*3\r\n$3\r\nset\r\n$5\r\n\hello\r\n$5\r\nwor\r\nld\r\n
~~~

## redis异步连接池

其实就是设置回调函数， 跟reactor模型一样， epoll + 回调函数配合

使用协程可以实现同步的编码方式，实现异步的效果。







第四节课

## redis计数

比如想获取一个网页的访问人数(不重复)。

zset，set， hash 都有**去重**的功能 ，并且zset，set还能记录是哪个用户访问多少次，并且zset还能根据次数进行排序，但string类型只能计数，却不能去重(即同一个用户访问多次)， 如何去重呢， **配合布隆过滤器**使用，布隆过滤器(存在假阳性)可以判断一定没有的情况，但不可以保证一定有， 当判断到一定没有，则+1， 这样string类型也能实现去重的功能了.

当数据量太大，比如10亿数据，此时就算使用布隆过滤器，也需要大量内存才能够实现(10~100G)， 不够明智。**更好的办法**就是采用HyperLogLog(采用少量内存 实现去重计数，也有误差， 0.8125%)



**HyperLogLog：少量内存下统计一个集合中唯一元素数量的近似值**；
HLL 采用的是调和平均数（倒数的平均）。调和平均可以有效平滑离散值的影响；比如一个很大的数，倒数就是很小的数，减少大数的影响

**算法**：

当一个元素到来时，它会散列到任意一个桶(根据计算出的64位哈希值的后14位决定)中，以一定的概率影响这个桶的计数值，因为是概率算法，
单个桶的计数值并不准确，但是将所有的桶计数值进行调和均值累加起来，结果就会非常接近真实
的计数值；

在 Redis 的HLL中 共有2的14次方(16384)个桶，而每个桶是一个 6bit 的数组；
hash 生成 64位整数，其中**后 14 位用来索引桶子**；后面 50位 共有2的50次方用来统计累计0的个数；保存对数的话最大值为 49(一共50位，第一位是1，其余全是0的数)；6位对应的是 对应整数值为 64 可以容纳存储 49；

![1669309256483](../pic/1669309256483.png)

如果你不懂为什么每个桶是6位， 因为它没有采用那种直接画64个桶的方式， 而是使用6位来表示可以有64个桶，也就是最大能表示2^64次方的数

**去重**：

相同元素通过hash函数会生成相同的 64 位整数，它会索引到相同的桶子中，累计0的个数也会相同，按照上面计算最长累计0的规则，它不会改变该桶子的最长累计0，最终计算出来的值是一样的，说明重复了。

**存储**：

redis 中的 hyperloglog 存储分为稀疏存储和紧凑存储；
当元素很少的时候，redis采用节约内存的策略，hyperloglog采用稀疏存储方案；当存储大小超过
3000 的时候，hyperloglog 会从稀疏存储方案转换为紧凑存储方案；紧凑存储不会转换为稀疏存
储，因为hyperloglog数据只会增加不会减少（不存储具体元素，所以没法删除）；
配置： hll-sparse-max-bytes 3000 





哪些结构可以**做窗口**？
list：顺序插入，可以得到一段数据
zset：通过score排序， 进行数据筛选得到一段数据

## redis拓展



redismodule.h是所有接口的封装

src/modules 中  helloblock.c, hellocluster.c, hellotimer.c

hellotype.c必须看，讲的如何发送数据给server端

RedisModuleCtx 上下文
RedisModuleString 传递的参数数组

在redis.conf 中进行配置，即可调用这些文件，

比如我的redis是在/home/jt/中

需要在redis.conf中加上

loadmodule   /home/jt/redis/src/modules/hellotimer.so
若在它的后边再加参数， 会作为参数传到.so文件的主函数中











第五课

## redis持久化

redis的数据全部在内存中， 如果突然宕机， 数据就会全部丢失， 因此需要做持久化， redis宕机后能用这些持久化文件恢复数据。

在redis.conf中与持久化相关的字段

~~~shell
###### aof ######
# redis.cnf
appendonly no
appendfilename "appendonly.aof"
# appendfsync always
appendfsync everysec		#每秒一刷
# appendfsync no
# auto-aof-rewrite-percentage 为 0 则关闭 aof 复写
auto-aof-rewrite-percentage 100	#百分比的意思
这里有个细节， 比如现在redis的数据内容是4k， 若设置为100，则当redis数据到达8k时，发生aof复写，以此类推，当达到16k时再次aof复写(复写会fork进程)。


auto-aof-rewrite-min-size 64mb
这个64mb的设置就是为了防止在数据量很小时，但一翻倍(比如32bit 到 64bit)就发生aof复写，造成没必要的少量数据aof复写。

aof-load-truncated yes
# 如果yes 当 aof 数据不完整，尽量读取最多的格式正确的数据；
# 如果no  当 aof 数据不完整 报错，可以通过 redis-check-aof 来修复 aof 文件；

# 开启混合持久化
aof-use-rdb-preamble yes
###### rdb ######
# save ""
# save 3600 1
# save 300 100
# save 60 10000

~~~



### aof:

存储内容：比如set hello world
则存储的东西为`*3\r\n$3\r\nset\r\n$5\r\n\hello\r\n$5\r\nworld\r\n`

配置：

~~~shell
# 开启 aof
appendonly yes
# 关闭 aof复写
auto-aof-rewrite-percentage 0
# 关闭 混合持久化
aof-use-rdb-preamble no
# 关闭 rdb
save ""
~~~

策略

~~~shell
# 1. 每条命令刷盘 redis 事务才具备持久性
# appendfsync always
# 2. 每秒刷盘
appendfsync everysec
# 3. 交由系统刷盘
# appendfsync no

其中第一个方法性能消耗最大， 但也只有这样才能无误的做到redis持久化， 第三个方法误差可能最大。
~~~

缺点

时间越长，日志会越来越长， 若采用aof恢复数据，导致redis长时间无法对外提供服务。



### aof rewrite

aof 持久化策略会持久化所有修改命令；里面的很多命令其实可以合并或者删除；
比如lpush lists key1 , lpush lists key2 , lpush lists key3 会被合并成一条命令
比如add key 1 ，  del key 1  ，对于aof复写，这种操作直接就不记录进文件。

aof rewrite 在 aof 的基础上，满足一定策略则 fork 进程，根据当前内存状态，转换成一系列的 redis 命令，序列化成一个新的 aof 日志文件中，序列化完毕后再将操作期间发生的增量 aof 日志追加到新的 aof 日志文件中，追加完毕后替换旧的 aof 日志文件；以此达到对 aof 日志瘦身的目的；
注意：aof rewrite 开启的前提是开启 aof；

![1669349997075](../pic/1669349997075.png)

配置

~~~shell
set key val
# 开启 aof
appendonly yes
# 关闭 aof复写
auto-aof-rewrite-percentage 0
# 关闭 混合持久化
aof-use-rdb-preamble no
# 关闭 rdb
save ""
~~~

策略

~~~shell
# auto-aof-rewrite-percentage 为 0 则关闭 aof 复写

auto-aof-rewrite-percentage 100	#百分比的意思
这里有个细节， 比如现在redis的数据内容是64k， 若设置为100，则当redis数据到达128k时，发生aof复写，以此类推，当达到256k时再次aof复写。

auto-aof-rewrite-min-size 64mb
这个64mb的设置就是为了防止在数据量很小时，但一翻倍(比如32bit 到 64bit)就发生aof复写，造成没必要的少量数据aof复写(因为每一次复写都会fork进程)。
~~~

缺点

尽管在aof基础上瘦身了，但aof复写的数据量仍然很大，加载很慢。

### rdb

基于aof或aof复写文件大的缺点， 出现了rdb持久化，它是一种快照持久化；它通过fork主进程， 在子进程中将内存当中的数据键值对按照存储方式持久化到rdb文件中，rdb存储是经过**压缩的二进制**数据。

配置：

~~~shell
# 关闭 aof 同时也关闭了 aof复写
appendonly no
# 关闭 aof复写
auto-aof-rewrite-percentage 0
# 关闭 混合持久化
aof-use-rdb-preamble no
# 开启 rdb 也就是注释 save ""
# save ""
# save 3600 1
# save 300 100
# save 60 10000

如果需要关闭rdb，就把save "" 的注释打开，因为当注释掉它，默认就是打开了rdb模式，并且是最后这三行的策略
~~~



策略：

~~~shell
# redis 默认策略如下：
# 注意：写了多个 save 策略，只需要满足一个则开启rdb持久化
# 3600 秒内有以1次修改
save 3600 1
# 300 秒内有100次修改
save 300 100
# 60 秒内有10000次修改
save 60 10000
~~~

需要注意的问题是，假设现在有99数据，后边两个都不满足，所以会在一小时后才进行持久化， 如果这期间redis宕机，则数据丢失。

缺点：

若采用rdb持久化，则一旦redis宕机，redis将丢失一段时间的数据(fork进行rdb持久化的时间)。

RDB 需要经常 fork 子进程来保存数据集到硬盘上，当数据集比较大的时候，fork 的过程是非常耗时的，可能会导致 Redis 在一些毫秒级内不能响应客户端的请求。如果数据集巨大并且 CPU 性能不是很好的情况下，这种情况会持续1秒，AOF 也需要 fork，但是你可以调节重写日志文件的频率来提高数据集的耐久度。

### 混合持久化

rdb文件小且加载快但丢失多，aof文件大且加载慢但丢失少；

混合持久化是结合两者优点的一种持久化方案；aof复写的时候实际持久化的内容是rdb， 等持久化后，持久化期间修改的数据以aof的形式附加到文件的尾部。

这里可能有因为，一个文件混了两种不同持久化方式的内容，怎么靠这个文件恢复数据呢？
其实在rdb内容前边会有标识符号，标识后边的内容是rdb，同样在aof内容的前边也会有标识符号，标识接下来是aof内容。



配置：

~~~shell
# 开启 aof
appendonly yes
# 开启 aof复写
auto-aof-rewrite-percentage 100
auto-aof-rewrite-min-size 64mb
# 开启 混合持久化
aof-use-rdb-preamble yes
# 关闭 rdb
save ""
# save 3600 1
# save 300 100
# save 60 10000
~~~

应用：

1.MySQL缓存方案中， redis不开启持久化， redis只存储热点数据，数据的依据来源于MySQL，若某些数据经常访问需要开启持久化，此时可以选择rdb持久化方案，也就是允许丢失一段时间的数据；

2.对数据可靠性要求高的， 在机器性能、内存也安全(fork写时复制 最差的情况可能内存翻倍，比如48G->96G，所以需要保证内存足够)的情况下，可以让redis同时开启aof和rdb，注意此时不是混合持久化，redis重启优先从aof加载数据， 理论上aof包含更多最新数据，如果只开启一种，那么就使用混合持久化。

3.在允许丢失的情况下， 亦可采用主redis不持久化(比如96G ，即可最大限度使用，比如能用90G内存存储数据)， 让从redis进行持久化；

4.伪装从数据库， 比如加载一个dll动态库文件，让这个程序冒充一个从数据库。但这个dll程序的实现并不需要fork子进程来持久化，就不用考虑内存翻倍的情况。

需要注意一点，因为fork最差情况会导致内存翻倍，所以在redis.conf中可以设置redis程序能占用内存的最大值。



**数据安全策略**

问：拷贝持久化文件是否安全？

是安全的， 持久化文件一旦被创建， 就不会进行任何修改，当服务器要创建一个新的持久化文件时，它先将文件的内容保存在一个临时文件里面，当临时文件写入完毕时，程序才使用rename(2)原子的用临时文件替换原来的持久化文件

数据安全要考虑两个问题：

1.节点宕机(redis是内存数据库，宕机数据会丢失)
2.磁盘故障

- 创建一个定期任务，每小时将一个RDB文件备份到一个文件夹， 并且每天将一个RDB文件备份到另一个文件夹中。
- 确保快照的备份都带有相应的日期和时间信息，每次执行定期任务脚本时，使用find命令来删除过期的快照；比如说，你可以保留最近48小时内的每小时快照，还可以保留最近一两个月的每日快照。
- 至少每天一次，将RDB备份到你的数据中心之外，或者至少是备份到你运行redis服务器的物理机器之外

持久化方案可以解决节点宕机问题， 但磁盘故障必须要在另一台物理机器上进行数据备份

## redis主从复制

以下主redis数据库用master表示, 从redis数据库用slave表示

因为是异步实现, 主从redis应**放在一个局域网**里,能减少网络问题带来的影响. 而且slave不一定是一个,

防止redis所在磁盘损坏,造成数据永久丢失,也有读写分离的作用

**全量数据同步**

slave第一次连接到master时,master会发送完整的rdb文件给slave(异步发送), slave**根据rdb文件**重做内存区.

**增量数据同步**

Master 采用了一个环形缓冲区来记录需要同步给slave的数据.
增量数据同步涉及两种情况, slave掉线重连,或者是正常的slave.

增量同步不依赖rdb或者aof,因此master是否开启rdb或aof都不影响

**为什么采用环形缓冲?**
1.如果从redis掉线时间**过长**就只能采用全量数据同步, 限制了可恢复数据的范围, 不能使用无限大的缓冲来进行数据同步
2.避免了数据拷贝(memcpy) 

会有两个**复制偏移量**(一个在主, 一个在从)记录这个环形缓冲 还有哪些内容是最新的,没有同步到slave的.  因为master是一直在更新这个环形缓冲区的,并且这个环形缓冲区的大小是固定的, 当slave重新连接上来发现自己的(slave)复制偏移量已经没有在这个环形缓冲区的范围内了, 则只能采用全量数据同步.

**怎样判断一个slave连接master的时候是掉线重连还是新的salve连接?**
服务器**RUN ID**, 无论是master还是salve都有自己的RUN ID, RUN ID启动时自动产生, RUN ID 由40个随机的十六进制字符组成;

当slave对master初次复制时,master将自身的RUN ID 传送给slave, slave会将RUN ID 保存, 当slave断线重连master时, slave会向master发送之前保存的RUN ID, 以此来判断是新的slave还是断线重连的slave.

- slave RUN ID  master RUN ID 一致,说明是slave掉线前复制的就是当前的master, master 会先尝试增量同步操作, 因为有可能刚掉线不久, 也行还能使用环形缓冲区进行增量同步,如果检测到slave的复制偏移量已经不在环形缓冲区的范围了,则采用全量同步
- 若不一致,则说明是新的slave, 则master对slave执行全量同步

**复制偏移量offset**

主从都会维护一个复制偏移量:

- 主库向从库发送N个字节的数据时,将自己的复制偏移量加上N
- 从库接收到主库发送的N个字节数据时,将自己的复制偏移量加上N

通过比较主从偏移量得知主从之间数据是否一致, 偏移量相同则数据一致, 偏移量不同则数据不一致.

**环形缓冲区(复制积压缓冲区)**

本质:固定长度的先进先出队列
当因某些原因(网络抖动或从库宕机) 从库与主库断开连接, 避免重新连接后开始全量同步, 在主库设置了一个环形缓冲区, 该缓冲区会在从库失联期间累计主库的写操作, 当从库重新连接,会发送自身的复制偏移量给主库,主库会比较主从的复制偏移量

- 若从库offset还在复制积压缓冲区中, 则进行增量同步
- 反之, 执行全量同步

~~~shell
#redis.conf
repl-backlog-size 1mb
#如果所有从库断开连接3600秒,则释放环形缓冲区
repl-backlog-ttl 3600
~~~

环形缓冲区的大小:disconnect_time * write_size_per_second
disconnect_time ：从库断线后重连主库所需的平均时间（以秒为单位）；
write_size_per_second ：主库平均每秒产生的写命令数据量；

![1669377529034](../pic/1669377529034.png)

像10000, 10001 这些就是复制偏移量, 是一个64位的数字,  永远用不完,能表示的范围太大了.

## redis哨兵模式

在这之前, 都没有实现可用性. 哨兵模式其实就是主从模式的一种升级.

主从复制只是数据同步的一种方式,但不涉及宕机恢复问题(可用性)

**什么是可用性: 看主redis宕机,多久才能恢复正常使用**(看redis切换恢复正常的速度)

哨兵模式采用 raft算法, 是redis可用性的解决方案, 它由一个或多个sentinel实例构成sentinel系统,  该系统可以监视任意多个主库,以及这些主库包含的从库, 当主库宕机时, 自动将这个主库所属的某个从库升级位新的主库.

若主库发生变化(原主库宕机, **如何选择新的节点, 根据每个从库的复制偏移量来决定,看谁拥有最新的数据**),哨兵会自动更改配置文件更新为最新主库信息, 并且会通知其他从库来连接新的主节点.

在哨兵节点的配置文件中,只需要配置主库的信息(ip, port),  可以通过主库来获取其他从库的信息,哨兵内部会通过命令互相建立连接,并且每一个哨兵会与所有redis节点(主库+从库)建立连接. 
![1669379383086](../pic/1669379383086.png)

![1669383274192](../pic/1669383274192.png)

每一个客户端会配置所有哨兵节点的信息, 采用取余的方式连接任意**一个**哨兵节点即可(因为若这个哨兵节点宕机了,要尝试去连接另外的哨兵节点). 

**配置**

~~~shell
#sentinel.cnf
#sentinel 只需指定检测主库就行了,通过主库自动发现从库,因为主库会有每个从库的信息.  
sentinel monitor mymaster 127.0.0.1 6379 2 
#这个2表示 配置了3个sentinel节点,当有2个(超半数)sentinel支持就可以执行相应处理,因为内部是用的raft算法
#判断主观下线时长
sentinel down-after-millise mymaster 30000
#指定在发生故障转时最多可以有多少个slave同时对新的master进行 同步, 这个数字越小同步时间越长, 而越大, 则对网络资源要求越高.
sentinel parallel-syncs mymaster 1
#指定故障切换允许的毫秒数, 超过这个时间,就任务故障切换失败, 默认为3分钟
sentinel failover-timeout mymaster 180000
~~~

**检查异常**

主观下线:
sentinel会以每秒一次的频率向所有节点(其他sentinel, 主节点, 以及从节点)发送ping消息, 然后通过接收返回判断该节点是否下线, 如果超过配置中的down-after-milliseconds则被判断为主观下线.

客观下线
当一个sentinel节点将一个主节点判断为主观下线之后,为了确认这个主节点是否真的下线, 它会向其他sentinel节点进行询问, 如果收到一定数量的已下线回复, sentinel会将这个主节点判定为客观下线,并通过领头sentinel节点对主节点执行故障转移(因为是raft算法,分布式一致性算法, 所以需要领头的来决定).

故障转移
主节点被判定为客观下线后, 开始领头sentinel选举, 需要一半以上的sentinel支持, 选举领头sentinel之后, 开始执行对主节点的故障移除.

- 在从节点中选举一个从节点作为新的主节点(**根据每个从节点中维护的复制偏移量来选, 越大说明数据越新**)
- 通知其他从节点这个新的主节点(之后复制这个新节点的数据)
- 若故障主节点重新连接, 将作为新的主节点**的从节点**

缺点:
redis采用异步复制的方式, 意味着当主节点挂掉时, 从节点可能没有收到全部的同步消息, 这部分未同步的信息将丢失, 如果主从延迟特别大, 那么丢失的可能会特别多, sentinel无法保证消息完全不丢失, 但是可以通过配置来尽量减少数据丢失.

~~~shell
#主库中必须有一个从节点在进行正常复制,否则主库就停止对外写服务, 此时丧失了可用性
min-slaves-to-write 1
#这个参数用来定义什么是正常复制, 该参数表示在10s内没有收到从库的反馈, 就意味着从库同步不正常
min-slaves-max-lag 10
~~~

同时, 哨兵模式的致命缺点是不能进行横向拓展(因为有中心 , 哨兵相当于是这种模式的中心,而redis cluster就是去中心的).
在Redis3.0以前的版本主要实现集群一般是借助哨兵sentinel工具来监控master节点的状态，如果master节点有异常，则会做主从切换，将某一台slave作为master，哨兵的配置略微复杂，并且性能和高可用方面表现一般，特别在主从切换的瞬间存在访问瞬断的问题，而且哨兵模式只有一个主节点对外提供服务，没法支持很高的并发量，且单个主节点的内存也不宜设置过大，否则会导致持久化文件过大，影响数据恢复或主从同步的效率.
每个节点都是相同的数据,  并没有将数据均匀的分散出去.

## redis cluster 集群

![1669383040386](../pic/1669383040386.png)

Redis集群是一个由多个主从节点群组组成的分布式服务集群，他具有**主从复制、高可用、分片特性**，Redis集群不需要sentinel哨兵(去中心化)，也能完成节点移除和故障转移的功能，需要将每个节点设置成集群模式，这种集群模式没有中心节点，可水平扩展；Redis集群的性能和高可用均优于之前版本的哨兵模式，且集群配置非常简单


当集群中**增加**节点的时候, 客户端会触发断开连接操作,把之前的redis集群的连接信息全部断开, 重新获取集群槽位配置信息.

当集群中有节点宕机时, 每个主节点都有几个从节点, 从这几个从节点中选取数据最新的(根据**复制偏移量**)从节点作为这个槽位的主节点

本来client是可以知道某个key对应在哪个redis节点上的, 但如果比如key 在 redis_5上,但redis_5正在将这个key迁移到redis_6上, 当client来想client_5获取这个key时,会被告知这个key在redis_6上,当redis_5将需要迁移给redis_6的数据迁移完后,再将 当前redis_5还具有哪些key的信息同步给client.

不同槽位的主库之间是有联系的, 但主库所属的从库之间是没有联系的.  

## 大 KEY

影响：

1.客户端阻塞， 比如请求大key的时候，服务器是单线程，响应回复较慢，表现为客户端阻塞。

2.网络阻塞：每次获取大key产生的网络流量比较大，比如每个key是1MB， 每秒访问量1000， 则每秒产生1000MB的流量

3：阻塞工作线程：如果使用del删除大key时，会阻塞工作线程，这样就没办法处理后续的命令

4：内存分布不均，集群模式在slot分片均匀情况下，会出现数据和查询倾斜情况，部分有大key的Redis节点占用内存多，则QPS(每秒查询量)也会比较大

### 对于AOF日志的影响：

redis有三种持久化AOF日志策略：

always，每次**写**操作命令都会先写AOF日志数据再调用fsync写回磁盘;(主线程写磁盘，影响主线程)
所以如果是对大Key进行操作，阻塞的时间比较久，因为写入的数据量很大就会很耗时

Everysec： 每秒，每次**写**操作命令执行完，先将命令写入AOF文件的内核缓冲区，然后每个一秒将缓冲区里的内容写到磁盘；(异步写磁盘，不影响主线程)

No：意味着不由Redis控制写回磁盘的时机，交由操作系统再合适的时机写入磁盘，但还是会先将命令先写到AOF文件的内核缓冲区中。(操作系统负责，不影响主线程)

### 大key 对AOF重写和RDB影响

当AOF日志写入很多大Key，那么就会很快触发AOF重写机制

AOF重写和RDB快照(bgsave命令)一样，都会通过fork()创建一个子进程来处理任务。

 尽管是写时拷贝机制，**内核会把父进程的页表复制一份给子进程**，但大key的页表较大，则会比较耗时，那么在fork函数的时候就会发生阻塞的现象。

如果在重写的过程中，父进程修改了大key，则会发生写时复制，会把物理内容复制一份，这个过程也比较耗时，可能发生阻塞.

额外提一下,如果linux开启了内存大页,也会影响redis性能,比如客户端只修改了100b数据,但redis需要拷贝2MB的大页, 如果是常规内存页机制,只用拷贝4kb, 大页的好处,**减少缺页异常处理**.(用户态到内核态的切换)

### 如何避免大Key

设计的是时候就把key拆成小key,定时检查redis是否存在大key ,如果该大key可以删除,不要使用DEL命令,因为该命令会阻塞主线程, 而是使用unlink删除大key,因为该命令的删除过程是异步的,不会阻塞主线程.

### 如何找到大key

1:加上-- bigkeys

如:redis-cli -- bigkeys

注意事项:
最好在从节点上执行该命令, 因为主节点上执行,会阻塞主节点
如果没有从节点, 那就在业务压力低的时候执行, 或者使用 -i 参数控制扫描间隔, 避免长时间扫描降低redis性能

不足之处:
这个方法只能返回每种类型最大的那个key(比如string 类型,返回最大的那个, 若是 list 或者set等 则返回的是这个key值包含的value个数) 个数多不一定就大,因为val可能很小.

2:使用SCAN命令查找大key

-- bigkey 原理也是SCAN命令
用SCAN对数据库扫描,用type获取返回的key类型
如果是string类型, 则直接用strlen获取长度
对于集合类型:如果能预先从业务层知道集合元素的平均大小,那么只有求出集合中的元素个数,相乘即可

求集合类型个数:
 List类型:HLEN命令, Hash类型:HLEN命令, Set类型: SCARD命令; Sorted Set类型: ZCARD

3:使用rdbTools工具查找大Key
原理就是分析RDB文件

## 缓存击穿，穿透，雪崩

穿透：**不针对同一个key**，mysql和redis都没有数据
解决方案 1.key-null, 2.布隆过滤器， 3.非法请求的现在(在API入口处，判断请求参数)

击穿：**同一个key**过期了，大量请求访问这个key
解决方案：1.加锁，2.设置热点数据永不过期

雪崩：1.相同时间key同时过期 2. redis故障宕机
解决方案：1.均匀设置key的时间，避免同时失效 2.缓存定时预先更新. 3.双key策略 4.加锁
若是因为故障宕机: 1.服务熔断， 2.限流机制 3. 构建redis缓存高可靠集群

拓展：如何在redis的前边做优化？ 在应用层建立缓冲区

## Redis过期策略

定期删除 + 惰性删除

1.定期删除

每隔一定时间，就随机抽取一些设置了过期时间的key， 检测这些key是否过期，如果过期则删除

2.惰性删除

惰性删除不是主动删除，如果上一步的定期删除没有删除到某些过期的key，那么当我们尝试获取某个key的时候，会先去检测一下这个key是否已经过期，如果过期了则删除，如果没有过期则返回结果给你。

### redis内存淘汰机制

如果定期删除和惰性删除都没有将某些key删掉，这些key就会一直在内存中，所以redis又引入了“内存淘汰机制”。

内存淘汰机制在redis内存占用过高的时候就会进行内存淘汰， 主要有两种常用的，lru和lfu

~~~
no-eviction 当内存不足，新写入操作报错，无法写入新数据，一般不采用
allkeys-lru :移除最近最久未使用的，这个是最常用的
volatile-lru: 在设置了过期时间的key中，移除最近最久未使用的
~~~

### 其他一些对过期key的处理

1.快照生成RDB文件时
过期的key不会被保存在RDB文件中

2.服务重启载入RDB文件时

master载入RDB时，文件中的未过期的键会被正常载入，过期键则会被忽略，Slave载入RDB时，文件中的所有键都会被载入，当主从同步时，再和Master保持一致

3.AOF重写时
过期的key不会被记录到AOF文件中

4.主从同步时
mater删除过期key之后，会向所有slave服务器发送一个del命令，slave收到通知后，会删除这些key；
slave在读取过期健时，**不会做判断删除操作(也就是不检查是否过期)**，而是继续返回该健对应的值，只有当master发送del通知，slave才会删除过期健， 这是统一、中心化的健删除策略，保证主从服务器的数据一致性。