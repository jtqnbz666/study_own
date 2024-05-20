1.[php7.4安装](https://www.gingerdoc.com/tutorials/how-to-install-php-7-4-and-set-up-a-local-development-environment-on-ubuntu-20-04)

2.msg_server里面的libslog.so 需要拷贝一份到 /usr/lib

3.三个库文件要放到/usr/lib

~~~
 server/src/slog/build/libslog.so
 server/src/base/build/libbase.a
 libhiredis.a
~~~

4.go-mysql-transfer的用法放到server/src/tools中了