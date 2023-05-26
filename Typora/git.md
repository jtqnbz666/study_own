在服务器上创建一个git仓库

~~~
mkdir github.git
cd github.git
git init --bare
~~~

机器首次提交的时候要自报家名

~~~
git config --global user.name "jt"
git config --global user.email "454193896@qq.com"

使用 git confil -l 查看是否设置成功
~~~

如果仓库含有子模块，需要把子模块一起拉下来的话

~~~
git clone --recurse-submodules
git pull --recurse-submodules //把子模块一起拉下来
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

![1676257212630](../pic/1676257212630.png)

//以下内容是以这幅图进行介绍的， 如果把下边的 "文件" 换成 "." 则表示所有文件

git checkout 文件， 会用暂存器的文件来替换工作区的这个文件的内容，比如工作区这个文件的内容是6666， 而暂存区是666，则会将工作区内容更改为666

git checkout HEAD 文件名， 会用本地仓库的这个文件来替换**工作区和暂存区**的这个文件的内容， 把文件名换为 点 表示所有文件

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

### 当在其他地方提交push， 发生冲突

直接git pull 可能覆盖代码

git stash -> git pull -> git stash pop -> git add. -> git commit -m "" -> git push

小提示git stash drop删除暂存内容

如果过程中出现了no branch问题， 说明你在匿名分支下，解决完冲突后， git rebase --continue,  再次重新add. commit push

**可以使用 git push -f  表示强制以当前代码覆盖之前提交的**

### 解决git冲突：
**方式一**：当两个地方同时发生改变， A 先提交， B再git pull 就会报错， 此时B应该git stash 一下缓存下来自己的更改， 
再git pull 拉取下来， 此时可以看到A提交的最新情况，但看不到B的更改，此时B执行git stash pop，就可以看到二者的冲突了。
其实这上边的git stash操作可以替换为 (git add 文件 + git commit 文件)， 那么执行git pull的时候就会发生冲突，此时可以同时
看到A和B的更改，并且有相应的标注。
**方式二**:  
当A提交代码， B执行git add. --> git commit -m "jt" --> git push(报错)
git pull --rebase	//此时会进入一个临时的分支， 若不加 --rebase 默认是 merge方式
//这个过程就可以看到拉下来的最新代码以及自己的冲突代码， 自己去做一些调整
git add .	//上边调整好后，执行git add .
git rebase --continue	//因为刚才是在一个临时分支解决冲突，现在回归到我们原来的分支
// 到这里为止， git log查看一下commit记录， 可以发现，包含了远程分支的最新commit，以及自己刚才的那条commit记录
// 已经解决冲突了，此时git push 推送最新内容即可
git push orign/master


上述的两种方式，git pull 和 git pull --rebase的区别在于
git pull 会将远程分支的最新commit 与自己刚才的commit记录合并，即只能看到自己刚才的最新commit记录
而git pull --rebase 会将远程分支的最新commit 与自己刚才的commit记录 都保留，即能看到两条commit记录



git pull = git fetch + git merge
git pull -rebase = git fetch + git rebase

git pull 有这么几种配置
git config pull.rebase false  // 这是默认的，也就是merge方式
git config pull.rebase true  //这表明当前使用rebase方式

git fetch: 这将更新git remote中所有远程仓库所包含分支的最新commit -id,将其记录到.git/FETCH_HEAD文件中

git pull: 首先基于本地的FETCH_HEAD记录，对比本地的FETCH_HEAD记录与远程仓库的版本号，然后git fetch 获得当前指向的远程分支的后续版本的数据，
然后再利用git merge将其与本地的当前分支合并
git pull 后不加参数的时候，和git push一样， 默认就是git pull origin 当前分支名， 如果远程仓库没有跟本地当前分支名一样的分支就肯定会报错。
比如再本地master分支执行git pull 的时候， 其实就是执行git pull origin master


将远程的origin仓库的xx分支合并到本地的yy分支有两种方式
1. 传统做法
git fetch origin 目标分支名字 // 会将远程的这个目标分支的最新commit -id记录到 ./git/FETCH_HEAD这个文件， 可以查看，只是一条commit -id记录
git checkout 要被合并的分支名 //切换到要被合并的分支
git merge FETCH_HEAD //将目标分支的最新commit记录合并到当前分支
举例：将远程origin仓库的xx分支合并到本地的yy分支
git fetch origin xx
git checkout yy
git merge FETCH_HEAD  //直接就是FECTCH_HEAD

2.使用git pull (因为它默认是合并了git fetch + git merge)
git checkout yy
git pull origin xx

3.第三种思路是把远程的xx分支拉下来， 即合并本地的xx 和 yy分支
git checkout xx
git pull  //拉取xx 到本地
git checkout yy //切换到yy
git merge xx // 合并本地的xx 到 yy分支， 可以加上 --no-ff参数，即git merge --no-ff xx



### 有时候出现bug，git pull

~~~
显示最新，却不是最新， 比如你打开typora，然后git pull， 关闭typora的时候一定要点不保存，不然出bug， 这时候
git reset --hard HEAD^ 
再git pull
~~~



**合并分支**

~~~
比如此时只有main分支，main分支有一个test文件
git branch test
git checkout test
git add .
git commit -m ""
git push origin test
此时test和main分支下都有一个test文件

情况一：此时test和main分支的文件内容是一样的，如果在test分支下对test文件进行修改(不提交)，那么切换到main分支的时候是可以看到这个修改的，如果单独在test分支提交了test文件的修改，那么再次切换到main分支是看不到修改后的test文件的
情况二：情况一结束后test分支已经领先了main分支的内容，此时如果修改了test分支内容，如果未提交或者stash，是无法切换到main分支的， 此时可以在test分支先提交修改，然后切换到main分支(只能看到未修改的内容)，然后执行git merge test，将test分支的内容合并到main分支
情况三：继情况二结束后，main和test分支内容是一致的，如果此时在main分支新增一个文件xx并且push上去，再去test分支修改一下原来已经有的某个文件并push上去，切换到main分支执行 git merge test命令，此时是可以把test分支做的修改合并到main分支的，并且不会把刚才在main分支新增的文件xx同步到test分支去
情况四：继情况三结束后，main分支比test分支多了一个文件xx，此时如果在test分支对某个文件进行修改但是未提交或者暂存，则无法切换到main分支去。 所以必须要在test分支下git push， 如果是多个协作者在test分支下进行操作， 那么自然需要先处理好test分支下的冲突，并且在test分支下完成提交(我平时在公司和家就可以理解为多协作者解决test分支的冲突)，再切换到main分支下进行合并。
~~~

