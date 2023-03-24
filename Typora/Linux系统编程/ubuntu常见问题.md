

电脑装不上powershell  就换成 windows terminal** 

## 远程连接密钥

机器1上(在哪个用户下进行，就对哪个用户产生密钥)：ssh-keygen ,一直按空格，此时会发现 cd ~/.ssh 多了两个文件id_rsa和id_rsa.pub。

在机器2上输入cd ~/， 在此目录下创建文件 .ssh文件夹，进入该文件夹，创建一个文件名为authorized_keys的文件，将刚才的id_rsa.pub的值拷贝进来即可。

此时可以远程copy免密码，但ssh还需要输入密码

在机器2上 ：vim /etc/ssh/sshd_config 

#PasswordAuthentication yes //关闭密码连接
passwordAuthentication yes  //允许密钥

退出保存，重启服务，systemctl  restart  sshd





```typescript
git config --global user.name "你的用户名" 
git config --global user.email "你的邮箱"
```



##  没有ip地址

sudo dhclient ens33（网卡名称）

  查看网络配置[vim](https://so.csdn.net/so/search?q=vim&spm=1001.2101.3001.7020) /etc/network/interfaces，发现只有lo的配置，没有对网卡进行配置设置动态IP地址：

对文件添加如下内容

~~~c
auto ens33
 iface ens33 inet dhcp
 重启网络/etc/init.d/networking restart
~~~







**问题：** 如果每次重启都要重新手动分配。
 **原因：** 网卡未加入托管。

```bash
#查看托管状态
nmcli n

#显示 disabled 则为本文遇到的问题，如果是 enabled 则可以不用往下看了
#开启托管
nmcli n on
```

## 克隆机ip相同

![img](https://img-blog.csdnimg.cn/b16e12687c944ea08f6dc43bba45405e.png)

![img](https://img-blog.csdnimg.cn/a56ac73828244ef3b669d4e166c87fda.png)

使用cd  /etc/netplan进入netplan文件夹中，ls   命令可以看到netplan文件夹中只有01-network-manager-all.yaml这一个文件

将内容设置为

~~~c
network:
  version: 2
  renderer: NetworkManager
  ethernets:
    enp0s3://网卡名称，Ubuntu是 ens33
      dhcp4: false //自动分配ip关闭
      addresses: [192.168.13.128/24]
      optional: true
      gateway4: 192.168.13.2
      nameservers:
        addresses: [114.114.114.114] //DNS这个好像不要也行

~~~

dhcp4为false，代表着动态ip关闭，为静态ip

gateway是网关：这在虚拟网络配置器中能查到

dhcp4为false，代表着动态ip关闭，为静态ip

gateway是网关：这在虚拟网络配置器中能查到、





## /mnt/hsfs/下不显示共享文件夹

root用户下执行：vmhgfs-fuse .host:/ /mnt/hgfs

## 修改文件描述符数量

我们先打开vim /etc/security/limits.conf文件，在下面加上

 * soft    nofile         1048576   



**soft    nofile         1048576                                                                        前边这个*  * 代表的是所有用户 **

如果上边这种方法不行的话,直接ulimia -a 
**显示出来的结果，比如文件描述符是-n  ,即可使用ulimit -n 数字来修改对应的值**（这种修改方式重启后就没有了）

另一个内核级的fd限制，Linux系统下，所有进程允许打开的最大fd数量，我们把1048576加入系统配置文件/etc/sysctl.conf里面然后运行sysctl -p使其生效。

![](C:\Users\ASUS\Pictures\博客图片\20201203162557980.png) 

![](C:\Users\ASUS\Pictures\博客图片\20201203174438692.png)

最后一行参数设置的是一个机器最大的connect 数量，默认是65535，用于测试百万并发中。

光添加还不行，还需要运行sudo modprobe ip_conntrack来对这个值进行加载，一般Linux内核默认是不会给用户这个模块权限，否则直接运行sysctl -p会报错:



## vim优化

先vim ~/.vimrc, 输入以下内容

~~~c
“去掉讨厌的有关vi一致性模式，避免以前版本的一些bug和局限
set nocompatible
“显示行号
set number
“检测文件的类型
filetype on
“记录历史的行数
set history=1000
“背景使用黑色
set background=dark
“语法高亮度显示
syntax on
“下面两行在进行编写代码时，在格式对起上很有用；
“第一行，vim使用自动对起，也就是把当前行的对起格式应用到下一行；
“第二行，依据上面的对起格式，智能的选择对起方式，对于类似C语言编
“写上很有用
set autoindent
set smartindent
“第一行设置tab键为4个空格，第二行设置当行之间交错时使用4个空格
set tabstop=4
set shiftwidth=4
“设置匹配模式，类似当输入一个左括号时会匹配相应的那个右括号
set showmatch
“去除vim的GUI版本中的toolbar
set guioptions=T
“当vim进行编辑时，如果命令错误，会发出一个响声，该设置去掉响声
set vb t_vb=
“在编辑过程中，在右下角显示光标位置的状态行
set ruler
“默认情况下，寻找匹配是高亮度显示的，该设置关闭高亮显示
set nohls
“查询时非常方便，如要查找book单词，当输入到/b时，会自动找到第一
“个b开头的单词，当输入到/bo时，会自动找到第一个bo开头的单词，依
“次类推，进行查找时，使用此设置会快速找到答案，当你找要匹配的单词
“时，别忘记回车
set incsearch
“修改一个文件后，自动进行备份，备份的文件名为原文件名加“~“后缀



如果去除注释后，一个完整的.vimrc配置信息如下所示：
set nocompatible
set number
filetype on
set history=1000
set background=dark
syntax on
set autoindent
set smartindent
set tabstop=4
set shiftwidth=4
set showmatch
set guioptions-=T
set vb t_vb=
set noeb vb t_vb=
set ruler
set nohls
set incsearch
set expandtab
set smarttab
set completeopt=menu,preview,longest

set noundofile 
set nobackup 
set noswapfile 

set relativenumber

set cursorcolumn 
set cursorline

set cursorline " 突出显示当前行
    
" 可以在buffer的任何地方使用鼠标（类似office中在工作区双击鼠标定位）
//set mouse=a
set mouse=nv
set selection=exclusive
set selectmode=mouse,key

set paste

set nohls //查找book单词 ,输入/b

set noexpandtab  //通过这行可以解决makefile table的问题
//在sublinetext中的view找到indentation取消对它的勾选也可以
set fileformats=unix,dos



//^M问题 ，  :%s/\r//gc
~~~

再执行source ~/.vimrc

## wsl重启

```
以管理员身份运行
#停止LxssManager服务
net stop LxssManager  
 
#启动LxssManager服务
net start LxssManager  
```

WSL2与vmware切换

**bcdedit /set hypervisorlaunchtype off**//关闭hyper-v执行vmware

**bcdedit /set hypervisorlaunchtype auto**//开启hyper-v执行wsl2

都需要重启机器

ERROR 2002 (HY000): Can't connect to local MySQL server through socket '/var/run/mysqld/mysqld.sock' (2) 问题

~~~c
mysql -h 127.0.0.1 -u root -p 可以正常访问

在/etc 中创建my.cnf 文件， 加入内容
[mysql]
protocol=tcp   即可
~~~



**docker一直starting** 

```csharp
net stop com.docker.service
net start com.docker.service
cd "C:\Program Files\Docker\Docker"
./DockerCli.exe -SwitchDaemon

都不行
wsl --update
```

docker启动不了服务

~~~
 docker run --name xx --entrypoint /bin/bash -dit db_proxy_server:latest
 直接进入服务内部
~~~

给wsl安装shh服务