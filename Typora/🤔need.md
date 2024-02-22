### 53. 抽奖系统

### 活动

**吞吞:**

判断上次使用时间是否是昨天 lastUseTime.Before(timeUtil.GetFixedHourTime(userData.LoadTime(), conf.NEW_DAY)) 

**新年活动:**

子模块(player.cs)和battleRoyale可以通过Campaign进行数据的关联

### 52.线上

/data/tool/

### 51. 匹配

~~~
match_success
~~~



### 50.实名认证

*MESSAGE_TYPE_REAL_NAME_AUTH*

### 49.锦标赛

尝试开启锦标赛：*MESSAGE_TYPE_BR_START_NEW_CHAMPIONSHIP*

路由搜索start-championship

### 48. UGC存储

OSS(Object Storage Service) 是阿里云提供的一种云存储服务，更像是一个文件存储系统，主要用于存储和访问大量非结构化的数据，例如图片，视频等。而 MySQL，HBase 是数据库管理系统，用于存储和管理结构化的数据。这些系统通常需要更复杂的查询操作，例如关联查询，索引查询等。

测试数据：

~~~go
上传音频资源：
328中
audioFilePath := "/Users/a123/a.mp3"
audioData, err := ioutil.ReadFile(audioFilePath)
objectKey := fmt.Sprintf("%v%v", "dubbing/", "123456")
err = global.OssClient.PutObject("assets/"+objectKey, audioData)

// uds中
var dubbing = &model.UserDubbing{
		UserID:      10000,
		DubbingPath: "123456",
		DubbingName: "测试",
		UpUserName:  "jt",
		IsPublic:    true,
		DubStatus:   pb.DubbingStatus_Dub_WaitingForReview,
		DubType:     pb.DubbingType_Dub_Attack,
	}
	utils.Create(dubbing)
	
	// gm-tool仿照查询
	dubbing.SearchByCondition(map[string]interface{}{
		"heroID":   float64(101),
		"isPublic": false,
	}, 5, 0)
	// gm-tool更新状态
	msg := pb.UpdateDubAuditStatus{
		ID:     uint64(1),
		IsHot:  false,
		OpType: pb.DubbingOp_Dub_UpdateIsHot,
	}
	jsonStr, _ := json.Marshal(msg)
	dubbing.UpdateDubbingStatus(jsonStr, nil)
~~~

### 47. PC登录

