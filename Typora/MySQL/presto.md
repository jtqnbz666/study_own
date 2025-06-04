数据类型

~~~sql
ARRAY(JSON):  [{"room": 1001, "games": 1}, {"room": 1002, "games": 3}]
ARRAY(int): [1, 2 ,3]
ARRAY(row(roomid int, games int)): [[1001, 1], [1002, 2]]
~~~

unnest

```sql
# 复合类型
SELECT room_info, t.roomid, t.detail.games
FROM (
  # 用子查询造一个数据
    SELECT 
    ARRAY[1, 2] AS testnum,  
  	cast(
    ARRAY[
        ROW(1001, ROW(10, 2)),	# 必须是row，超过一个int就是复合类型了
        ROW(1002, ROW(20, 2))
    ]  as array<row(roomid int, detail row(games int, players int))>
    ) AS room_info
) AS subquery
CROSS JOIN UNNEST(testnum, room_info) AS t(num, roomid, detail) # 第一个数组一个字段，第二个数组2个字段，testnum和room_info的位置换了则后边的顺序也要换
```

filter（返回数组）

```mysql
SELECT filter(ARRAY[1, 2, 3, 4], x -> x > 2) AS filtered_array;
# [3, 4]

# 进阶
SELECT filter(cast(ARRAY[row(1001, 2)] as array(row(roomid int, games int))), x -> x.roomid > 2) AS filtered_array
```

row（结构体）

``` sql
select * From (select row(1, 'example') AS my_row) 
# [1,"example"]
```

transform（返回数组）

```mysql
SELECT transform(ARRAY[1, 2, 3], x -> x * 2) AS doubled_array
# [2, 4, 6]

string转为array<int>
SELECT transform(split('1,2,3', ','), x -> CAST(x AS integer)) AS int_array;
```

解析json对象

```mysql
CAST(json_extract_scalar(extrainfo, '$.matchpoint') AS integer) 
# integer和int没区别，如果是多层字典嵌套的比如{"level1": {"level2": {"matchpoint": "123"}}}可以写成'$.level1.level2.matchpoint'
```

中位数

``` sql
approx_percentile(total_game_count, 0.5) # 第二个值在0-1之间
```

小知识

