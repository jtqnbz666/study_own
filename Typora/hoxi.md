问题： 看一下如何把奖励写到mails表的，考虑是否可以在新接口返回一个邮件信息(以前有一个通用信息，但没有包含mail， 可以新增一个)。

2.在哪里关注同事是否提交了新东西，

判断赛季更新那里， 返回一个通用的消息就行（GetRewardBackMsg）

~~~go
天梯之路

// 合法检测
	Ladderinfo := GetLadderInfo(msg.UserID)
	Explorationinfo := GetExplorationInfo(msg.UserID)
	gradeMaxRewardID := getCurMaxLevel(Explorationinfo.GotRewardID + 1)
	explorationMaxRewardID := getCurRewardIDByExp(uint64(Ladderinfo.Score))
	rewardID := MinOfTwo(gradeMaxRewardID, explorationMaxRewardID)
	if Explorationinfo.GotRewardID >= rewardID {
		logrus.WithField("role", "get-exploration-reward").Warnf("玩家已经领到此ID奖励 %v %v %v", msg.UserID, msg.RewardID, Explorationinfo.GotRewardID)
		backMsg.ResultCode = pb.RESULT_CODE_ERROR
		return backMsg
	}
	//判断奖励id是否对应
	if msg.RewardID != rewardID {
		logrus.WithField("role", "get-exploration-reward").Warnf("奖励ID与探索值不匹配 %v %v %v", msg.UserID, msg.RewardID, rewardID)
		backMsg.ResultCode = pb.RESULT_CODE_ERROR
		return backMsg
	}

	// 更新记录
	res := global.MysqlIns.Model(&model.User{}).
		Where("id = ?", msg.UserID).Select("got_exploration_reward_id").Updates(&model.User{
		GotExplorationRewardID: rewardID,
	})
	res = global.MysqlIns.Model(&model.User{}).
		Where("id = ?", msg.UserID).Select("exploration_score").Updates(&model.User{
		ExplorationScore: uint64(explorationMaxRewardID),
	})
~~~

赛季更新

~~~go
还需要做的：去user结构加一个season和一个isupdate字段

一些功能性函数
func splitRewardStrRaw(str string, level int) int {//考虑写到工具中
	allRewardStr := strings.Split(str, ",")
	for _, rewardStr := range allRewardStr {
		var splitData = strings.Split(rewardStr, "|")
		var intList [2]int
		for i := 0; i < 2; i++ {
			temp, _ := strconv.Atoi(splitData[i])
			intList[i] = temp
		}
		if int(intList[0]) == level {
			return intList[1]
		}
	}
	return 0
}

//根据当前赛季得到具体的奖励id
func getrewardID(season string)  int{//得到具体的奖励id
  // 先得到当前段位，
  level := getLevel(ladder.highscore) 
  var rewardlist string 
  idx := 0
  for {
    tmpconf := getLadderSeason(idx)
    if season == tmpconf.name {
        rewardlist = tmpconf.reward
        break
    }
  }
  return splitRewardStrRaw(rewardlist,level)
}

如果是，执行更新
func GetNewScore(score int32) int32 {
	var newscore int32 = 0
	var curIdx int32 = 1
	for {
		levelConf, has := conf.GetLadderGrade(curIdx + 1)
		if !has {
			break
		}

		need := levelConf.ScoreList[0]

		if score < need {
			break
		}
		own_levelConf, has := conf.GetLadderGrade(curIdx)
		newscore = (own_levelConf.resetScore)
		curIdx++
	}

	return newscore
}
func GetNewRewardLevel(score int32) int32 {
	var curIdx int32 = 1
	for {
		levelConf, has := conf.GetExploreLevel(curIdx)
		if !has {
			break
		}

		need := levelConf.score

		if score == need {
			return curIdx
		}

		curIdx++
	}

	return curIdx
}


判断赛季是否更新
isupdateseason = 0;

	if err != nil {
		logrus.Errorf("UserLadderRewardRec create failed %v %v", msg.UserID, msg.RewardId)
		return
	}


//得到当前赛季
func getnowseason() string{  
  nowtime := uint64(time.Now().Format("20060102150405"))
  idx := 0
  for {
    conftmp := conf.GetLadderSeason(idx)
    t := ParseTimeToUtc(conftmp.start)
    timestamp := t.Unix
    if nowtime < timestamp {
      break
    }
    	
    idx ++ 
  }
  return conftmp.name
}

//先得到这个赛季的段位，好像不用这一步了，不过也可以判断一下数据库中存储的与理应的是否一致
func getLevel(highscore string) int{
  idx := 1
  for {
     tmpconf := getLadderGrade(idx)
    if highscore < tmpconf.ScoreList[0] {
      break
    }
       
     idx ++
  }
  return idx - 1
 
}