1. Conn_redis里的s_10000记录玩家所在大厅服标识， b_(大厅服标识)是对应的地址
2. 看328的RegisterToBroadcaster, 328先发送*MsgSetServerID*给广播服告诉广播服自己的唯一标识id，然后就建立连接了， 广播服维护了一个ServerConnIDMap来管理所有的大厅服连接
3. s_10000是在LockSetSession里生成的
4. 在func (c *Connection) Stop() {看具体移除的连接的connID
5. 在328的func (mgr *Manager) send(可以看到发送消息给客户端的流程，可以看出来是通过userID来找到玩家的
6. OnConnStart和OnConnStop分别是刚连接和刚断开的hook函数，在InitTCPServer的时候设置好的，可以算在线时长

### 46. 服务器相关

1. docker的镜像的名字的第一部分是分支commit，第二部分是数值commit

### 45.战令系统

1. 单个领奖get-battle-pass-reward
2. 全部领奖get-all-battle-pass-reward

### 44. 支付

看pay center相关日志

~~~
1. 搜索日志服务SLS进一步找到aliyun-fc-cn-进一步找到上下文浏览
2. 函数计算FC
~~~



**小米支付**

https://dev.mi.com/distribute/doc/details?pId=1428

细节点： 

1. 在对方notify我的时候，判断消息里面的状态是不是完成支付（有可能是未支付)， 如果是才设置已支付状态
2. notify我的时候， 返回给他的时候要告诉他我已经成功处理了这个notify， 不然他会定时发
3. 注意别人发我的http请求中，消息不一定在body里面，可能在query里, http就三部分，body，query，head
4. io.readall(reso.body)一般就是我们需要的数据(json应该是)， 可以直接通过json.unmarshl解析成json串

**登录：**

![](/Users/a123/Library/Application Support/typora-user-images/image-20231127152831518.png)

**支付：**

![image-20231127165411165](/Users/a123/Library/Application Support/typora-user-images/image-20231127165411165.png)

加密流程：

https://mis.migc.xiaomi.com/api/biz/service/verifySession.do?appId=2882303761520282579&session=cm56jSgp57ExdWFY&uid=2023112800235103&signature=1d0f88a5197c2b38a7663d47ca89cba44a40dc01

~~~go
func getRequestUrl(params map[string]string) string {
	signString := getSortQueryString(params)
	signature := getSign(params)
	return orderQueryURL + "?" + signString + "&signature=" + signature
}
//1. params是除了signature的其他对象，要先进行排序，以一个value为对象，比对value的大小，从小到大排序

func getSign(params map[string]string) string {
	signString := getSortQueryString(params)
	return HmacSHA1Encrypt(signString, miSecret)
}

func getSortQueryString(params map[string]string) string {
	keys := make([]string, 0, len(params))
	for k := range params {
		keys = append(keys, k)
	}
	sort.Strings(keys)

	var sb strings.Builder
	for _, k := range keys {
		sb.WriteString(fmt.Sprintf("%s=%s&", k, params[k]))
	}

	s := sb.String()
	return strings.TrimSuffix(s, "&")
}

~~~

**B站登录**

1. 返回的时候要求是[success]而不能是["success"]， 只要不使用json.Marshal就是前者的效果

**华为登录支付**

开发者服务器创建好了内部订单号后， 前端在吊起支付的时候把我们自己的订单号发过去，到时候支付完杀端，前端查询SDK数据的时候，就可以看到我们自己的订单号， 然后再传给开发者服务器

### 43. 手机验证码登录

阿里云不支持游戏通知短信， 可以用这个https://www.mysubmail.com/documents/49BVE

1. 登录有很多方式， 第一次登录的时候，比如验证码登录会先检验一些基础信息， 如果通过的话会返回一个token，玩家之后就用这个token进行登录， token格式如下:

   ~~~go
   token := fmt.Sprintf("%v_%v_%v_%v", global.MobileNumberLoginType, account.ID, msg.PhoneNumber, time.Now().Add(global.TokenLoginTimeout).Unix())
   	token = encrypt.EncryptString(token, global.TokenEncryptKey)
   前端在PathHelper.cs修改指定token
   ~~~

2. 使用token登录的时候，又把这个token反解出来看看password对不对的上，如果是手机号登录，那password一般就是手机号，小米的话，就是小米唯一id，当然也可以采取对密码进一步加密。

3. 验证码校验需要加限制，比如15s内只能尝试3次，  验证码请求次数阿里已经做了限制， 比如一分钟只能请求2次

4. 注意是验证码用字符串， 不然0开头的有问题

   

### 42.bug前端记录

1. 收到新一轮回合开始BattleScene.BattleSceneManager.ReceiveBRNewRoundMsg

### 41.新版uds架构

1. 新的RespError可以作为error对象返回，因为实现了Error()方法, 涉及到资源花费的时候，如果有resultcode，最后会在currency.go中的errors.As方法进行转化，返回给前端实际的错误原因

~~~
if err != nil {
		var respErr *errors2.RespError
		if errors.As(err, &respErr) {
			return TransRet{
				Err:        err,
				ResultCode: respErr.Code,
			}
		} else {
			return TransRet{
				Err:        err,
				ResultCode: pb.RESULT_CODE_ERROR,
			}
		}
	}
	
	
	if ret.Err != nil {
			respErr, ok := ret.Err.(errors2.RespError)
			if ok {
				ret.ResultCode = respErr.Code
			}
		}
~~~

### 40.商店系统

RewrdType：12是礼包， 4是宝箱

2.资源转换：buyShopGoods的时候info字段会携带相关信息

1.皮肤商店的道具在shopGoods.xsls中，比如手选卡对应216

2.道具里面的8个东西(每日礼品和每日金币，以及开宝箱的单抽和十连抽都属于道具，在shopMain里面, 指向shopGoods.xsls)， 对应的状态存储在user_shop_item_data中

3.购买记录都会在buy_records的sql表记录， 比如**神秘商店**对应type为3

~~~
select * from buy_records where user_id = 10001 and type = 3\G;

把buy_records的记录全删了，神秘商店就可以全部重新购买
~~~

4. 道具比如手选卡和钥匙记录在user_bag_items中
4. 随机石灵碎片，第四个参数是决定分多少组的

**礼包系统：**

增加一些有时间限制的礼包：addTimeShopGiftBag

**目前可以这样理解：LotterMain.xslx就是宝箱， 里面的具体奖励在LotteryAward.xslx看**

相关表：ShopGift、ShopGoods, GiftBag、LotteryMain、LotteryAward、LotteryNew

1. 目前商店的礼包栏中，商店中的礼包在ShopGift中(进一步是GiftBag中),在user_shop_gift_bag_data记录购买情况，会在splitGiftBagReward函数去解析礼包里面包含的东西，直接存放在user_shop_gift_bag_data中的content字段

2. 举例：比如ShopGift的主键3，可以看到award字段对应的是12,203,1 其中12表示是个礼包，203是主键(对应GiftBag的id)，然后去GiftBag看203，可以看到reward字段又有一个12|503|1, 通用12表示礼包，503同样是GiftBag的主键(当前是一个随机史师级英雄)， 然后目光来到503对应的reward，里面是4|23|1, 其中4为RewardType_RewardBox(表示宝箱，23对应LotteryMain表的id), 然后目光来到 LotteryMain主键23对应的awardLibrary字段为23|1|1|1, 重点来了，这个23表示libraryID(对应LotteryAward表的library字段) 

3. 接着第二点说明， 看OpenLottery函数，通过genLibraries得到libraries之后，会遍历一遍libraries，每次筛选了LotteryAward中library值一样的对象。
4. lotteryNew表会提升中奖概率（原因是每次如果获取了重复的英雄，LastGetNewHero字段就会+1， 当这个数越来越大，就会提升中奖概率，
5. 礼包可以再开礼包和宝箱， 但宝箱不能再开宝箱
6. getRewardCount根据权重决定这个宝箱的奖励有多少个（看LotteryMain的count字段）
7. **宝箱的核心**是通过OpenLottery里面的genLibraries函数计算宝箱里面包含的奖励（也就是LotteryMain的awardLibrary字段）对应LotteryAward的library（而不是ID
8. 权重计算的本质就是， 把所有比重加起来， 比如20+100+200, 那么就是320，然后在320中随机一个数，比如180，那就是20<180, 20+100 < 180 , 20+100+200 > 180,于是就选中了这个200比重的物品 

9. 宝箱的最终奖励就是LotteryAward的内容（比如你抽到libraryID是23，就找到library为23的所有行根据一定概率选一个出来）， 所以如果要操纵宝箱得到的奖励，比如宝箱开出来的英雄， 如果是重复的，可以动手脚给这项加一个字段， 表示，如果这个奖励被抽中并且重复了，就给她换成不重复的。

### 39. 服务搭建

~~~
1. 先bin下搜索user-data-service,
2./Users/a123/ku/deploy-tool/config/tmpl加一个tmpl.ini
3./Users/a123/ku/deploy-tool/project加一个sh
4./Users/a123/ku/deploy-tool/config 这里面的env.sh加一个新服务的配置信息
~~~

**39.1 ci上部署服务，打包机缓存没清问题**

~~~
47.95.6.108 /data/deploy-tool/
进去stash一下

然后pull下来

在stash pop回去
~~~

39.2 加入到ci数值部署中

~~~
数值部署增加服务:config-tool下修改
新服务自身需要有一个deploy文件夹(仿照uds加)
~~~



### 38.AI优化



### 37. 邮件

前端在mailpanel.cs中的btmmail发送MESSAGE_TYPE_GET_MAIL_REWARD

~~~
328先发送mail-get-single获取这封邮件， 如果存在的话并且没有领取则进一步调用mail-set-status设置状态为删除(表示这次就会领取他)
~~~



### 35.mysql和redis数据处理

~~~mysql
#导出联赛服的mysql,sed '24,25d' 表示删除这两行(SET @@GLOBAL.GTID_PURGED长这样)
mysqldump -h rm-2ze3rkvpx9768m99v.rwlb.rds.aliyuncs.com -u root -phoxi0328JING project328 | sed 's/utf8mb4_0900_ai_ci/utf8mb4_general_ci/g' | sed '24,25d' > /tmp/project328.sql

taptap服
mysqldump -h  rm-2zevgb9724yvm7951.rwlb.rds.aliyuncs.com -u root -phoxi0328JING project328 | sed 's/utf8mb4_0900_ai_ci/utf8mb4_general_ci/g' | sed '24,25d' > /tmp/project328.sql

#本地创建一个docker镜像
docker run -d --name=mysql-remote -p 3306:3306 -v mysql-data:/var/lib/mysql -e MYSQL_RO    OT_PASSWORD=123456 docker.hoxigames.com/mysql:5.7

# 创建本地数据库
docker exec -i mysql-remote mysql -u root -p123456 -e "CREATE DATABASE IF NOT EXISTS project328"

# 导入数据库到本地
docker exec -i mysql-remote mysql -u root -p123456 project328 < project328.sql

#更改玩家密码为nswdsm58;如果是taptap登录，就把账号一起改了
update hoxi_accounts set password = 'BDC6CF885AA4C40E883D8B69B39072DF', account = 'jtnb' where id = 81;;  #比如此时uid为10080
~~~



### 34.指定镜像

~~~c#
public SelectMirrorBackMsg Select()
        {
            LogHelper.WriteLog("开始为战斗选择镜像:\n" + _Param);
            
            var ret = new SelectMirrorBackMsg();
            var usedHeroSet = new HashSet<int>();
            var curUserSet = new HashSet<ulong>(_Param.ExcludeUid);
          
            var mirrorCnt = _SavedMirror["AIMirror_KnockoutPool_1_4_101_All|Devil|Insect|Mutant|Neutral|Royale|Wild_Insect|Royale"];
            for (var i = 0; i <  _Param.NeedCnt; i++)
            {
                var index = RandomUtil.Next(0, mirrorCnt);
                var json = MirrorRedis.db.ListGetByIndex("AIMirror_KnockoutPool_1_4_101_All|Devil|Insect|Mutant|Neutral|Royale|Wild_Insect|Royale", index);
                AIHeroMirror mirror = null;
                try
                {
                    mirror = ((string)json).ToProtobuf<AIHeroMirror>();
                }
                catch (Exception)
                {
                    LogHelper.WriteError($"镜像格式出错,跳过该镜像:\n{"AIMirror_KnockoutPool_1_4_101_All|Devil|Insect|Mutant|Neutral|Royale|Wild_Insect|Royale"}\n{json}");
                }

                if(mirror == null) continue;
                // 兼容之前的数据
                mirror.RedisKey = "AIMirror_KnockoutPool_1_4_101_All|Devil|Insect|Mutant|Neutral|Royale|Wild_Insect|Royale";
                usedHeroSet.Add(mirror.HeroId);
                ret.Results.Add(mirror);
                // 每个RedisKey只选一个,保证多样
                break;
            }
            LogHelper.WriteLog($"选择到{ret.Results.Count}个镜像\n" +
                               $"{string.Join("\n", ret.Results.Select(r => r.RedisKey))}");
            return ret;
        }
~~~



### 33.道具

#### 手选卡

model.UserBagItem{}中，是道具类型(RewardType_Item), item_id为9(*BagItemId_BagItemId_HeroSelectCard*)

在神秘商店中买(就是用皮肤碎片兑换那里)， 具体的东西在ShopGoods.xlsx中

#### 

### 32. 机制

4.添加道具的技能：ActiveEntity

3.指定发现：ForceDiscoverDedicatedMinion

2.检查**是否需要加载词条**：GetEnableStageEnum

1.发现石灵相关看waitingForSelectionQueue， PlayerSelectMinionAction.cs

具有主动战吼的石灵不会加载词条（搜PrepareBattle

1.战斗开始前会加载一次词条(比如亡语狼和双角宝宝的词条生效阶段,看StartBattle)

2.火角山羊实质上是添加了一个叫BoomCounter的计数器(IncrPureCounter, 计数器一般都是通过配置表来添加的)，炎豚触发效果的伤害就会去累加这个值,  这种伤害效果叫*造成爆炸伤害*

3.光环的增益（atk,hp,addAtk有时候是用的字符串，比如GetAddDamageFromSelf， 这个Self一直都指的是光环拥有者，比如蝉就是光环拥有者）

4.光环的target(judger)用的是bool类型， 每个石灵去尝试获取光环buff的时候就用judger判断一下， 而entry中是通过target(selector)选择一批符合条件的石灵出来放到effect中处理

5.蝉原理， 买石灵的时候会去遍历光环，给商店的石灵加上光环的永久buff

6.把光环转为buff

~~~
OwnPlayer.AuraMgr.ToBuff(minion, BuffType.Temporary);
~~~

7.羊铃奶奶：她和奇迹奶牛一样，目的是让具有成长效果的石灵的成长效果额外触发一次， 拓展：大闹鹅的遗言是让友方触发成长效果，但他本身不是一张成长效果的牌，蛤蟆才是，所以他的tag上不需要加growth。

8.entry里面alltarget， 如果是战斗阶段加载的词条，就包含了敌方，如果是运营阶段的词条就只有己方

9.entry里面的triggerCondition要看细节就在TriggerFuncContent函数看，每个section对应一个小条件

10.AfterEffectPerform： 每个词条都会触发一次亡语效果的地方

### 31.动画

以changeheroskill为例，后端先添加动画ClipType.**ChangeHeroSkill**，前端在script/animation/animclips中找到对应的类的GenAnimation就可以看出来，最终调用RefreshRuneDress

### 30. 镜像

一开始所有的ai都会把镜像ai置为true，然后分配镜像，没拿到镜像的就把镜像ai置为false，然后设置为生成ai， 

一般镜像level-排名-英雄

把测试服的镜像搞到本地：先找到aof文件的位置：

~~~
docker exec -it dev4-mirror-redis redis-cli config get dir
~~~

进入mirror-redis

~~~
docker exec -it dev5-mirror-redis redis-cli
~~~



获取某个redis容器的aof：

~~~
docker cp 488132640bd0:/data/appendonly.aof  .
~~~

先清空

~~~
docker exec -i mirror-redis redis-cli flushall
~~~

再加载

~~~
拿到aof文件之后，docker exec -i mirror-redis redis-cli --pipe < aof文件位置(注意没有t)
~~~

如果需要指定选择镜像

~~~
如果只是同一个服务器上 ，直接改镜像服，获取指定key
如果不在同一个服务器上，就先把比如9002的aof导到9005上
~~~

本地将所有镜像的value搞成同一个

~~~
/Users/a123/project328/db/mirror-redis执行python脚本可以处理成同一个value, 但这种方法不可取， 就算是同一个value，拿到镜像的概率也低，因为有规则， 还是直接拿指定key比较好
~~~





创建mirror-redis镜像

~~~
docker run -d --name mirror-redis -p 6379:6379 --memory=4096m -v /Users/a123/.deploy-tool/dev/mirror-redis:/data docker.hoxigames.com/redis:6.2.4-alpine3.13 redis-server --appendonly yes
~~~



### 29. pve前端

guidesection中的isClientAction，如果涉及到战吼状态，就应该置为1， 完全让前端去判断是否按照要求做了

失败弹框：ShowAdventureFail();

章节：PVEChapterPanel

pve胜利界面：ResultShowAdapter.ShowAdventureCloseUp(msg, this);]