~~~c++
20.虽然array<row(roomid int, detail row(games int, players int))的数据内容为[[1001, [1, 2]]]这种格式的，但构造的时候必须用array[row(1001, row(1, 2))]
19.cross join相当于完全的笛卡尔乘积，不依赖条件，cross join unset只是展开后与展开的那行联合而不是整个表，from table cross join unnest 和 from table, unnest功能上等价
https://bi.duole.com/queries/8977/source
17.approx_percentile求中位数，approx_percentile(total_game_count, 0.5)
16.分房间 and room_id = {{房间}}， 选择bigdata-room-list-shengji， 分玩法：and {{玩法}}，选择bigdata-room-list-shengji
15.group by 和 having中都不能用select中的别名，比如group by不能用select中的case分组后的别名，having不能直接用聚合函数结果别名，需要重新写一份。因为presto中select部分在having后执行
14.group by中的字段不能用select中的别名，格式要和select中的一样，比如 select regexp_extract(__content__, 'userid=(\\d+)', 1) AS userid from testtable group by regexp_extract(__content__, 'userid=(\\d+)', 1) 
13.count_if(condition) 和 count(if(condition, 1, null))一样，condition可以是条件，也可以是true/false，不能是数字
11.日期差值, date_str_diff('2025-01-01', '2025-01-02')，结果为1，date_str_add('{{date.end}}', 7)，增加7天。
10.ARRAY[10, 12, 13]放到一个集合中
9.timestamp类型(如2015-04-21 11:09:11)判断是否大于某一天，where  cast(createtime AS varchar) > '2000-12-07'，varchar在比较时是从左到右逐位比较的。
8.时间戳格式化：sql中的datetime类型通过presto查询时比如_game.shengji.benkorecord，会被转为timestamp(0)类型，比如sql中用like '%2025-02-06%'，在presto中要用DATE_FORMAT(createtime, '%Y-%m-%d') = '2025-02-06'， 也可以用 cast(createtime AS varchar) like '%2025-02-06%'
7.cardinality求数组的长度
6.transform和filter配合使用， transform(filter(room_detail.room_info, x -> x.room_id = 11101), x ->cast(row(x.room_id, x.game_count) as row(room_id int, games int))) as room_info， 使用unnest展开room_info数组，unnest(room_info) as t(room_id, games)，因为每个元素都有两个值，所以是t(room_id, games)，如果只有一个值就只写一个，unnest还支持展开多个数组，但要求长度相同，比如unnest(useridlist, playcards) as t(user_id, play_cards)，useridlist和playcards的元素一一对应的展开。
5.regexp_like比like效率高，regexp_like(lower(channel), 'wechat|mini' 等效于 lower(channel) like '%wechat%' and lower(channel) like '%mini%'
4.RANK() OVER (ORDER BY COALESCE(weekend_login_weeks, 0)) - 1 AS rank，COALESCE 用于把weekend_login_weeks为NULL的情况转为0，rank的值从1开始，如果玩家的分数一样，名次会一样，比如1、2、2、4，这里减1的目的是得到的rank值表示自己超过了多少人。
3.regexp_extract用于匹配并且提取内容，regexp_like验证是否匹配返回bool
regexp_extract(evtname, '(【分享出去】.*|【分享成功】.*)') AS evtname 用于提取evtname信息，比如evtname为'test【分享出去】分享得额外救济金'，就可以提取出来'【分享出去】分享得额外救济金'，regexp_extract(__content__, 'userid=(\\d+)', 1)表示提取第一个捕获组，也就是()内的值
2. ROW_NUMBER() OVER (PARTITION BY date, user_id ORDER BY id) AS rn 分区筛选第一条数据，用where rn = 1过滤
1. json_extract:返回 JSON 对象或数组。json_extract_scalar:返回标量值（如字符串、数字），如：COALESCE(CAST(json_extract_scalar(extrainfo, '$.throwntentimes') AS INTEGER), 0) ，其中 COALESCE会返回第一个非NULL值(0也是非NULL)，CAST进行类型转换。
                                  
语法说明：
7.with trialuser as () 多表查询
6.timestamp是包含时分秒的，date只有年月日，只要是时间字符串，前边必须加上date或者timestamp强转他的类型，不然会认为是个字符串。
5.引号中两个连续单引号可以表示一个单引号，比如where name = 'It''s a str'， presto的where中必须用单引号，别名中必须用双引号，比如select user_id as "alias" from 
4.presto支持跨库查询，比如从MySQL1.table拿部分数据和MySQL2.table进行联表。
3.join时大表放在左边，presto默认分割左表到多个worker，然后把右表完整放到每一个worker中。    
2.presto跨库查询，比如SELECT count(id) FROM table_1 WHERE condition=1;(1)先向Mysql发起SELECT id FROM table_1 WHERE condition=1;(2)对结果进行count计算，(3)返回结果。                      
1.bi中的python可以用custom_print(sql1)打印sql
shengji-game和shengji-log分别是线上的业务库和日志库
presto存储数据：
上报到fluent的: shengji.tablename
上到到日志库的每日数据: mysql_shengji.login
mysql_shengji_log.shengji.表名  # 日志库
mysql_shengji_game.shengji.表名 # 业务库
线上服务器的日志记录: logs.shengji_tcp为连接服日志，logs.shengji_game为游戏服，logs.shengji_internal为C++其他服务

# 常用
mysql_shengji.buy看真实付费充值记录，带有coin的表示多乐币的充值
mysql_shengji.coin_exchange看多乐币的交易记录
mysql_shengji.debt看游戏豆的变化原因

# web服线上日志查询
select * from logs.shengji_web_uwsgi where date = '2024-12-07' and hour = '11' and __content__ like '%260402120%' and __filename__ = '/var/log/uwsgi/shengji.log'
                                  
# web脚本日志
select __content__ from logs.shengji_internal_02 where date = '2025-01-20' and  hour = '20' and __filename__ = '/var/www/apps/shengji/log/monitor/log_monitor.log'  and __timestamp__ > 1737377120000000 and __timestamp__ < 1737377130000000 
                                  
参考文档：https://doc.weixin.qq.com/doc/w3_ARgAcga2AGs0ZD3atd9SS6Gxp1V0j?scode=AAUA-geVABEz1XJIUhAasACgagACk
~~~

bi操作（python语法）

~~~python
3.如果引号中有%，需要用%%来代表一个%
2.打印：custom_print(result)
1.获取结果：
res = execute_query('shengji-presto',sql)
for r in res['rows']:
  tempinfo = {}
  tempinfo['游戏服务日志'] = r['__content__']
  result['rows'].append(tempinfo)
如果不需要重命名，可以直接用res的结果，比如
 result['columns'] = res['columns']
 result['rows'] += res['rows']
# case:
import json
import datetime
import sys
import datetime
from datetime import timedelta
import base64
result = {
    'columns':[
            {'name':'平台订单信息'},
            {'name':'游戏订单信息'},
        ],
    'rows':[],
}

result['rows'].append({"游戏订单信息":1, "平台订单信息":2 })
result['rows'].append({"游戏订单信息":3, "平台订单信息":100 })
custom_print(result)
# 说明
1.当前用的是python2，不支持f''格式化操作
# 参考bi
https://bi.duole.com/queries/8380/source?p_%E5%BC%80%E5%A7%8B%E6%97%A5%E6%9C%9F=2025-01-14&p_%E7%BB%93%E6%9D%9F%E6%97%A5%E6%9C%9F=2025-01-15&p_%E9%80%89%E9%A1%B9=%E6%B1%87%E6%80%BB#9383

https://bi.duole.com/queries/8748/source?p_date.start=2025-02-01&p_date.end=2025-02-26#9785
~~~

