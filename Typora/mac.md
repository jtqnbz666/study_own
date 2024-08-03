mac的termius乱码

~~~
~/.zshrc加上， 然后source ~/.zshrc
export LANG=en_US.UTF-8
~~~



光标移动， w下一个单词， b上一个单词

NG移动到第N行

shift+# 光标下一个匹配的单词， shift+* 光标上一个匹配的

goland中：command+shift+f 全局搜索，+o 搜索文件

control+command + f 应用全屏

/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)" 下载brew

有时候brew intsall 无法安装一些东西， 去 /opt/homebrew/var/homebrew/linked看是否已经有符号链接，如果有就删了再重新安装

https://macwk.com.cn/soft/typora可以下载不少需要付费的软件