pve投降结算：ShowAdventureCloseUp

pve点击投降：ServerSurrender

### 28.日志系统

经验：1.查uds战绩未结算， 先去找到大致发生时间， 然后grep时间看uds的完整日志，看到了panic

#### docker直接看9001-9005

#### 所有日志的地方

过滤时间：T03:59

战斗服：BattleRoyaleServer

~~~
docker ps -aqf "name=dev5-project328-battle-royale-server" 查看有哪些dockerid
~~~

lbs:   RankBoardService

uds：UserDataService（success deal:UdsAddUserGameHistory）

328:HallServer

broadcast/consumer：HoxiBroadcaster/HoxiConsumer

director：Director

Dev5：172.17.0.1_dev5

taptap服务器：172.24.106.255_taptap231001 

联赛服：k8s_test

taptap服：k8s_taptap

grep

~~~
搜索不包含一下任一字段的内容 grep -vE 'SelectHero|InBattle|IsMatching|WaitingForPreparation|InTeam|Watching|InAdventureBattle'   不要-v就是包含任一字段的意思
~~~

实时看某个文件的日志

~~~
tail -n 10 -f data.b60b4195543d60247dca3e117fc9b12af.log | grep RankBoardService
~~~



### 27. 兜底提示前端

guideprompt表， 用事件(比如GlobalGuideEventType.OnOperationStart)去触发某些效果

### 26. 打包相关

海外包：内测用/海外测试google play打包
taptap：外部体验客户端/Android外部预览包

ASMDEF文件主要用于管理脚本文件，并定义程序集之间的依赖关系，以及对编译设置进行精细控制。Manifest文件则用于管理Unity Package，定义其属性和依赖关系，以及描述所需资源和文件。ASMDEF文件用于组织和编译一组脚本，而Manifest文件用于定义Unity Package及其属性和依赖项。

### 25. gm前端

在src/components/App.jsx中写布局信息(会引用到路由信息，也就是指向一个具体执行函数的地址)， 写 src/index.jsx下写路由信息， 

### 25.guidepromot 重连问题

客户端战斗阶段重新连接处理：BRBattleSceneManager.c s的ReconnectBROperation可以看到

### 24. 本地化

**客户端弹框：**

1. CommonPopupWindow.Instance.ShowToRestart(LocaleManager.GetText("报错信息_更新_游戏更新失败", "游戏更新失败"));

本地化配置中英文都在 LocaleConfig.xlsx里面

1. 如果是在Locale.xlsx里面的修改，先去点下ci会生成内容到待翻译的文本， 然后由LAQ翻译好后再导出就会到LocaleConfig.xlsx

![image-20231010132608946](/Users/a123/Library/Application Support/typora-user-images/image-20231010132608946.png)

2. 如果是添加文本直接在客户端的LocalLanguage.txt加了ci，就会生成到所有.csv中， 如果中途需要修改**中文文本内容(而不是key)**，直接在程序赋值使用.csv修改后(这个时候客户端的LocalLanguage.txt对应的中文翻译和你在程序赋值使用.csv修改的中文翻译应该是不一样的，以程序赋值使用.csv里面的为准，因为本来**一开始**也是根据客户端的LocalLanguage.txt生成了一个相同的中文翻译的)， 点ci会进一步到待翻译的文本.csv, 让LQA翻译即可 , 翻译好之后需要剪切到翻译好的文本中去再次点ci即可。(如果翻译好的文本.csv和程序赋值使用的key一致但中文文本不一致，以翻译好的文本.csv为准， 但最终在LocaleConfig.xlsx中的位置取决于程序赋值使用.csv的位置)

    客户端的**LocalLanguage.txt本质是为了直接在客户端操作，减少直接操作程序赋值使用.csv**
    
