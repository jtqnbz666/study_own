MySQL和Redis基本操作

~~~
1. mysql重建索引，两个500w，一个12w基数的联合索引
~~~



**数据备份**: 线上的mysql数据备份到新实例上

~~~
可以查看帮助文档搜索 "恢复到其他已有实例" 

1. 搜索云数据库RDS-->在实例列表中找到线上mysql实例并点进去-->左侧备份恢复-->根据需要选择一个时间节点并且点击右边的恢复--> 选择serverless一直点确定然后付款之后等待5分钟左右就完成了新实例的创建(210GB数据量)-->直接去实例列表拿到这个实例的地址，把staging的svc地址换成这个新地址(kubectl edit svc staging-mysql -n staging,修改externalName)-->重启pod-->删除原有redis缓存(flushdb)
~~~

导出线上redis中HASH类型

~~~shell
# 导出
redis-cli -h $addr hgetall BeastBoardCoreData_3_24 > beastBoardHash.txt

# 导入
key="BeastBoardCoreData_3_24"
lines=$(wc -l < beastBoardHash.txt)
for i in $(seq 1 2 $lines)
do
    uid=$(sed -n ${i}p beastBoardHash.txt)
  	jsonStr=$(sed -n $((i+1))p beastBoardHash.txt)
    docker exec -i data-redis redis-cli HSET "$key" "$uid" "$jsonStr"
done
~~~

导出线上redis中ZSET类型

~~~shell
# 背景：zset大小5w玩家，执行耗时很快(立马就执行完了)
redis-cli -h r-bp1lb2qpinvy0ze8ae.redis.rds.aliyuncs.com 
HGETALL BeastBoardCoreData_3_24 > hash_export.txt

# 导出 （排行榜只拿前200就够了）
redis-cli -h r-bp1lb2qpinvy0ze8ae.redis.rds.aliyuncs.com zrevrange RankingBoard_1 0 200 withscores > rankingBoard.txt

# 本地导入,建一个sh脚本 (细节: 文本中member和score的顺序和需要执行的顺序是反的)
lines=$(wc -l < rankingBoard.txt)
# 按行读取文件内容，并交替处理成员和分数
for i in $(seq 1 2 $lines)
do
    member=$(sed -n ${i}p rankingBoard.txt)
    score=$(sed -n $((i+1))p rankingBoard.txt)
    docker exec -i data-redis redis-cli ZADD RankingBoard_1 $score $member
done
~~~

导出线上数据的.sql文件

~~~shell
# 背景:11万用户量， 去掉较大的战绩表，执行耗时1分钟左右，导出的.sql有2.5G，导入到本地花了大于10分钟时间

# --single-transaction 不会对数据库全局锁定，减少mysql的影响
# staging
mysqldump -h rm-bp1xx0ybar41opwch.rwlb.rds.aliyuncs.com -u root -phoxi0328JING --single-transaction project328 > project328.sql

# 忽略太大的表
mysqldump -h rm-bp1xx0ybar41opwch.rwlb.rds.aliyuncs.com -u root -phoxi0328JING --single-transaction project328 --ignore-table=project328.user_game_history_record_data --ignore-table=project328.user_game_history_records --ignore-table=project328.mails > project328.sql

# online
mysqldump -h rm-bp1c4m1w22oq77q82.rwlb.rds.aliyuncs.com -u hoxi_admin -phoxi0328JING --single-transaction project328 > online.sql

mysqldump -h rm-bp1c4m1w22oq77q82.rwlb.rds.aliyuncs.com -u hoxi_admin  -phoxi0328JING --single-transaction project328 --ignore-table=project328.user_game_history_record_data --ignore-table=project328.user_game_history_records --ignore-table=project328.mails > online.sql
~~~

staging的工具在/home/staging/tools/build-server/src
online的工具在/data/tools/build-server/src

~~~shell
重启staging的gm:
bash deploy-gm-tool-agent.sh develop

重启战斗服:
先clear-old-bsm.sh 再 deploy-bsm-new.sh
~~~

