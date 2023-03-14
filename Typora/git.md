在服务器上创建一个git仓库

~~~
mkdir github.git
cd github.git
git init --bare
~~~





svn: 集中式

git：分布式

git init --bare 创建一个裸仓库，不加bare 就是非裸

密钥配置：ssh-keygen -t rsa 一路回车， 在~/.ssh下id_rsa.pub是公钥文件，id_rsa是私钥文件， 把自己的公钥拷贝到远端的~/.ssh 中的authorized_keys文件(没有就创建)

watch -n 0.5 tree  监视tree命令，每0.5秒刷新一下

分四个区， 工作区， 暂存区， 本地仓库， 远程仓库

**常用命令**：

~~~
git add , git commit -m "test" , git push

git status 看到文件状态

git branch 查看当前分支，git branch -a 查看所有分支， git branch test 创建一个test分支， git branch -d test 删除test分支

比如现在我们git branch test

需要git checkout test   表示切换到test分支

但由于现在远程仓库还没有test分支，所以需要把这个test分支同步到远程上

git push original test 即可在远程建立test分支
~~~



**实用命令：**

![1676257212630](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\1676257212630.png)

//以下内容是以这幅图进行介绍的， 如果把下边的 "文件" 换成 "." 则表示所有文件

git checkout 文件， 会用暂存器的文件来替换工作区的这个文件的内容，比如工作区这个文件的内容是6666， 而暂存区是666，则会将工作区内容更改为666

git checkout HEAD 文件， 会用本地仓库的这个文件来替换**工作区和暂存区**的这个文件的内容

git reset HEAD 文件， 表示把本地仓库的文件同步到暂存区，但**不影响工作区内容**。
**不存在 git reset --hard HEAD 文件** ，这种方式是错误的。

git log  查看提交情况， 使用 git reset --hard " hash值" 去到某一个提交的地方，这个命令可以去到以前，也可以回到未来，只需要记住具体哪次提交的hash值。 也可以通过**git reset --hard HEAD^1** , 这个1表示往前回退一个版本， 表示把本地仓库中的那个提交情况同步到**暂存器以及工作区**

当遇到同时提交冲突时，使用git pull 拉取冲突情况，这时候需要重新git add -> git commit -m " " - > git push 重新提交

**情景分析：**

~~~
1.当前文件内容为666  -> git add .  -> 修改文件内容为6666 -> git commit -m "test" -> git push     那么提交到远端的内容是666 而不是 6666
可以看出git commit只是将缓冲区里已经add的部分同步到本地仓库中去，不包括工作区里
存放的内容


2.当前文件内容为666 -> git add. -> git commit -m "test" -> 修改文件内容为"6666" -> git add . -> git push   那么提交到远端的内容是666, 而不是6666
可以看出git push只是将本地版本库里已经commit的部分同步到服务器上去，不包括暂存区里
存放的内容

~~~