3. 中途在程序赋值使用.csv中修改了中文，那必须也把对应的英文也得翻译一下，不然配表中看不到新的英文， 如果你直接在翻译好的文本.csv中加上新的中文对应的新的英文翻译， 去点ci的时候就不会生成到待翻译的文本.csv中了

### 23. 战绩拉取优化

游戏类型(gamemode)， 锦标赛：1020100， 排位赛：1010100， 匹配赛：1010200

mongo保存快照，mysql只做简单信息存储

玩家每一条战绩的时间是自己结束游戏的时间，没结束的时候就会有一个自己的排名

查看战绩详情流程：**核心是mysql的recordid（也就是主键id）对应mongo的记录id**
 mongo记录玩家自己的快照信息以及历史记录的recordId(对应mysql的历史记录表的id)，如果点击查看详情，就会立马给大厅服发送请求GetBattleResultMsg， 通过recordid可以知道gameid， 同一局游戏玩家记录的gameid是相同的，筛选出gameid相同的记录，依次用这些记录的recordid去mongo里面拿到这局游戏的所有玩家快照，再通过rank进行一个排序，返回给客户端进行展示

个人主页信息都是uds通过buildStatisticsInfo得到的，游戏历史记录也是，通过mysql的UserGameHistoryRecord表得到所有记录，用这些记录的主键id也就是上边说的recordid去mongo里面拿数据再返回给客户端

玩家历史游戏记录存在于PersonalGameHistory

展示结算页面(其实战绩的详情页也是打开了结算页)ResultPanel

最后一名结束游戏的时候会在gamehistoryrecord中记录这场战斗， 然后通过gameid把这场战斗所有玩家的战绩的缓存给删了， 下次玩家打开主页的时候就能拿到最新的信息， 就不会显示**进行中**了, **注意**：战绩相关的有两张表GameHistoryRecord和UserGameHistoryRecord,其实还有一个MongoUserGameHistoryRecord暂未细看

个人主页的战绩连胜是前端自己算的， 用了一个栈，根据拿到的战绩数据的rank自己算出来的，没有区分模式

### 22. 开宝箱

ShopLottery_Open_Multi修改宝箱抽奖次数

更新周进度(所有定时刷新的都是前端发的)：1.前端发*MESSAGE_TYPE_REFRESH_LOTTERY_WEEK_AWARD*， 2. 登录的时候去刷新RefreshLotteryWeekAward

 领取周奖励：GetLotteryWeekAward

uds流程：BuyShopData-->case pb.SaleType_GoodItem-->buyShopGoods（如果是宝箱直接看 if isChest）-->OpenLotteryUtil(shopGoods表的的goods字段的第二个值作为参数，开宝箱现在是1)-->OpenLottery（目前lotteryId 为1）-->genLibraries(通过lotteryId（1）去LotteryMain表拿更详细的信息，根据一定规则生成获得的奖励库id(LotteryMain表的awardLibrary字段的第一个字段)<==>对应的是LotteryAward的library)

宝箱配表ShopGoods，98(单抽), 99(十抽) ，goods字段的第二个值为lotteryID， 这个lotteryID对应的是LotteryMain的id

皮肤碎片是 1:3:数量 （*RewardType_Currency*:*helper.CurrencyIDFragment*:数量）

手选卡是 2:9:数量（*RewardType_Item*：BagItemId_BagItemId_HeroSelectCard:数量

石灵碎片：10:-2/-3:数量（*RewardType_MinionStone*， -2表示紫色， -3橙色

宝箱周奖励表：LotteryWeekAward.xlsx, 周奖励进度是user表的lottery_week_award_count，周奖励已经领取进度是user的finish_lottery_week_award_id_list_str

10连抽的逻辑，后端会优先走消耗9个钥匙的逻辑，如果够的话就把需要消耗的钻石设为0，如果不够，就去尝试用钻石

开宝箱奖励如果是皮肤碎片：12|601|3， 12对应gift，601对应giftbag.xlsx,

### 21.前端记录

获取战斗服的链接

~~~c#
 var battleSer = new BattleRoyaleMsgMgrAdapter();
            _msgMgr = battleSer;
            _msgMgr.RegisterPushMessage<BR_OperationStart>(MessageType.MESSAGE_TYPE_BR_OPERATION_START,
                EnterBattleScene);
~~~



好友：1.loadingUtils 好友增删消息监听 2.MateInfo: 好友状态相关  3.好友申请界面ApplicationView 4.单个好友申请对象ApplicationCard
个人主页: personalPagePanel.cs(战绩以及个人主页信息)

邮箱处理：mailPanel.cs

音乐音效类型：audioPathConst.cs

游戏内倒计时：countdownbar(RefreshSecond)

展示4个英雄槽位：firstpanel.    beforefanel是禁用种族那个界面

选择英雄倒计时: secondpanel.cs（点了firstpanel就会进这个）

前端也有不同的状态， 可能会在不同的状态注册相同的信息比如英雄选择界面也得注册**MESSAGE_TYPE_BR_OPERATION_START**，以及运营状态， 状态切换的时候记得取消注册UnRegisterPushMessage

前端收到**MESSAGE_TYPE_BR_GAME_START**进入选择英雄界面，这个消息里面带了选择游戏的时间戳**(60s)**

前端收到**MESSAGE_TYPE_MATCH_CONFIRM_DATA_CHANGE**表示有玩家点了确认加入房间按钮

获取某个panel对象：UIManager.Instance.GetPanelInstance<SecondPanel>().

运营阶段的左下角个人信息:PlayerDisplay

游戏过程中，进入下一回合更新画面信息：PlayerDisplayData()



### 20.好友系统

1.mysql表(friend_ships)，每条记录对应一个user_one和一个user_two。一条好友记录只用记录一条，不用交换记录两条

2.双方只会有一个人有未读消息，好友关系表记录未读的人，以及未读条数

### 19. 优化战斗服对uds的调用

一切的请求都围绕BattleRoyale对象，它继承了很多东西。

创建**BattleRoyale对象**的时候会记录每个玩家的信息(RecordTeamMember),建立映射关系， 然后进入到等待确认状态机。

玩家如果点击确认，就会调用join， 这里面就会去调用uds来初始化一个BattleRoyalePlayer对象, 并且会建立userID<-->roomID的映射

OnBeforeEndGame的最后一个参数是isSpecial ， 如果是正常失败或投降，它就是true， 如果是排位组队，ttf会根据队伍最后一名挂掉的玩家来计算， 最后的玩家挂的时候会去调用一同组队的玩家的OnBeforeEndGame，这个时候设置为false， 就只涉及uds的调用，由uds主动推送给组队的玩家

借鉴现在生成AI的TASK实现(BeginCalculateAISnapshot)，

很多地方难免需要调用uds， 比如今日首胜， 不可以避免的要拿到用户最新的任务数据判断一下

添加

### 18.机制bug

2.如果没有词条，就搜索：not found with key:Entry

特性构造器:GetAttributesFromArray

三连合金：CombineMinions

Todo: 石灵maxlevel 。2。老号pve任务加载有问题。 3.老夫子bug

在Battle.cs里面的atk.Begin进行一次攻击的计算，两个石灵伤害的扣除在DamageMinion(有两个， 第一个是被打的，第二个是自己受反击)

2. 触发事件的时候有一个是判断函数JudgerFunc(与它相关的是TriggerHelper.cs)，另一个是选择目标函数SelectorFunc(若是石灵则TargetHelper.cs和若是玩家则playerTargetHelper.cs)

