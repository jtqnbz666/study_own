

QA: 1.新英雄必出现 2. 皮肤体验卡使用成功没有 .3游戏历史数据可能影响， 4.每日首胜, 2.死亡的时候好友列表是否正常 56.游戏历史记录

自测过的内容，天梯分变化，战绩、结算奖励(显示发奖与实际货币增长相符)， 首选卡数量， 最近系列英雄排序。

历史数据如果出问题就拆出来

当前已处理的调用：

gamehistory里面：**Setuserguideround**， 把体验控制合并到里面计划

BeforeEndGame之前返回的是reward，太丑了，应该把奖励作为参数

更新天梯分函数返回一个待准备消息

首选卡会阻塞游戏，不能合并

观战要改异步吗，观战是个问题， 也会跟uds交互,  不管还是搞成异步， 合并也不好合并

### 30. 镜像

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

### 28.日志系统

#### docker直接看9001-9005

docker ps -aqf "name=dev5-project328-battle-royale-server" 查看有哪些dockerid

#### 所有日志的地方

看consumer：

~~~
cat data.20231013.log | grep HoxiConsumer |grep k8s_test | grep 10360
~~~

看uds：

~~~
cat data.20231013.log | grep UserDataService | grep 设置玩家战斗状态 | grep 10130
~~~



战斗服：BattleRoyaleServer

uds：UserDataService

328:HallServer

broadcast/consumer：HoxiBroadcaster/HoxiConsumer

director：Director

Dev5：172.17.0.1_dev5

taptap服务器：172.24.106.255_taptap231001 

联赛服：k8s_test

grep

~~~
搜索不包含一下任一字段的内容 grep -vE 'SelectHero|InBattle|IsMatching|WaitingForPreparation|InTeam|Watching|InAdventureBattle'   不要-v就是包含任一字段的意思
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

在客户端的LocalLanguage.txt中修改，点ci会自动同步到LocaleConfig.xlsx

本地化配置中英文都在 LocaleConfig.xlsx里面

如果是在Locale.xlsx里面的修改，先去点下ci会生成内容到待翻译的文本， 然后由LAQ翻译好后再导出就会到LocaleConfig.xlsx

![image-20231010132608946](/Users/a123/Library/Application Support/typora-user-images/image-20231010132608946.png)

如果是添加文本直接在客户端的LocalLanguage.txt加了ci，就会生成到所有.csv中， 如果中途需要修改文本内容，直接在程序赋值使用.csv修改后点ci会进一步到待翻译的文本.csv, 让LQA翻译即可

### 23. 战绩拉取优化

游戏类型(gamemode)， 锦标赛：1020100， 排位赛：1010100， 匹配赛：1010200

mongo保存快照，mysql只做简单信息存储

查看战绩详情流程：**核心是mysql的recordid（也就是主键id）对应mongo的记录id**
 mongo记录玩家自己的快照信息以及历史记录的recordId(对应mysql的历史记录表的id)，如果点击查看详情，就会立马给大厅服发送请求GetBattleResultMsg， 通过recordid可以知道gameid， 同一局游戏玩家记录的gameid是相同的，筛选出gameid相同的记录，依次用这些记录的recordid去mongo里面拿到这局游戏的所有玩家快照，再通过rank进行一个排序，返回给客户端进行展示

个人主页信息都是uds通过buildStatisticsInfo得到的，游戏历史记录也是，通过mysql的UserGameHistoryRecord表得到所有记录，用这些记录的主键id也就是上边说的recordid去mongo里面拿数据再返回给客户端

玩家历史游戏记录存在于PersonalGameHistory

展示结算页面(其实战绩的详情页也是打开了结算页)ResultPanel

### 22. 开宝箱

更新周进度：NotifyServerUpdateLottery

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

Todo: 石灵maxlevel 。2。老号pve任务加载有问题。 3.老夫子bug

在Battle.cs里面的atk.Begin进行一次攻击的计算，两个石灵伤害的扣除在DamageMinion(有两个， 第一个是被打的，第二个是自己受反击)

2. 触发事件的时候有一个是判断函数JudgerFunc(与它相关的是TriggerHelper.cs)，另一个是选择目标函数SelectorFunc(若是石灵则TargetHelper.cs和若是玩家则playerTargetHelper.cs)

### 17.pve局外新手相关

#### 17.1 任务系统

一键领取战令奖励：GetAllBPTasksReward

一键领取任务奖励：GetAllWeekDailyTasksReward

328也有MergeUserdata