func main {
  //得到当前赛季
  nowseason := getnowseason() 
  //查看数据库中是否有赛季信息
	user3 := &People{}
	db.Model(&People{}).Select("season", "name").Where("age = ?", 18).First(&user3)
	fmt.Println(user3.Season)
	if user3.Season == "" { //如果信息为空，说明用户第一次登陆，给它配置一下赛季信息
		db.Model(&People{}).Select("season", "name").Where("id = ?", msg.userid).First(&user3).Updates(&People{
      Season: getnowseason(),
		}).First(&user3)
    //更新完赛季直接返回就是， 通过那个通用返回消息(注意去看一下得到更新后的userdata的处理函数是否有设置我新增的两个字段(season和是否改变))。
  } else if users3.Season == getnowseason() { //赛季信息等于当前赛季
    //这种情况也直接返回就行
    
  } else { // 说明赛季发生了变化。
    
//根据当前赛季得到具体的奖励id
	getrewardID(users3.Season)  得到具体的奖励id
 //接着把这些奖励写入mail数据库表中， 直接返回前端
  
// 如果有， 判断一下是否跨赛季，跨了则执行领奖逻辑，没跨就直接返回。
//如果没有就把当前赛季记录一下， 不考虑领奖问题，直接返回user信息。

//有，如果跨赛季了
}

~~~





1.ladderreward

2.explorreward

3.新接口判断是否更新赛季

4.用户登陆的时候把赛季信息返回给他，如果没有历史就根据当前时间赋值一个版本信息给它， 每次打开排位赛界面的时候再去判断是否跨越季度了

需要在mysql中加3个字段： own赛季，now赛季,赛季是否改变





我会记录我当前领到那个探索等级了， 如果是通过段位分， 需要先通过已经领到的等级在explorelevel中得到所需天梯分，然后拿这个天梯分去laddergrade判断上限分数， 然后用上限分数去explorelevel中得到当前段位的上限领取等级(一共需要三次)， 因为服务器是记录的当前领到哪个探索等级的奖励。 如果说服务器通过记录分来判断领到哪个位置的奖励是不行的(因为配置文件的主键是探索等级。)



我认为需要改的， 把score改成可领id， 把数据库中的探索值改为可领id， 以前的score就通过数据库中的这个值来赋值 

数据看板：账号 jiangtao	密码 nsswdsm58

debug代码：/Users/jiangtao/data/project328-client/Assets/Scripts/DebugCmd/CustomCmdRegister.cs

装备码

~~~
礼包信息在GiftBag表，有随机，固定，可选三种
宝箱信息在LotteryMain表，各种宝箱
~~~

![image-20230524150351313](/Users/jiangtao/Library/Application Support/typora-user-images/image-20230524150351313.png)

作弊码

```go
添加钻石：setDiamond+空格+数值 “setDiamond 10000”

添加金币：setCoin+空格+数值 “setCoin 10000”

跳过新手：setGuide+空格+值“setGuide 10”

调奥林岛等级：updateExploration+空格+等级 updateExploration 50

局内AI投降：b.surrender+AI昵称 “b.surrender先贤显现”——局内的debug只能在运营阶段使用，战斗阶段使用不生效

局内增加随从：b.get+随从名 “b.get蝌蝌”
```





**小知识**

~~~go
0.deploy-tool的脚本的generater.sh + 服务， 单独启动某一个服务
1.可以在rider中起服务针对某一个服务调试，rider起的服务优先级高
2.GlobalServerConfig这个全局对象加载的就是service.ini
3.service.ini 中的consul意为领事的意思，访问它对应的url可以得到各个服务器的地址
4.使用到公网ip的目前只有battle-server_manager.ini ,其他都是用的local IP
5.Fluentd是一个开源的通用日志采集和分发系统
6.project328中global下的一些参数具体的值是通过serivce下的evn.sh配置的 
7.通过excel表格倒出来的数据会生成.pb和.json文件，生成的文件在服务对应的/ci/asset/card-game-config下
8.deploy-tool/bin/lib/generate.go:根据环境配置文件，比如version.sh(deploy-tool/version.sh和deploy-tool/config/target/version.sh)和evn.sh(deploy-tool/config/env.sh)，配合go中的template方法，生成四套环境(./deploy-tool/config/target下,会拷贝一份到缓存(~/.deploy-tool/dev下))。 比如每个服务都有的service.ini配置文件就来源于这四套环境
9.功能性proto文件存放在config-tool/protos/rawdata下,  生成的所有.pb.go文件其实是在config-tool/gen/golang/project328-server/gen/rawdata，每个服务启动的时候都有一个service.ini文件(从四套环境中拷贝过来，放在服务对应的conf目录下)，里面有一个CardConfig路径的配置，通过它进一步会把所有生成的.pb.go文件弄到服务对应的/gen/rawdata下，以及gen/conf下的gen系列也是这样得到的（直接去project328的server.go看service.InitCardGameConfig()，就是在做这个事情, 还有很重要的一个事情就是将excel生成的.pb文件的数据加入到了xx_conf.pb.go中）；
基础配置性proto文件在proto-tool/protos下， 最后会被同步给
10.excel文件都在config-tool/excel下
11.proto-tool下有一些基础信息的proto文件，比如用户信息(userdata.proto), 各种消息回复(message.proto比如领了奖品后回复)等等，而前边说到的config-tool/protos/rawdata下的proto文件倾向于一些功能性的协议, project328-server/pb下的.proto文件就来自于proto-tool
12.去看proto-tool下的userdata下的ShowRewardData， 逐步可以看到领取奖励的一系列信息，ShowRewardData(这里面可以看到一个userData成员，它表示的是领取奖励之后的用户信息)->Reward(单个奖励,是一个切片成员，表示有多个奖励)->RewardFromInfo(因为什么得到的奖励)
13.领取任务奖励相关的内容在get-task-reward.go文件中可以看到。 主要函数getRewardMsg
每一个任务都对应了一个奖励号，需要通过这个奖励号去simplereward表格中才能得到真正的奖励
14.在project328配置好了consul的地址后，比如得到了uds的url(127.0.0.1:12000)，uds的所有web服务方法的实现都在user-data-service/main.go中可以看到
15.每个proto文件都有一个_conf.proto文件，里面是一个map表，实际上就是excel表对应的每一条信息。
16.在gen_load.go中GetSimpleReward函数可以看到获取excel奖励表的某一行内容，每一行中有一个rewards字段代表奖励，可以是多个，并且其中可以有礼包或者宝箱，需要一直拆解下去，最终得到一个奖品数组
17.礼包是分了类型的，有随机的(根据weight权重来决定)，有固定的，还有可选的，比如大厅任务中的战令任务完成后就能得到礼包(这个是固定的)
18.玩家获得奖励的时候，对应了三个字段，类型｜ID｜数量， 如果是简单的比如货币，根据ID来决定是金币还是钻石，不用单独配置excel表， 而对于礼包和宝箱则需要配置excel(GiftBag,LotteryMain)
19.天梯之路和战令路线的奖励不属于任务奖励， 领取了之后如果把分数扣下去不会显示已经领取，但当你把等级升回去的时候也不可以重复领取,任务对应的playertask表， 天梯之路对应的是explorelevel表
20.比如玩家点一个领取时， 服务器这边会处理做真正的领取操作，如果领取成功，会返回一个通用消息，主要是两部分， 一部分就是结果码(成功与否)，第二部分是ShowRewardData消息，它里面又包含了本次领取的所有东西，以及用户领取完后的一个新的状态(以一个userdata结构新状态信息)， 所以这个通用消息能解决大部分问题。
~~~



**hoxi-server这个包中比较重要的几个部分**

~~~go
通信实体接口或者一些其他的实体接口都在hoxi-server/interfaces包下
各种proto配置文件(xx_conf.proto)的信息在hoxi-server/conf包下
一些实用工具在hoxi-server/util包下
~~~







**大厅服务的一些细节：**

~~~go
大厅任务界面涉及的excel在config-tool/excel/PlayerTask下

PlayerTask 具体的任务

BattlePassLevel 对应大厅战令中的普通/进阶任务路线

BattlePassTask 任务中的活动战令任务（根据不同的路线有不同的任务）

NewHandTask 任务中的新手任务

WeekTaskReward 任务中的周常任务奖励

DailyTask 任务中的周常任务对应的每一个小任务点就是日常任务
~~~



思路阶段

~~~go
完成任务领奖过程：
1.最开始是在server.go初始化的时候根据配置文件设置好consul的位置，通过consul得到uds服务的位置，赋值给roject328-server/service/userdataservice/main.go 中的uds对象的url成员
2.以project328-server/api/get-task-reward.go中的Handle函数为例，先拿到客户端的请求，根据用户ID拿到userdata对象，判断一下任务是否完成以及是否已经领取过了
3.第二步如果通过，通过第一步得到的url地址加上/user-get-simple-reward(获取不同的奖励加不同的语句)就能得到需要的http服务，这些所有的http服务都是数据相关的服务(具体的语句与处理函数的映射关系都在user-data-service/main.go中可以看到)，这里对应的是UserGetSimpleReward函数，先拿到任务id，通过任务id去simplereward表中找到对应的奖品，奖品可能有多个，并且单个奖品可能是礼包或者宝箱，需要层层打开，最终会返回一个ShowRewardData对象(它里面用切片的方式包含了所有的奖品信息，以及更新后的用户信息)。
~~~

探索之路相关

~~~
userdata.proto中有一个探索信息结构，包含了当前探索值，已领完的奖励id，已领取的随机奖励结果列表
~~~