### 17.pve局外新手相关

#### 17.1 任务系统

应该思考，任务表里边的任务的可复用性

~~~
case1:皮肤对应的任务，不可能重复，那就不用额外增加skin字段标识
case2:活动任务可能复用，就得有额外的标志
~~~



任务类型

~~~
搜PlayerTaskType看任务类型，
1.通行证里面的: 1.1.挑战是活动任务2.1.任务是每日任务
2.主界面的: 2.1.挑战是新手任务
3.还有就是pve任务
~~~

涉及的表

~~~
最终都会指向playerTask.xlsx
1.新手任务(也就是现在的主界面的那个挑战任务)在newhandtask.xsls,
2.全胜任务在328的AddNewHandTaskGroup可以看到，
3.每日任务在dailytask.xsls中, 
4.周奖励任务在weektaskreward.xsls
~~~

1. 只要是单个任务领奖都走get-task-reward(章节奖励，全胜奖励，周宝箱奖励也是)， 在getRewardMsg根据类型区分领取不同奖励
2. 作弊码设置任务状态走：set-user-task-stat
3. **一键领取挑战奖励**：get-all-bp-tasks-reward(获取所有通行证里面的挑战奖励(它只会发章节的任务id，然后把这个章节下的所有任务领取了，在判断章节奖励是否可以领取)， 
4. **一键领取任务奖励**(获取所有通行证里面的任务模块的奖励)：get-all-week-daily-tasks-reward
5. 添加章节任务addNewBPChapterTask，再添加章节内的普通任务addNewBPNormalTask
6. 普通任务领奖时，会更新关联父任务UpdateParentTaskProgress
7. 刷新所有任务数据(新号也会走，我主要关注pve任务)
   userdataservice.GetUDSInstance().UpdateNewPlayerTask(user.ID)， 新手任务AddAllNewHandTaskGroup()
8. 通行证里面的挑战对应的活跃度宝箱是根据宝箱对应的任务的progress字段， 简单来说就是每领取这个界面的每日奖励，就会更新一下这些宝箱对应的任务的progress进度，这几个宝箱的slot字段不一样，分别是1，2，3，4，5， 比如第五个宝箱，在数据库的记录是cfg_id:5, type:12, progress:30, 这个五个宝箱的任务挨的比较近可以观察确认
9. 通行证的任务， 第二天更新的时候会实时通知，会发送NOTIFY_UPDATE_PLAYER_TASK

10. 通行证里面的任务模块有个刷新时间， 这个依赖于后端的addTaskRefreshTimeStamp来给返回的userdata添加上刷新时间

11. 任务类型PlayerTaskType，周任务(通行证里面的圆圈任务宝箱)是*PlayerTaskType_PT_Week*，奖励在weektaskreward表

uds：更改任务状态UpdateTaskStats

Project328:在CheckRefreshTask检查生成所有的任务， addNewBPTask添加战令任务,checkDailyTask添加每日任务(进一步使用AddTaskFromConfig去uds)



**使用toMinion或者toSnapShot可以保留一个状态信息， 简单说就是new了一个对象来存储信息， 就不用担心存储的是c#的指针对象。**

#### 17.2 pve加载章节

有关系的表， pvemap.xlsx, pvechapter.xlsx

~~~

1.328启动的时候会去checkAdventureTask
2.pve不存在锁章节的逻辑， 只要加载了就不会再上锁了，加载的时候就把任务都加上了
3.添加玩家的冒险信息，AddPveInfo(
~~~

### 16.bug

3. 战斗服获取userdata字段比如UserData.**ActivityInfoFieldNumber**， 实际上发送的是activityInfo而不是ActivityInfo

2. 血量变化:直接grep玩家的id，然后搜update player 62742 hp to 25, damage: 9

1. 总击杀人数不对， creategame的时候如果对方已经挂掉了，就会去新建一个player对象放进去，bug的原因是这个新建的player加载了玩家活着的时候最后一次运营结束的数据(hp > 0)， 所以即使击杀了死亡的玩家也算了人头数(搜if (g.P2 == null))

2. 模拟战斗工具， game在一开始运营就会创建，并且加入gamelist， game里面有一个curbattle会在计算战斗的时候创建，并且会连接在game上， 所以不要轻易改动玩家的game对象

3. 组队相关：邀请好友会先去328然后回到被邀请者的客户端MESSAGE_TYPE_TEAM_JOIN_REQUEST， 被邀请者的响应为MESSAGE_TYPE_TEAM_JOIN_RESPONSE。

4. 匹配成功会客户端会收到OnMatchFinish，进一步打开确认UI，这些相关的操作可以在客户端的Scripts/Battleroyale/Team下看到

5. 孔苏bug： 投降玩家也会有还原玩家数据的步骤，需要注意player转储

6. 蝉， 主要是光环问题， aura对象，里面会构建一些东西，比如GetBuffAtk和GetBuffHp，根据配表的值配合CreateGetValueFuncFromStr得到需要增加的攻击和生命， 这里面有一个BeginReceiveAuraFlag控制是否开始接收光环效果， 每次去获取石灵的atk，就会遍历玩家的光环列表auraList，比如蝉的光环列表就有加攻击和生命，所以在auraList.Add(aura)，add的那一刻就会给商店的石灵加上攻击和生命, 但BeginReceiveAuraFlag加完攻击和生命之后就会置为false，当你买石灵的时候回去遍历玩家光环，这个时候会把光环的效果转化为实际的buff，获取石灵的Attack时，虽然BeginReceiveAuraFlag为false，但buff里面有数据了，就会从buff里面给石灵加上永久的增效， 所以本质上还是通过buff加的，光环只是临时的效果

7. 华鹿bug， 游戏技能可以在playerskill中看到技能类型， 可以在BuildPlayerSkill看到加载词条的过程， 不管是什么类型如果有词条都会加载（如果不是被动类型的技能是通过ActiveSkill构造基类ActiveEntity加载的词条）。
   如果是普通的词条判断是否可用在EntryBuilder.cs中的judgeFunc判断， 而如果是技能类型的词条(被动技能除外)判断是否可用在ActiveEntity中， 但实质 [🤔need.md](🤔need.md) 上他们都是同一个委托类型(EntryImplementation.JudgerFunc), 具体的判断函数都是用的TriggerHelper.cs中的函数。

   一句话：前端显示英雄技能是否可用，每次执行一个操作都会去ExecutePlayerAction，然后会检测是否需要播放动画， 这里面就包含了前端技能状态的更新，调用了CheckCanUse来检测这个技能是否可用

   不是只有targethelper和triggerhelper， 比如NumberCompare(MyGoldMinionCount,NoSmallerThan,6)，MyGoldMinionCount会去**CreateGetValueFuncFromStr(用的很多，通过字符串获取用户某个属性经常用)**中，进一步使用GetNumWithCounter利用反射去遍历configCountUtils.cs里面的计数函数。

8. pve回溯，玩家的计数器没有清空， 用到了GetSummonPlainMinionNumOneRoundFromPlayerCounter，这里面的Get和FromPlayerCounter是固定的

9. 战绩异常，先去查这个game_id，看是否有8条数据， 大概率是ai保存战绩的时候出了问题

10. 同时开了两场对局， 战斗服成功开始游戏的时候会有一个match_success埋点， 游戏结束是match_result

### 15.工具

- gm-tool发邮件奖励的时候会走AddSingleRewardToUser

- 单独重启gm-tool工具

~~~
/data/build-server/src 可以看到deploy-gm-tool-agent.sh, 第一个参数表示分支
比如deploy-gm-tool-agent.sh dev
~~~

- 活动管理工具

~~~
1.活动名，2.活动类型，3.活动id，2.是否禁止活动， 3. 活动开始时间， 4.活动截止时间

~~~



AI模拟战斗：

~~~
核心思路，不管一回合打了多少人以及多少回合， 都先只做记录（记录到下边三个数据结构中）， 并没有区分是因为打谁造成或受到的伤害。

全部模拟逻辑：
1.先InitMirrors中初始化，用镜像得到player对象(核心是MirrorId字段)，并且会检查一下这些镜像是否正常(使用loadsnapshot)，如果正常会放到AiHeroMirrorPlayers对象中.
2.三个重要结构：AiPoolBattleWinRecords(每回合的总战斗次数和赢的次数)，AiPoolBattleDamageRecords(每回合造成的总伤害)，AiPoolBattleBeDamageRecords(每回合受的总伤)
~~~





最小二乘拟合直线: 就是给一些坐标点，求一条坐标上的直线，使得这条直线到这些点的总和最小

~~~
最小二乘法的目标是找到一条直线，使得数据点到直线的距离之和最小，也就是使得拟合误差最小化。具体做法是通过最小化平方误差和来估计直线的斜率和截距。

在最小二乘拟合直线中，斜率代表直线的倾斜程度，而截距则是直线与y轴的交点。拟合直线的斜率和截距可以用来描述两个变量之间的关系。斜率反映了自变量单位变化时因变量的变化情况，而截距表示在自变量为0时因变量的取值。

最小二乘法广泛应用于许多领域，包括回归分析、数据拟合和线性模型等。它为数据建模提供了一种简单而有效的方法，并且在许多情况下可以提供可靠的结果
~~~

方差

~~~
方差可以用来判断数据的离散程度。如果方差较小，表示数据点较集中；如果方差较大，表示数据点较分散。
方差的数学表达式为：Var(X) = Σ[(X - μ)^2] / N    μ是平均值的意思
~~~



CustomCmdRegister.cs的Start()可以看到大部分客户端作弊码的命令显示行(搜索DebugLogConsole.AddCommand添加debug命令)

修改商店石灵的种族：
获取种族名字:
foreach (var minionRaceName in Gen.GetMinionRaceNameMap().Values) =

snapshot最好还是在pb里写一份， tmd在外边如果是list类型，一直被优化成同一个对象了。

-  锦标赛灵宝获取debug

### 14.pve对局

pvetaskmanager.cs的UpdateWatch可能有坑

SetGuideTaskRecord可能有坑

~~~
对局流程：EnterRoomHandler.cs的Process接收进入房间的信息，接着GetOrCreateBattle创建战斗，接着调用CreateBattle，接着创建BattleRoyale，BattleRoyale的构造函数如果是guide模式会先把玩家自己的信息加载好(join一下)，并且把GuidePlayerSelf设置为自己, 同时把guideoperation的状态机初始化
设置好之后调用battle.Start()，如果是guide进一步调用StartGuideBattleRoyale，接着调用base.Start切换状态再gameThread.ChangeState(initStateID)，这步实质上是在gameThread里加了一条消息，然后调用gameThread.Start去处理消息，实际处理消息的时候在去调用具体状态里面的OnEnter函数
~~~



1.在AIReactionService.cs可以看到AI的一些数据更新操作

2.前端接收战斗服的数据

3.AI数据来自RequestAi(因为使用AIOperationEndSnapShot保存返回结果，最后进战斗的时候会加载这个快照)， 在WaitAI()中做最后的处理(修改AIOperationEndSnapShot)，这里面还可以**更改玩家最后进入战斗时的数据**

4.进入房间的消息是在EnterRoomHandler.cs，之后也去这里拓展冒险模式

5.消息接收以及处理说在GameThread.cs里面做的： 增加消息Add, 处理消息HandleMessage

6.游戏状态机GameStateMachine(里面有一个)，所有的状态类都是继承自AbstractGameState，状态类型搜索StateConstant，子类只要继承这些状态类，并且加入状态机，切换状态的时候就去的是这些子类，因为类型id是继承下来的，不需要在状态类型StateConstan中额外增加子类的类型id

7.RestorePlayerAfterBattle做**数据转存**(如果是修改了snapshot里面的数据，需要在这里restore一下，不然数据会丢失)。

8.运营模式发送BR_OperationStart消息有两个情况
1.SendOperationStageStartMsg*//* 玩家运营阶段开始
2.BuildEndMessage

9.对于引导模式，客户端的EnterFromOperation表示根据战斗服传过来的运营消息进入房间，播放完动画后会给战斗服发送Guide_ClientReport，战斗服再进一步推进guide

10.客户端创建引导模式房间CreateBRGuideRoom->GoToBRGuideScene->EnterFromOperation(根据服务器传过来的运营结束开始消息BR_OperationStart触发的)

11.所有的action比如cheataction都继承于playeroperationaction，所有的action都在player.cs中，去里面搜索CheatCmd可以看到作弊码相关的，**具体处理是ExecutePlayerAction函数**(先执行Perform，就去到了playeroperationaction，根据不同的action去执行不同的execute函数)

11.1 各种effet和action的关系， action中可能会初始化一个effet对象的实例，进一步执行里面的perform函数，还有另外一种方式可以在effect.xlsx中直接配置，但effet的类必须要写明标签，将effet表内容通过标签转为具体的effet实例可以看gametask.cs中的BuildLogic函数,  会把这些转换出来的实例都执行一下(effect.Run)
11.2 各种游戏事件都在GameEvent.cs中， action驱动事件，一种是之前使用getmion作弊码时的显示增加事件，还有一种是在effect表中配置，比如刷新商店的时候去显示的使用TriggerEvent遍历一下entry表中的所有刷新商店才会触发的事件。

12.去HandDeck.cs和BoardDeck.cs可以看到对手牌和战场牌的操作

13.每次战斗结束的时候，需要还原玩家战斗前运营结束的时候的快照数据，在RestorePlayerAfterBattle还原玩家之前的数据(比如战场石灵)

14.在BattleRoyalePlayer.cs中的ProcessOperation可以看到**处理一条action消息的流程**

15.如果想在局内操作别的玩家的数据，可以看看OperationState.cs的OnRequest函数

16.客户端一个执行命令完整流程， 比如现在在运营状态，客户端SendOperation，这个消息会去到OperationState.cs的OnRequest函数(在这里可以改变userid让不同的玩家执行)， 接着通过消息里面的userid把这条消息放到这个带处理消息的字典中(MessageDict)，接着遍历字典，调用ProcessOperation处理具体的消息(比如作弊就回在这里创一个CheatAction对象，后边会调用里面的Execute),  最后所有消息都会去统一的接口ExecutePlayerAction，这个接口会返回OperationResult，里面就包含了本次客户端执行操作的返回(**动画和操作结果**)

17.所有客户端的action行为，服务器通过MESSAGE_TYPE_OPERATION_RESULT返回结果，客户端根据返回结果播放此次操作动画。

18.客户端创建房间CreateBRGuideRoomAsync, 客户端是在收到运营开始消息的时候去播放的上一回合计算出来的**战斗结果动画**的，ReceiveBRNewRoundMsg可以看到播放动画逻辑如果startMsg.LastBattleData(服务器有两种情况会返回这个消息，如果还有下一回合，那么就在下一个运营开始时才发，如果是最后一个回合，比如引导模式，直接在Guide_BREndMsg发) 不为空，则播放上一场战斗动画,

19.player.cs的RecordedMinionData保存着战斗开始前board上的石灵信息

20.MinionUtils.cs  7AI的任务所需数据就来自这里的CreateGetValueFuncFromStr, 在PVEJudgeState.xlsx里面

21.**加载石灵**是在convertdata.cs的ToMinion根据不同位置加载的，遍历snapShot.AllEntities的时候可以看到, 所以如果要设置一个玩家的状态可以使用LoadSnapShot，因为这里面已经有了AllEntities，

22.玩家的playerskill控制着必刷石灵， 通关hero表配置playerskill，在playerskill表找到mainentries对应着entry.xlsx表，里面每一条对应着一个触发条件(程序上：从LoadAdventurePlayerStat的AddPlayerSkill可以追下去），进一步找到effect的每一项

23.entry和effect并不是所有的都会触发，在GetAllListeningEntries可以看到玩家需要加载的词条有哪些(战场石灵，英雄技能...)

24.1 词条加载流程(LoadEntries函数)， 每一个实体比如技能或者石灵，它们都继承了IEntryOwner，所以它们都各自拥有一个词条记录器(事件类型对应OperationEntry链表，OperationEntry是在LoadSkillFromData中构建出来的，主要包含了对应的entry表实例和判断是否满足触发条件的函数、selector选择器以及对应的effects，接着调用AddToEventList将这些OperationEntry加入到词条记录器) 
24.2 词条触发流程，先TriggerEvent触发事件，再调用GetAllListeningEntries获取所有监听此事件的实体(minion，英雄技能等)对应的OperationEntry， 然后去遍历这些OperationEntry里面的effects即可(每一个effect调用run函数就会进一步去执行perform函数，可以参考作弊码cheataction进一步调用对应effct的例子)，

25. 在商店刷新前使用技能在player.PrepareMinionToShop加载好石灵， 然后真正刷新商店的时候先看看这里面有没有石灵
25. 战吼流程：上场的时候如果该石灵拥有战吼技能就播放战吼动画并且将该战吼石灵(player.BattleCryingMinion)， 过程中如果操作不规范就是退出战吼状态， 如果成功选择目标则把刚才记录的战吼石灵放上战场，并且对被选择的石灵添加属性。
25. 光环和entry的理解， 光环比如说蝉鸣谋士和猴长老， 光环更像是加的一种效果，但如果你移除蝉鸣谋士或猴长老效果就会消息， 而entry则是一种触发后无法取消的效果



### 13.resultcode整理与显示

1.比如领奖系统，在一开始设计的时候就该注意返回的时候多给个参数记录resultcode，这样在具体函数中处理的时候就可以根据出错原因直接设置resultcode而不局限于只是成功或者，如果出错并且没有设置指定resultcode，就统一用300错误码

2. 发奖励的时候记录一下该奖励的来源，方便埋点使用

客户端：BuildUnknownError  <====>. 服务器: result-code.go

客户端的LocaleManager.cs有一些默认的显示

比如“报错信息_UnknownError”，既在LocalLanguage.txt配置了中文 ，也在dicDefaultEN和dicDefaultCN单独配置了中英文

3. 前端弹框

~~~
1.1  CommonPopupWindow.Instance.Show_WithBtn("报错信息_网络_测试".ToLocal()); //简单弹框不重新连接
1.2.账号密码输入提示
CommonPopupWindow.Instance.ShowToast(LocaleManager.GetText("报错信息_注册_密码格式错误"), Toast.Mod.Negative);

报错信息_BuyShopGoodFailed
  CommonPopupWindow.Instance.DirectlyShowRestartPopup(                    InGameText.BuildUnknownError(result.ResultCode));
2.1需要重新连接
 CommonPopupWindow.Instance.DirectlyShowRestartPopup("报错信息_网络_长时间未操作".ToLocal());
或者
2.2CommonPopupWindow.Instance.ShowToRestart(LocaleManager.GetText("报错信息_更新_获取更新内容失败", "获取更新内容失败，请稍后重试"));
              

~~~



### 12.android包共存 

一开始就修改AndriodManifest.xml，把android:authorities改成对应的包名， 再去调用之前unity打包的命令

### 11.英雄皮肤

### 10. 跑马灯

石灵能量那个：13对应的是品质， 从大->小的配置， 比如13，12，11，后边的value就是3，3，3这样

**前端部分**

~~~~
1.发送MESSAGE_TYPE_MARQUEE_NORMAL
2.MQMsg.cs中可以看到文本的设置
~~~~



目前想法， 只放一个消息进kafka， 然后大厅服消费的时候根据类型决定是广播给所有在线玩家还是自己。

~~~
 message Marquee {
  uint64 uid = 1; //玩家id
  int32  type = 2; //触发消息类型
  MarqueeInfo Info = 3; //具体数据，按需采用
}
 //跑马灯具体信息
 message MarqueeInfo {
   int32  num = 1; //涉及数量都用它， x级，x个, x连胜等
   TeamType gameType = 2; //游戏模式
   int32   resonanceConfId = 3; //共鸣类型对应的confid
   int32 levelConfId = 4; //段位对应的confid
   int32 minionId = 5; //对应cardhero的id，从而获得品质
   float percent = 6; //超过百分比
 }
~~~



疑问：1.现在的广播逻辑， 是同一份消息，换了不同的to_user_id，一起放到kafka里面。
2.在哪里管理所有在线用户的

写到kafka的消息包含，1.uid，2，事件类型 3.广播范围（前三个类型固定），4.str串(包含所有其他信息)

主要看两个模块，consumer和broadcaster

生产消息：
在open-match-director/service/kafka里面可以看到初始化kafka的过程， 以及topic的选择，还有生产消息的过程（目前所有的消息类型都是BroadcastMessage(消息类型(message-type.go里面的)，对方id，和Message，还有所在大厅以及唯一消息id的标识，但此时还没设置，在消费的时候设置的，  真正的数据是Message成员)，然后进一步封装为(topic，key，data(也就是BroadcastMessage))放到kafka中）

消费者处理消息(从consumer->broadcaster->大厅服)：
在consumer库中消费消息的时候会先通过玩家id得到玩家所在的大厅服ip信息，然后通过post方法（最多尝试5次）把消息(同样是BroadcastMessage，同时里面添加了玩家所在大厅服的信息)转发给broadcaster库的Send函数，进一步调用broadcaster库的forwarder模块进行广播(去看broadcaster库main函数，消息会通过Send函数放入chanel中最后会由forwarder模块的manager进行转发)，通过MsgBroadcast消息发送到大厅服进行处理，所有的广播消息都通过大厅服的connMgr.Run()去处理
在这个过程中，**如果都成功了的话， 大厅服会回复MessageAck给broadcaster告诉它成功了，然后broadcaster会回复返回码200告诉consumer成功了。** 如果broadcaster那里一直不成功最多重传10次，不管是否成功都会返回200告诉consumer成功了



### 2.赛季需求

4.现在改为了再用到的时候才会去创建对应的表可以看getByConfId



3.石灵收藏

搜MinMinionLevel配置石灵最小等级

DamageUtils.cs：175 计算伤害，判断石灵击杀情况

Minion.cs： 106*//* 随从信息，可以看到本回合杀死的随从数量

ConvertData.cs负责protobuf和内存数据之间的相互转换*

HistoryRecord.cs 一整局游戏所有回合数据记录

RoundRecord.cs 回合记录的具体类

1.一键升级：遍历一下当前石灵列表即可

2.总等级达到一定值领奖， 参考天梯之路领奖方式

3.石灵羁绊：当前有14种羁绊， 就给个切片， 给石灵添加一个类型字段， 每次提升石灵等级的时候都先通过该石灵类型得到大致范围，从conf中得到数据，去玩家当前拥有的石灵中做对比，记录该羁绊所涉及石灵的最小值。 每一种羁绊都记录一个最高达到的领奖位置和当前最新的位置。



判断是否包含

~~~
	"github.com/elliotchance/pie/v2"
	pie.Contains
~~~



set集合	

```go
 mapset "github.com/deckarep/golang-set/v2"   	  
var normalTaskTypes = mapset.NewSet(
	pb.PlayerTaskType_PT_Daily,
)
normalTaskTypes.Contains(...什么什么)
```

​	 

4.石灵击杀数，战斗服中用哈希表记录.

~~~c#
  //更新石灵击杀累积计数
                Dictionary<int, ulong> killCount = new Dictionary<int, ulong>();
                Dictionary<int, RoundRecord>.ValueCollection valueColl = Player.History.GetAllRoundRecord().Values;
                foreach (var tmp in valueColl) {
                    foreach (var rec in tmp.GetEventRecordsByType(EventRecordType.DeathMinion))
                    {
                        var record = (BattleDeathMinionRecord)rec;
                        if (record.MinionData.PlayerID != record.From_MinionData.PlayerID)
                        {
                            //只要不是自杀的，就记录一下
                            killCount[record.From_MinionData.ConfigID] += (ulong)record.MinionData.KillMinionCount;
                        }
                    }
                }
                UpdateMinionKillCount(killCount);
~~~

~~~c#
 private void UpdateMinionKillCount(Dictionary<int, ulong> killCount)
        {
            var killList = new List<SingleMinionKillCount>();

            foreach (KeyValuePair<int, ulong> kvp in killCount)
            {
                killList.Add(new SingleMinionKillCount()
                {
                    MinionID = kvp.Key,
                    Killcount = kvp.Value,
                });
            }

            if (UserDataService.UpdateMinionKillCount(Player.ID, killList) != ResultCode.RESULT_CODE_SUCCESS)
            {
                LogHelper.WriteLog($"累加玩家{Player.ID}石灵击杀数失败");
            }
        }
~~~

~~~c#
public static int UpdateMinionKillCount(ulong ID, List<SingleMinionKillCount> killList)
        {
            var sendinfo = new UpdateMinionKillCount();
            sendinfo.MiniKillInfo.AddRange(killList);
            sendinfo.UserID = ID;
            return Request<UpdateMinionKillCount, SimpleBackMsg>("update-minion-killcount", sendinfo).ResultCode;
        }
~~~

4. 首充

前端部分：

要用gen就需要先using conf;

去进游戏之前的位置更新红点(LoginProcessVm.cs)：RedPointManager.Instance.DataChange(RedPointConstant.**FirstPay**, PlayerDataManager.instance.UserData);



~~~
MainSceneManager里面的MainCanvas可以找到具体的某一个按钮

LoginProcessVm.cs中可以看到第一次登陆返回的消息

把MainCanvas中的按钮拖动到Pannel上即可实现绑定。

如果MainSceneManager报错就点击右键reimport一下

预置体都在Assets/Prefabs/NewUI/MainScene里面

预置体对应的脚本都在Assets/Scripts/UI/MainScene里面，或者不要这个UI

在mainscenemanager中有一个uimanager.cs 一开始就初始化了所有子panel的init函数， 所以可以在不同的panel中监听别的panel里面的按钮
~~~

打开最外层panel的时候可以看到btnOpen.onClick.AddListener(() => UIManager.Instance.OpenUI<FirstPayPanel>()); 实质上是去了FirstPayPanel类的OnOpen函数，再进一步调用gameObject.SetActive(true)。 再打开别的panel的时候， 比如bp那里，BattlePassBuyAdvanced.Open(), 本质上就是去BattlePassBuyAdvanced这个子panel里面调用gameObject.SetActive(true)。

后端部分：

purchase.go的Purchase：看付款买商品逻辑

在BattlePassBuyAdvanced.cs的Buy可以看到战令进阶路线购买过程，里面的IAPTools.Instance.Buy是**实际支付**的过程，支付如果成功了的话，会有一个receipt(收据)， 客户端把收据发到服务器做对比看是否正确。





4.点卷

后端：

清除购买的商品数据

在表清除购买记录

delete  from buy_records where user_id = 10003;

delete  from user_shop_gift_bag_data where user_id = 10003;

delete  from user_shop_item_data where user_id = 10003;

redis清除shop_data缓存



![](/Users/jiangtao/Library/Application Support/typora-user-images/image-20230611211149852.png)

1.用真钱买金块的渠道是固定的吗，有没有买任意金额的金块的渠道

2.只能先通过现金转点卷吗再买东西吗，还有没有别的东西是需要现金买的

3.是不是也得给首充配置一下，配置到哪个表

4.![](/Users/jiangtao/Library/Application Support/typora-user-images/image-20230611205915481.png)

这个是根据什么决定的

5.现在法币的类型是99， 直接用这个字段当人民币的类型，需要特殊处理吗

6.之前好像没有用钱买钻石的逻辑。





前置礼包：纯后端

在商店里购买东西，不管是买礼包，或者充钱，都会传到服务器一个type， 根据这个type判断是买的什么。对于礼包，目前只用的上SaleType_Gift这种类型，涉及到的配置表就是ShopGift， 一个用户对应的**可买**的礼包信息在UserShopGiftBagData数据库表里面



新手引导：

1.战斗服：CalculateState.cs(Battle.SendGuideEndMsg每一回合同步信息给客户端)， GuideProgressRecorder.cs，ServerGuideManager.cs(SyncClientProgress很重要，同步guide进度给客户端, 118行UpdateOperationProgress更新guide)



2.在GuideBattleRoyale76行看看，目的说一开始附上初值GuideManager.ProgressRecorder.RoundFaildInfo = GuidePlayerSelf.UserData.FinishGuideInfo.FaildInfo;

3. Bug, 在BattleRoyale.cs，BuildEndMessage报错



### 7Ai多回合引导

IsAllGuideOver代表的是一个guideId的内容

新手引导的前4个回合，没有guideid++的时候，在每次进入新的引导回合的时候在GuideCurGuide.cs初始化下一次需要加载哪个guideid， 7AI的时候是个例外，7AI进入下一回合的时候不会去让guideid++

每次一个section完了之后(或者点击准备的时候,其实点了准备之后就已经计算出游戏结果了)都会收到一个OnRequest请求，在GuideOperationState.cs的UpdateOperationProgress会去加载下一个引导步骤，从StepNextGuide->LoadNextGuide(这里会判断是否所有引导都完成了)->LoadGuide, 引导信息准备完毕之后，在最顶层的OnRequest函数执行了SyncClientProgress将信息同步给客户端

进入LoadGuide有两条路径，一个就是初始化(新的一回合开始或者刚进7AI)的时候，另外一个就是当一个section完了之后调用LoadNextGuide的时候

在GuideBattleRoyale.cs和ServerGuideManager.cs这两个地方都需要初始化新手引导相关信息比如**失败信息(FaildInfo)**。

1.BattleRoyaleManager.cs的CreateBattle创建房间，会有以下3个操作

~~~
1.BattleRoyale.cs的if (msg.Type == RoomType.Guide)处进入

2.在GuideBattleRoyale.cs的InitGuideBattleRoyale

3.到ServerGuideManager.cs的Init函数 (核心)
~~~

2.BattleRoyaleManager.cs的battle.Start()进入战斗



**拿不到用户信息就直接player.UserData = UserDataService.GetUserData(player.ID);**

### 8.随机英雄控制

BattleRoyale.cs的GenNormalSelectHeroList中选择英雄



1.新加一个数据库表，存储三种不同模式的最近选择英雄， 放到heroinfo结构中。

### 9.7AI支持非8人

登陆注册过程在MainScreenSceneManager.cs中

client

~~~
while (_FakePlayers.Count < guideState.GameList.Count)
{
       _FakePlayers.Add(new TeamPlayerView());
}

7AI点击匹配的时候，出来的头像数说根据主场景的GuideForceMatchCanva下的PlayerContainer的个数决定的。
~~~

战斗服没什么好改的，改下读配置方式

1.天梯之路