uds：更改任务状态UpdateTaskStats

Project328:在CheckRefreshTask检查生成所有的任务， addNewBPTask添加战令任务,checkDailyTask添加每日任务(进一步使用AddTaskFromConfig去uds)

如何确定当前房间的模式， 构建BattleRoyale对象的时候可以看到， 
GmaeType枚举类型(Normal和Championship)表示游戏类型，只要不是锦标赛就都是Normal；
RoomMatchType枚举类型(Public和Private) 表示房间类型，锦标赛和排位是Public， 匹配是Private
TeamType枚举类型，也表示游戏类型，排位赛(PublicKnockout), 匹配赛(PrivateKnockout), 锦标赛(PublicChampionShip)

更新状态还没有处理（八人合并）

**使用toMinion或者toSnapShot可以保留一个状态信息， 简单说就是new了一个对象来存储信息， 就不用担心存储的是c#的指针对象。**

### 16.bug

1. 总击杀人数不对， creategame的时候如果对方已经挂掉了，就会去新建一个player对象放进去，bug的原因是这个新建的player加载了玩家活着的时候最后一次运营结束的数据(hp > 0)， 所以即使击杀了死亡的玩家也算了人头数

2. 模拟战斗工具， game在一开始运营就会创建，并且加入gamelist， game里面有一个curbattle会在计算战斗的时候创建，并且会连接在game上， 所以不要轻易改动玩家的game对象

3. 组队相关：邀请好友会先去328然后回到被邀请者的客户端MESSAGE_TYPE_TEAM_JOIN_REQUEST， 被邀请者的响应为MESSAGE_TYPE_TEAM_JOIN_RESPONSE。

4. 匹配成功会客户端会收到OnMatchFinish，进一步打开确认UI，这些相关的操作可以在客户端的Scripts/Battleroyale/Team下看到

5. 孔苏bug： 投降玩家也会有还原玩家数据的步骤，需要注意player转储

6. 蝉， 主要是光环问题， aura对象，里面会构建一些东西，比如GetBuffAtk和GetBuffHp，根据配表的值配合CreateGetValueFuncFromStr得到需要增加的攻击和生命， 这里面有一个BeginReceiveAuraFlag控制是否开始接收光环效果， 每次去获取石灵的atk，就会遍历玩家的光环列表auraList，比如蝉的光环列表就有加攻击和生命，所以在auraList.Add(aura)，add的那一刻就会给商店的石灵加上攻击和生命, 但BeginReceiveAuraFlag加完攻击和生命之后就会置为false，当你买石灵的时候回去遍历玩家光环，这个时候会把光环的效果转化为实际的buff，获取石灵的Attack时，虽然BeginReceiveAuraFlag为false，但buff里面有数据了，就会从buff里面给石灵加上永久的增效， 所以本质上还是通过buff加的，光环只是临时的效果

7. 华鹿bug， 游戏技能可以在playerskill中看到技能类型， 可以在BuildPlayerSkill看到加载词条的过程， 不管是什么类型如果有词条都会加载（如果不是被动类型的技能是通过ActiveSkill构造基类ActiveEntity加载的词条）。
   如果是普通的词条判断是否可用在EntryBuilder.cs中的judgeFunc判断， 而如果是技能类型的词条(被动技能除外)判断是否可用在ActiveEntity中， 但实质上他们都是同一个委托类型(EntryImplementation.JudgerFunc), 具体的判断函数都是用的TriggerHelper.cs中的函数。

   一句话：前端显示英雄技能是否可用，每次执行一个操作都会去ExecutePlayerAction，然后会检测是否需要播放动画， 这里面就包含了前端技能状态的更新，调用了CheckCanUse来检测这个技能是否可用

   不是只有targethelper和triggerhelper， 比如NumberCompare(MyGoldMinionCount,NoSmallerThan,6)，MyGoldMinionCount会去**CreateGetValueFuncFromStr(用的很多，通过字符串获取用户某个属性经常用)**中，进一步使用GetNumWithCounter利用反射去遍历configCountUtils.cs里面的计数函数。

8. pve回溯，玩家的计数器没有清空， 用到了GetSummonPlainMinionNumOneRoundFromPlayerCounter，这里面的Get和FromPlayerCounter是固定的

### 15.工具

AI模拟战斗：

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

### 12.android包共存 

一开始就修改AndriodManifest.xml，把android:authorities改成对应的包名， 再去调用之前unity打包的命令

### 11.英雄皮肤

### 10. 跑马灯

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



