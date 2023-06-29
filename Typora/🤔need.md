1.天梯之路

2.赛季需求



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

战斗服：

