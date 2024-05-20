git http推， 全局代理

13.添加远程仓库到本地仓库

~~~shell
git remote add origin <远程git仓库地址>
# 注意: 没有git信息之前就需要执行这个指定远程仓库，git信息可以在.git/config中看到
~~~

12.git pull

~~~shell
git pull = git fetch + git merge.   # 默认
git pull -rebase = git fetch + git rebase

首先基于本地的FETCH_HEAD记录，对比本地的FETCH_HEAD记录与远程仓库的版本号，然后git fetch 获得当前指向的远程分支的后续版本的数据，
然后再利用git merge将其与本地的当前分支合并
git pull 后不加参数的时候，和git push一样， 默认就是git pull origin 当前分支名， 如果远程仓库没有跟本地当前分支名一样的分支就肯定会报错。
比如再本地master分支执行git pull 的时候， 其实就是执行git pull origin master
~~~

11.git fetch

~~~
这将更新git remote中所有远程仓库所包含分支的最新commit -id,将其记录到.git/FETCH_HEAD文件中
~~~

10.如果仓库含有子模块，需要把子模块一起拉下来的话

~~~
git clone --recurse-submodules
git pull --recurse-submodules //把子模块一起拉下来

如果子模块中有改动，拉的时候失败了，先去子模块git stash 再 git stash drop 再重新拉
~~~

8.在服务器上创建一个git仓库

~~~
mkdir github.git
cd github.git
git init --bare  创建一个裸仓库，不加bare 就是非裸
~~~

7.小知识点

~~~shell
2.有时候发现git reset --hard 无法删除本地工作区新增的文件， 两种方法，采用git clean --df 或者 先git add . 添加到暂存区，再 git reset --hard 一次，因为git reset 删除的是已跟踪的文件，将已commit的回退。 git clean 删除的是未跟踪的文件
1.强推(git push origin develop --force)和强拉(git reset --hard origin/develop)

~~~

7.rebase和merge遇到冲突时的乱码理解 (以A，B分支，将B分支内容合并到A上为例)

~~~
merge: git checkout A, git merge B, 如遇冲突，HEAD表示A分支的内容，====下边表示B的内容

rebase: git checkoutA, git rebase B, 如遇冲突，HEAD表示B分支的内容
~~~

6.常用配置

~~~shell
# 设置git pull的模式
git config pull.rebase false  // 这是默认的，也就是merge方式
git config pull.rebase true  //这表明当前使用rebase方式

# 设置提交用户信息
git config --global user.name "jt"
git config --global user.email "454193896@qq.com"
使用 git confil -l 查看是否设置成功
~~~

5.rebase技巧

~~~shell
# 进入一个交互界面
git rebase -i 分支名 
# 伪变基，目的是整理单个分支，合并提交
git rebase -i HEAD~6 # 6表示最近6个提交
~~~

4.更改用户名/邮箱地址

```sh
git config --global user.name "Your Name"
git config --global user.email "your_email@example.com"
```

3.修改文件名大小写问题

```shell
git mv --force filename FileName
```

2.切分支流程(从某个节点创建新分支)

~~~shell
1.git branch hotfix/20240103
2.git checkout hotfix/20240103  # 可以加-b参数省去上一步
3.git push origin hotfix/20240103
~~~

1. merge和rebase学习

   rebase和merge的区别是前者会先隐藏当前分支公共节点之后的commit内容，暂时只能看到对方最新的commit内容，然后去解决冲突，逐渐就能看到当前分支公共节点之后的commit内容，然后git add . 再 git rebase --continue(可以用git rebase --skip跳过冲突的解决)， 而merge是将对方最新push的commit内容和自己本地的最新commit内容合并在一起， 解决完冲突后先执行git add . 再 git merge --continue，其实我感觉merge更好用，因为rebase用时候需要解决多个阶段有点复杂。 它们都可以用git xx --abort中断。

![image-20240302112020209](../pic/image-20240302112020209.png)

rebase操作: 找到公共节点3，然后把5,7嫁接到4,6后边， 但这样之后5',7'虽然内容和之前一样，但是commit号会是一个新的

~~~shell
比如B要合并到A上: git checkout A --> git rebase B --> 处理冲突 --> 直接强推 如果公司不支持强推，1.那就得git pull 再处理下B分支有而A分支没有的conmmit内容，2.采用下方拓展方法


# 拓展
配合merge实现rebase的效果
比如将B合并到A上, git checkout B --> git rebase A --> 处理冲突 --> git checkout A --> git merge B
~~~

![image-20240302112149170](../pic/image-20240302112149170.png)

merge操作:找到公共节点3,然后配合两个分支的最新一次提交节点(一共三个节点)组成一次新的提交8， 这样的好处是能保留原有的commit号以及提交顺序，问题就是会显得提交 结构复杂。



**merge案例分析**

1.两个分支A和B， 如果现在A和B都有一个1.txt 和 2.txt文件，此时在A上增加3.txt，修改1.txt， 而在B上删除2.txt，修改1.txt， 此时在A分支执行git merge B 就会出现A上的3.txt不变，2.txt会被删除，1.txt出现冲突需要解决。（如果A上的2.txt发生改变，合并B过来的时候A就不会删除，而需要手动去处理冲突）



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

![1676257212630](../pic/1676257212630.png)

//以下内容是以这幅图进行介绍的， 如果把下边的 "文件" 换成 "." 则表示所有文件

git checkout 文件， 会用暂存器的文件来替换工作区的这个文件的内容，比如工作区这个文件的内容是6666， 而暂存区是666，则会将工作区内容更改为666

git checkout HEAD 文件名， 会用本地仓库的这个文件来替换**工作区和暂存区**的这个文件的内容， 把文件名换为 点 表示所有文件

有时候上边两部执行了也没有用， 无法切换分支，那么执行一下git fetch即可

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



### 有时候出现bug，git pull

~~~
显示最新，却不是最新， 比如你打开typora，然后git pull， 关闭typora的时候一定要点不保存，不然出bug， 这时候
git reset --hard HEAD^ 
再git pull
~~~

