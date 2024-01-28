

3.主要关键字

~~~yaml
source: 数据来源
~~~



2.主要日志文件

~~~yaml
data.b60e0b0c6a3cf4222a5d6ba8234176fe3.log (理解为缓存区)  data.b60e0b0c6a3cf4222a5d6ba8234176fe3.log.meta(记录缓存区文件信息)
data.2024010321.log (存储日志数据)
一开始的数据会写到比如"data.b60e0b0c6a3cf4222a5d6ba8234176fe3.log"缓冲区。以一个小时为例，一个小时结束后就会把缓存刷到data.2024010321.log中去
#1.比如21点，不会生成data.2024010321.log，22点的时候才会生成data.2024010321.log
#2.再比如以天为时间切片，那不一定会在第二天开始的时候才刷第一天的日志，因为fluent也有一定默认阈值，超过就会主动刷一下
#3. #time_slice_wait 比如配置为1m，意思是说，21:59准备跨越到22:00的时候，还会等待1m，确保21点的所有日志都到达了，然后再刷入data.2024010321.log中
~~~

**yaml配置**

~~~yaml
# 格式: /fluentd/log/服务名(UserDataService)/每小时的日志(比如data.2024010314.log)
# /fluentd/log/ 一般是容器挂载的日志路径
<source>
  @type  forward
  @id    input1
  @label @mainstream
  port   24224
</source>

<label @mainstream>
  <match **UserDataService**>
    @type file
    @id   output1
    path         /fluentd/log/UserDataService/data.*.log
    symlink_path /fluentd/log/UserDataService/data.log
    append       true
    time_slice_format %Y%m%d%H
    time_slice_wait   1s
    time_format       %Y%m%dT%H%M%S%z
  </match>
  # path里面的*表示接受任意格式的日志文件，然后time_slice_format标明了日志格式，最后就会生成一个比如/fluentd/log/UserDataService/data.2024010321.log的文件
~~~

官网链接:https://docs.fluentd.org/configuration/config-file-yaml
