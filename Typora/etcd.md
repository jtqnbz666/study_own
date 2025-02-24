## etcd

etcd基本操作

~~~c++
put key val  //设置key
get key		//获取最新的key-value
get key --rev=66 //获取版本号为66的key-value对
get key --prefix //通过前缀匹配所有有关的key
get key -w json  //查看key的详细信息，数据是base64编码(兼容2进制数据)
get key --keys-only //只返回key,不管value
get key --limit=2 //只看两个
get key --order=descend //降序的方式
get key --sort-by=value//默认是按照key排序,可以指定用value、create、modify、version进行排序
get key key3   //表示获取key到key3范围之间的key-value，是左闭右开的
//比较是依据字符串比较的,比如key12 是小于 key2的
get key2 --from-key //表示获取大于等于key2的所有key-value
  
  
  
租约：
    
lease grant + 秒数 //创建一个租约 ，会获得一个唯一的串来标识租约
put key --lease=刚才获得的那个串 //将key挂到这个租约上

lease keep-alive 刚才获得的那个串 //续租

lease revoke 刚才获得的那个串 //删除租约，则所以挂载的key都会删除
    
lease timetolive 刚才获得的那个串 //查看租约的信息

watch  key 命令，监听key的变化
watch key --prefix 监听包含key前缀的所有key
~~~

一个key对应的版本相关的信息

```c++
create_revision 创建数据时，对应的revision
mod_revision 数据修改时，对应的revision
version  当前这个key的版本号，也就是当然这个key修改了几次

linux下通过命令get key -w json | json_pp (json_pp表示以美观的格式输出) 来查看这些信息，其中还可以看到一个raft_term字段， 表示leader的选举次数
```

etcd事务

~~~c++
txn -i 开始事务，有三个部分组成， 1.compares:比较阶段，2.比较成功了执行什么，3.比较失败了执行什么

比如：
compares:
value("key") = "others"
//mod("key") = "66" 查看key的mod_revision是不是66
//create("key") = "67" 查看key的cteate_revision是不是67
//version("key") = "3" 查看key的version是否是3
    
success request(get, put, del):
del key

failure requset(get, put, del):
put key jt666
get key
 
 
事务语句中如果出现语法错误会中断退出， 比如输入qwe xyz
~~~

etcd读写机制

~~~c++
串行写， 并发读
1. 写+并发读情况， 去读磁盘上的b+树， 写的时候先根据raft算法询问其他节点，如果通过提交再记录wal日志，写到内存中的b树， 所以通过b+树访问到的磁盘数据不可能出现脏读， 和mysql中一样的利用mvcc解决不可重复读问题，对于幻读(读+读 或 读+写)，etcd是key-value结构，不会向mysql那样出现读+读(一会一条数据，一会多条数据)的幻读情况，并且etcd是直接把key对应的value覆盖,所以也不存在mysql中读+写的幻读情况(读的时候没有，后来又有了，造成insert失败)。
2. 只有并发读， 可走内存b树。
~~~

etcd的key可不可以是随机数？

~~~c
可以，但不好， 比如TTL为5，如果服务在5秒内挂掉，重登，此时会有两个不同的key(因为是随机数), 对应相同的value
一般使用，ip + port;
~~~



跟mysql 对比分析
wal 相当于redolog 
snapshot相当于rdb， 当有其他服务加入集群时，通过快照的方式同步数据
boltdb 相当于存储引擎 ，如innodb等
term：leader任期时间， leader切换时term加一，全局单调递增，64bits

revision：etcd键空间版本号，key发生变更(增删改)，则revison加一，全局单调递增，64bits， **用来支持MVCC**的  (每一个key对应一棵B+树，每个节点存储不同的版本,每次对这个key进行修改都会记录在这棵树上) 





etcd 2 和redis比较像
都是内存数据库， 同时设置key的过期时间的方式一样，这样涉及的话，如果有大量Key都是同样的过期时间，就会大量进行相同的删除key的操作，  而 etcd 3中采取了租约的方式， 多个Key挂在一个租约上，当租约到期， 所有挂在这个租约上的key都失效，用于集群监控以及服务注册发现(回想redis中watch命令的使用，可以监控key)

etcd v2 版本存在丢弃历史版本数据的问题，仅保留最新版本的数据。但是这样做引起了一系列问题，**比如 watch 机制依赖历史版本数据实现相应功能**，因此 etcd v2 又采取了在内存中建立**滑动窗口**来维护部分历史变更数据的做法，然而在大型的业务场景下还是不足以支撑大量历史变更数据的维护。到了 etcd v3 版本，该功能得到了更新，etcd v3 支持 MVCC，可以保存一个键值对的多个历史版本。

**etcd 存储以数据的 Revision 作为 key**，键、值、创建时的版本号、最后修改的版本号等作为 value 保存到数据库。etcd 对于每一个键值对都维护了一个全局的 Revision 版本号，键值对的每一次变化都会被记录。



mysql中MVCC 和 etcd中MVCC对比：

~~~
mysql中通过undolog来实现回滚和MVCC，事务一旦提交， undolog就没啥用了(mysql在事务未提交时才会用到mvcc机制，所以事务提交后基本就用不上了，而etcd会经常根据以往的revision去查询某条记录)， 而etcd中涉及到对以前版本的查找(多，而且需要通过key(revision)去找到指定的版本)，所以使用B+树提高查找效率

需要注意一点，对于mysql事务未提交之前，undolog也会发生变化，而etcd事务未提交之前，b+树是不会发生变化的，  这里说的undolog和b+树就是MVCC的关键
~~~



mysql和etcd中为加速索引采用的方法：

~~~c++
mysql通过在内存中维护自适应hash来加快索引的查找， etcd在内存中维护了一棵B树，B树的key为key，value为revision
~~~


