核心点：k8s等中间件学习 ， 算法， 基础

远程道具110

Todo: 1. 限时升级道具礼包(完成）， 2.新增两种道具(完成) 3. 获得高级皮肤，默认拥有低级皮肤(完成) 5提醒前端加错误返回码 6. ai珍藏值 7.额外金币加成

我就关心两个点， 前端定时器到0的时候禁止玩家操作， 2. 点击确定等待返回前禁止操作 3. 前端要用后端给的英雄信息

问题1. 前端可能展示了后端不允许的英雄， 问题2.前端同时发信息， 问题3.前端有预展示，4.前端超时是几秒， 5. 预选英雄不合法，怀疑是前端点了使用手选卡之后直接进入选择界面了，或者说使用手选卡失败，重新连接有什么标志位没清除， 所以展示了使用手选卡才能玩的英雄, 

0的时候不能点？

预选保证顺序，相信确定消息

选英雄到进入战斗到时候会再次触发重连吗

计划：1.就算不能预展示，那么在切换英雄的时候应该禁止点击确认，hy54	`

担心点 ：发送预选消息的时候，后端返回错误信息

选择英雄整理:

状态：1. 预选， 2.确认选择 3. 只换皮肤

问题分析： 1. 消息同时发过来，处理顺序不一致 2. 

main
rebase1
uds的滚动更新是k8s保证的吗， 如果正在执行业务怎么办

```
合并分支尝试用下rebase而不是merge
b.getMinion 435,BuffID:11681 哥斯拉
b.getMinion 724,BuffID:11681 老虎
b.getMinion 429,BuffID:11681 恶霸
b.getMinion 729,BuffID:11681 豹冰冰
```

24:26

7:27

18:51

234（小蜂，239(女王+2391的buff)， 230(任意), 913河马

数据双写, 定时任务的异常捕获





设置200个连续玩家的榜单分数

~~~shell
for i in $(seq 10001 10199); do
  docker exec -it redis redis-cli ZADD ChampionshipBoard_2_25 666 $i
done
~~~

~~~go
// 使用pipeline
	pipe := global.DataRedis.Pipeline()
	for _, uid := range users {
		res := pipe.HMGet(context.Background(), GetUserDataKey(uid), redisFieldList...)
		cmdList[uid] = res
	}

	_, _ = pipe.Exec(context.Background())
	for uid, cmd := range cmdList {
		userData, err := userRawData.LoadFromPipelineResult(uid, prepareSet, cmd)
		if err != nil {
			logUtil.FormatLogError(logUtil.LeaderBoard, "GetUserLBInfo", "获取玩家数据失败", uid)
			return ret
		}

		// 拼数据
		var entity = &pb.LeaderboardEntity{}
		entity.Uid = uid
		entity.PersonInfo = makePersonInfoFromRawData(userData)

		switch lbType {
		case pb.LeaderboardType_HERO_BOARD:
			// 英雄榜
			entity.HeroInfo = HeroInfo2HeroDetail(resource.Hero.GetPbById(userData, heroId))
		case pb.LeaderboardType_CRITICALHIT_BOARD:
			// 暴击榜
			entity.MinionInfo = getMaxDamageMinionInfo(uid)
		case pb.LeaderboardType_BEAST_BOARD:
			// 巨灵榜
			entity.MinionInfo = getBeastMinionInfo(uid)
		}

		// 添加进返回给lbs的集合中
		ret = append(ret, entity)
	}

~~~



确认英雄选择的时候要带上当前英雄和使用的皮肤

### 埋点关键字眼

battle_hero_choose: 选择英雄信息

现在赛季切换危险的地方:  石灵收藏点升级

只要是前端发送的请求是加了玩家锁的。

todo : init检查活动时间的时候处理

阿里云账号

~~~
jiangtao@1936892076124227.onaliyun.com
公网114.55.88.127
~~~



新抽奖活动资源检查

周榜更新可能有问题，操作mysql数量太多

[]*int 和 *[]int的区别

搞懂删除镜像的命令



疑问：十连==10次单抽吗

日志级别调整

// 找到大于并最接近newAwardCnt的限制组，比如newAwardCnt=9, limit(10:1)就是最接近的 findK对应10，findCnt对应1

空切片append空切片

index := pie.FindFirstUsing(data.ChapterFinishInfo, func(chapterInfo *pb.ChapterInfo) bool {
    return chapterInfo.ChapterId == chapterId
})

学一下循环调用的设计

获取皮肤的时候需要检查是否拥有基础皮

限制进入飞

看一下pve的任务是怎么加载的，做两个事情：1.活动任务， 2.加载活动时机

优化手机号的索引

会议纪要

~~~
1.专门搞一个密钥的配置仓库来管理权限
2.vpc是什么
3.写个脚本10分钟获取一下动态ip，发过来
4.滚动更新会断一部分玩家
5.服务器通过lb暴露ip， lb只有一个
6.logrus是同步写日志
~~~

utils.Base64Unmarshal()解码userdata

没做的事， hotfix/20231228的礼包奖励还没切到dev,  2. douyin分支合到dev上

1. 匹配成功会发送BR_GameStartMsg里面包含了可选英雄的信息，比如是否免费手选卡之类的
1. ubs_10000是战斗状态
1. 如何确定当前房间的模式， 构建BattleRoyale对象的时候可以看到， 
   GmaeType枚举类型(Normal和Championship)表示游戏类型，只要不是锦标赛就都是Normal；
   RoomMatchType枚举类型(Public和Private) 表示房间类型，锦标赛和排位是Public， 匹配是Private
   TeamType枚举类型，也表示游戏类型，排位赛(PublicKnockout), 匹配赛(PrivateKnockout), 锦标赛(PublicChampionShip)
1. cardhero的deckrace表示卡池种族(在商店卖的)， race是显示出来的种族

Todo:

1. ugc分支合并(包含协议分支), 华为渠道分支合并

2. 学习go的切片和指针切片等

~~~
// 单个赛季信息
message SingleSeason {
  int32 season = 1;                         //赛季
  int32 total1stCount = 2;                  //总夺冠次数
	int32 totalWinCount = 3;                  //总胜利场数
  int32 matchTotalCount = 4;                //匹配总场数
  int32 qualifyingTotalCount = 5;           //排位总场数
	int32 csTotalCount = 6;                   //锦标赛总场数
	int32 csWinCount = 7;                     //锦标赛总胜利场数
	int32 csCompletionCount = 8;              //锦标赛通关次数
  int32 csHighestSingleRoundWin = 9;        //锦标赛单轮最高胜场
}

~~~



需要支持的，

// UserAttackDubbing 攻击配音
type UserDubbing struct {
    gorm.Model
    UserID uint64 `gorm:"index:i_dubbing_user"`
    // 音频唯一标识
    DubbingPath string
    // 上传者名字
    HeroID     int32  `gorm:"index:i_dubbing_hero"`
    UpUserName string `gorm:"index:i_name;collate:utf8mb4"`
    IsPublic   bool
    IsHot      bool
    DubType    pb.DubbingType
}

服务器维护中返回码处理

~~~go

func test() {
	params := map[string]interface{}{
		"game_id":     bili.BiliGameID,
		"merchant_id": bili.BiliMerchantID,
		"server_id":   bili.BiliServerID,
		"uid":         172372941,
		"version":     bili.BiliVersion,
		"timestamp":   uint64(time.Now().UTC().UnixNano()) / uint64(time.Millisecond),
		"order_no":    "20231204181701458434157",
	}
	// post构建请求参数
	postParams := url.Values{}
	for key, val := range params {
		postParams.Add(key, fmt.Sprintf("%v", val))
	}
	postParams.Set("sign", bili.GetBiliSign(params))
	urls := []string{bili.BiliQueryUrl, bili.BiliBackUpUrl1, bili.BiliBackUpUrl2}
	for _, url := range urls {
		resp, err := bili.SendRequest(url, postParams)
		if err != nil {
			global.Logger.Warnf("查询B站订单失败失败, 尝试换线:url:%v, resp:%v", url, resp)
			continue
		}

		// 读取响应内容
		body, err := ioutil.ReadAll(resp.Body)
		resp.Body.Close()

		var order bili.QueryOrderBili
		err = json.Unmarshal(body, &order)
		if err != nil {
		}

		if order.OrderStatus == bili.Payed {

		}
		// 只要有一个url可用就退出
		break
	}
}

~~~



疑问：为啥一级指针赋值为null，作为参数传递不会影响到值

lbs检测ac时间调长一点，解开25限制

Todo: 完善排行榜日志， 缓存加载更替细节

埋点看某一时刻玩家领取的东西

配表反射怎么搞

![image-20231012192210562](/Users/a123/Library/Application Support/typora-user-images/image-20231012192210562.png)

想问的问题， 多服是怎么实现的。

test.hall.project328.hoxigames.com 8999 联赛服   

taptap.hall.project328.hoxigames.com 8999 taptap服     

approve.project328.hoxigames.com:8999 提审服    

```
查看联赛服pod有哪些
kubectl get pods -n test

查看某个pod的日志
kubectl  logs -f test-user-data-service-6fcd5754c6-6frr5 -n test test-user-data-service --tail 100

服务器的所有日志
/k8s-data/fluentd  联赛服是以k8s_test开头的

查看9005的uds
docker ps
docker logs 8fd5abf06c6d -f --tail 10

联赛服数据库
kubectl get svc -n test
里面有个test-mysql，能看到extanelname，是个地址
15:54
mysql -h 那个地址 -uroot -p
密码是hoxi0328JING
mysql -h rm-2ze3rkvpx9768m99v.rwlb.rds.aliyuncs.com  -u root -p

redis-cli -h r-2ze3br4wfpg5hja167.redis.rds.aliyuncs.com


taptap服务器

kubectl get svc -n taptap
里面有个taptap-mysql
mysql -h rm-2zevgb9724yvm7951.rwlb.rds.aliyuncs.com -u root -p

redis-cli -h r-bp1lb2qpinvy0ze8ae.redis.rds.aliyuncs.com
UDS_USER_
```



%system.teamcity.build.checkoutDir%/Assets/ 路径

数数，数据看板：账号 jiangtao	密码 nswdsm58， 用的是我的本地时间

~~~
点击用户-》用户细查-》搜索user_explicit_id外显id查看玩家事件
~~~



ssh [deploy@47.95.6.108](mailto:deploy@47.95.6.108)

Reddit: AcademicLandscape180   nswdsm58

Kindly_Owl_6770

debug代码：/Users/jiangtao/data/project328-client/Assets/Scripts/DebugCmd/CustomCmdRegister.cs

### token登录

~~~~


// 生成token
token := fmt.Sprintf("%v_%v_%v_%v", global.TapTapLoginType, account.ID, msg.ObjectID, time.Now().Add(global.TokenLoginTimeout).Unix())
token = encrypt.EncryptString(token, global.TokenEncryptKey)
拿到这个token去unity登录
在main函数开头debug一下， 主要是获取时间戳和编码一下

token := fmt.Sprintf("%v%v", "TapTap_1063_652ff1e95cc71c58cd4fb756_", time.Now().Add(30*24*time.Hour).Unix())
test := encrypt.EncryptString(token, "H*a&OJgx^@eI14)s")
fmt.Printf(test)

//1063_652ff1e95cc71c58cd4fb756这串是需要换的，右边这个看起来是密码的串
~~~

手机号渠道token

~~~go
直接复制到328， 92是account_id 
token := fmt.Sprintf("%v_%v_%v_%v", global.MobileNumberLoginType, 92, "15652660058", time.Now().Add(global.TokenLoginTimeout).Unix())
	token = encrypt.EncryptString(token, global.TokenEncryptKey)
~~~

抖音的token获取方式比较特别， 可以直接修改自己的账号的account_id为需要登录的那个号的account_id, 然后登录自己的账号，信息就是你需要的那个人的


测试包：

~~~
/Applications/Unity/Hub/Editor/2020.3.47f1c1/Unity.app/Contents/MacOS/Unity -batchmode -projectPath ./ -executeMethod Build.Builder.BuildAndroid -logfile - -outputPath ~/tmp/android -cachePath ~/tmp/cache -channel Inner -packageName com.hoxigames.test.project328 -appName Project328Test -appVersion 0.99.155 -forceLog on -hallServer 47.95.6.108 -hallPort 9001 -developmentBuild false -enableProfiler false -androidVersionCode 216 -disableDebugTool off -buildAab off -deepProfiling off -quit -logFile
~~~



app：https://launch.hoxigames.com/index.html

gitlab：https://gitlab.hoxigames.com/users/sign_in

ci：https://hoxigames.feishu.cn/wiki/wikcnPfIzdcQk1qL7Z69hSgZaKb

成大邮箱：http://mail.stu.cdu.edu.cn/cgi-bin/loginpage?s=session_timeout&from=&r=1e92dfa32ab874b79be5624f7c54e1ab

```
9003 ：
ssh deploy@47.95.6.108
docker exec -it dev5-mysql /bin/bash
docker exec -it dev5-data-redis redis-cli
```

装备码

~~~
礼包信息在GiftBag表，有随机，固定，可选三种
宝箱信息在LotteryMain表，各种宝箱
~~~



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
21.ci其实就是去执行config-tool/build.sh， 它做了这么几件事， 先给一个缓存目录， 去这个缓存目录下把你需要ci的所有git仓库代码都拉取(现在公司是develop分支)到那个缓存目录下， 然后本身把excel表格序列化后， 把这些序列化后的文件同步给所有的git仓库(大厅服务，uds，client等)， 这样就叫集成。
那么在config-tool/env.sh中配置路径是为了方便自己本地调试，执行gen.sh会重新把excel生成序列化文件根据env.sh的配置同步到本地(自己选择同步uds还是大厅服还是client，但这都还只是本地，同步完后并没有push， 而ci是做了push操作的)
22.每次调用请求返回的userdata不一定是完整的，所以客户端采用的是merge方式， 如果返回的userdata包含的有的字段就覆盖之前的，如果没有就继续用之前的。
23.GameConfConstant表中记录了一些固定的游戏相关常量配置， 比如战令进阶豪华版的信息等等。
24.注册一个玩家时需要设置最基本的信息， 比如新手礼包等等，在uds/utils/data.go的findOrCreateUserByAccountID中
25.登陆注册过程在MainScreenSceneManager.cs中
26.战斗服接口在TcpServer.cs
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

