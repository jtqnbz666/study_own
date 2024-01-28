3.list对象有时候会用count，但如果是null对象，不能调用count方法

=>的含义

~~~c#
lambda表达式, 表达一个匿名函数，  => 前面的是参数， 后面的是函数体
举例：Func<int,int> Add=(x, y)=>x + y; Console.WriteLine(Add(2,3));
~~~

2.协议中的repeated字段， 需要采用list的addrange方法， 把这些repeated字段加入到list中
但本质上都是Pb.Reward, 只是需要这种转换。

~~~c#
 List<Pb.Reward> rewards = new List<Pb.Reward>();
rewards.AddRange(res.ShowRewardData.RewardList);

res.ShowRewardData.RewardList是pbc::RepeatedField<global::Pb.Reward>类型的
~~~



3.有时候可以看到一个类中调用 base.函数名() , base指的就是它的父类。

4.c#比较好的一点就是， 类中的对象可以赋变化的值，对于状态机模型感觉特别友好

5.根据某一条件过滤

~~~c#
var heroInfoList = data.HeroInfos.Where(heroInfo =>{
  var hero = Gen.GetHero(heroInfo.HeroID);
  return hero.BattleRoyale && 			    availableRaceIdList.Contains(hero.RaceID);//是否包含
}).ToList();
~~~

6.list筛选去重

~~~c#
List<ModelJD> data = myDalJD.GetAllDataList(); 
List<string> list= new List<string>();

this.cmbJdUserName.DataSource = list.Distinct().ToList(); //去重复，绑定数据后面要加ToList()


筛选出符合条件的
 set = set.FindAll(heroinfo => !tmp.ChampionShipsInfo.Contains(heroinfo.HeroID));
~~~

7.select用法

~~~c#
1.list= datalist4.Select(p => p.name).ToList();  //只取name字段，重新生成新的List集合
2.var canSelectHeroes = RandomSelect(heroInfoList, canChooseNum, player).Select(heroInfo => Gen.GetHero(heroInfo.HeroID)).ToList(); //会把GetHero表项返回，RandomSelect本来返回的是List<HeroInfo>，经过Select后变成了List<Hero>类型
~~~

8.list移除和插入某个元素

~~~c#
移除
CanSelectHeroes.Remove(newHero);

插入
CanSelectHeroes.Insert(0, newHero);
~~~

9.list根据某一字段得到自己想要的新的list

~~~c#
var allheroinfo = list.Select(val => new HeroInfo() { HeroID = val.Id }).ToList();
~~~



10.对象设置属性

~~~c#

//保存石灵等级
               public MinionCollectionInfo _minionLevelInfo;

               public MinionCollectionInfo minionLevelInfo
               {
                   get => _minionLevelInfo;
                   set
                   {
                       if (value != null)
                       {
                           _minionLevelInfo = value;
                       }
                   }
               }
~~~

11.遍历map

~~~c#
foreach (var entry in snapShot.MinionLevel)
            {
                player.MinionLevelInfo.Collections.Add(new SingleMinionCollectionInfo()
                    { MinionID = entry.Key,Level = entry.Value});
            }
~~~

13.c#的Task类型， c# 和 lua异步的相似之处

二者有很大的区别点在于c#是多线程的异步概念(不是说异步就一定会新开线程，而是一般会配合task使用， task会创建新的线程， 对应lua而言，会阻塞等待去执行别的地方，而c#不会阻塞主线程继续往后执行。)， 而lua是单线程靠协程实现异步

~~~c#
//1.开始一个异步操作 
var task = Task.Run(() =>
{
    // 这里是异步操作
    System.Threading.Thread.Sleep(1000);
});
//2.等待异步操作结束
task.Wait()
  
在某种程度上可以说C#中的await和Lua中的协程具有一些相似之处。当遇到await关键字时，当前方法的执行会暂时返回给调用者，线程将返回到线程池中，允许其他任务在此期间执行。一旦异步操作完成，调用将恢复，并继续执行await后面的代码。

虽然C#中的await和Lua中的协程并不完全相同，并且底层实现机制也不同，但它们都提供了一种在异步操作期间暂停当前执行，并在操作完成后继续执行的机制。

使用await的C#异步编程模型是基于任务（Task）和异步编程模式（async/await）的，它通过将任务封装在可等待的类型中，然后使用await等待任务的完成。

而Lua中的协程（Coroutine）是一种轻量级线程，在协程之间可以进行切换，实现非阻塞的异步操作。使用协程，可以暂停一个协程的执行，切换到其他协程进行处理，并在后续某个时机恢复协程的执行。

尽管await和Lua中的协程有一些相似之处，但它们的实现和使用方式有很大的差异。在C#中，await是通过异步编程模式和任务来实现的，而Lua中的协程是通过协程库和提供的API来实现的。

需要注意的是，C#中的await机制更加内置和集成化，而在Lua中使用协程需要显式地管理协程之间的调度与状态。

总结来说，C#中的await和Lua中的协程都提供了一种非阻塞式的异步处理机制，但实现细节和使用方式有所差异。无论如何，它们都可以在处理异步操作时提供更优雅和灵活的编程方式。

~~~

14= 和 =>的区别

~~~C#
"="：这是一个赋值运算符
 int a = 5
"=>": 这被称为“箭头操作符”
// 定义函数 或者 上边的lambda表达式
public int Add(int x, int y) => x + y;
// 定义只读属性
public string Name => "Foo";
// 定义只读属性的另外一种方式
public override bool IsFull => HandElems.Count == Capacity;
实质上就是
public override bool IsFull
{
    get { return HandElems.Count == Capacity; }
}
  //用于筛选，其实也就是lambda
  set = set.FindAll(heroinfo =>     !tmp.ChampionShipsInfo.Contains(heroinfo.HeroID));



~~~

15.Action和Func的使用

~~~c#
Action是一个泛型委托类型，它不返回任何值（void），通常用于表示没有返回值的方法。Action可以接受多达16个输入参数，但不返回任何值。Action可用于执行操作，如触发事件、执行异步任务等。
Func也是一个泛型委托类型，它可以表示具有特定输入参数和返回值的方法。Func的最后一个泛型参数表示返回值类型，对于简单的单行代码，Func委托可以使用隐式返回。但对于复杂的多行代码，仍然需要显式使用return语句来指定返回值
  
Action<int, int> myAction = (x, y) => Console.WriteLine("Values: " + x + " and " + y);
myAction(5, 10);  // Outputs: Values: 5 and 10

lambda和action的关系
// 定义一个 Action
Action<int> myAction = (x) => { Console.WriteLine(x); };

// 定义一个 lambda expression，注意Func的最后一个参数是返回类型的意思
Func<int, int> myFunc = x => x * x;
  
Action和Func这两者之间并没有本质上的冲突或者对立，相反，它们经常一起被使用。
~~~

16.delegate 委托(用于表示对一个或**多个方法**的引用)

~~~c#
委托（Delegate）是一种类型，用于表示对一个或多个方法的引用。委托可以实现类似函数指针的功能，允许将方法作为参数传递、存储和调用。通过委托，可以将方法作为参数传递给其他方法，实现回调和事件处理等功能。
例如：
public delegate void MyDelegate(string message);

一旦定义了委托类型，你就可以实例化这个委托，把它和一个方法绑定：
// 假设有一个名为Print的方法，它的签名与委托相匹配
public void Print(string message)  {  Console.Writeline(message);  }

// 创建一个这个委托类型的实例
MyDelegate del = new MyDelegate(Print);

然后就可以调用这个委托：
del("Hello, world!");

//匿名委托
MyDelegate del3 = delegate(string message) { Console.WriteLine(message); };
MyDelegate del4 = message => Console.WriteLine(message); //注意一下，匿名函数返回什么，那么这个函数就是什么类型
  
~~~

17. lamda结合委托：

~~~c#
原型： public delegate void BeforeEndGameDelegate(int rank, UserData userData, List<Reward> battleRewardInfo,bool isSpecial);

p.OnBeforeEndGame += (rank, userData, battleRewardInfo, isSpecial) =>{balaba}

+=表示给OnBeforeEndGame函数注册事件， 后边则是lambda表达式
~~~

18. 值传递和引用传递(ref和out)

19. 把string按照',' 分割为int类型数组

~~~c#
string[] minionIdsStr = minionIds.Split(',');
int[] minionIdsInt = Array.ConvertAll(minionIdsStr, int.Parse);
~~~

20. 获取随机种子

~~~c#
 Random random = new Random(DateTime.Now.Millisecond);
 var seed = (uint)random.Next();;
~~~

21. 把string类型转为int

~~~C#
var num = int.parse(str)
~~~

22. ? 和 ？？的区别

~~~c#
? 以防空值
?? 左边如果为null，则取右边的值
string name = person?.Name ?? "Unknown";
~~~



23.值类型和引用类型的区别

~~~c#
在C#中，可以使用以下方法来区分值类型和引用类型：
内置类型和自定义类型：
内置类型（如int、double、char等）是值类型。
自定义类型（如class、struct、enum等）可以是值类型或引用类型，具体取决于定义时用的是class还是struct关键字。
使用typeof运算符：
使用typeof运算符可以获取类型，然后使用IsValueType属性来判断是否为值类型。
使用typeof运算符获取的类型对象，调用IsClass方法来判断是否为引用类型。

int是值类型，string和Person是引用类型。
要注意的是，C#中struct类型也被认为是值类型，而class类型是引用类型。
~~~

24.as的用法

~~~c#
BR_SurrenderBackMsg collector = collectMsg as BR_SurrenderBackMsg;  //其中collectMsg是IMessage 类型

使用 as 关键字可以将一个对象转换为指定类型，如果不能转换，则返回 null。
as 关键字只能用于引用类型之间的转换，对于值类型，可以使用强制类型转换（如 int x = (int)someObject;）。另外，如果进行类型转换的两个类型之间不存在继承关系，则无法使用 as 进行转换，需要使用其他方式（如显式类型转换）。
~~~

24.is的用法, 判断一个消息的类型

~~~c#
 if (message is Uds_SelectStateMergedMsg selectStateMsg) 
            {
                await Task.Run(() =>
                {
                    // 模拟异步发送消息的操作
                    var result = SelectStateMergedInfo(selectStateMsg);
                });
            }
            else if (message is Uds_GameOverStateMergedMsg gameOverStateMsg)
            {
                await Task.Run(() =>
                {
                    // 模拟异步发送消息的操作
                    var result = GameOverStateMergedInfo(gameOverStateMsg);
                });
            }
~~~



25.把enum类型转为对应的值

~~~c#
enum Colors
{
    Red = 1,
    Green = 2,
    Blue = 3
}
static void Main(string[] args)
{
    Colors color = Colors.Green;
    int value = (int)color;
    Console.WriteLine("Enum value: " + value); // 2
}
~~~

26. 函数里面可以有本地函数

LoadMirrorData相当于PrepareAIData的本地函数

~~~c#
private void PrepareAIData()
        {
            // 将JSON字符串反序列化为AIHeroMirror对象列表
            List<AIHeroMirror> mirros = new List<AIHeroMirror>();
            mirros.Add(MirrorA.ToProtobuf<AIHeroMirror>());
            foreach (var user in Battle.AllPlayerList)
            { 
                LoadMirrorData(user);
            }

            void LoadMirrorData(BattleRoyalePlayer player)
            {
                var used = mirros[0];
                mirros.RemoveAt(0);
                player.Player.Mirror = used;
            }
        }.   
~~~

27. 很多都是引用类型, 用clone方法得到一个拷贝而不是引用

~~~c#
var res = resp.Heroes.Clone();
public AllHeroInfo Clone() {
  return new AllHeroInfo(this);
}
~~~

28. Task类型举例

~~~c#
以下两种方式是等价的。
public static Task DoSomethingAsync()
{
    return Task.Delay(1000);
}

public static async Task DoSomethingAsync()
{
    await Task.Delay(1000);
}
~~~

29. 异步代码演示

~~~c#
namespace  tt 
{
    class Test
    {
        public Test () {}
        public static async Task<int> CalculateValueAsync()
        {
            Console.WriteLine("Inside CalculateValueAsync");
            int threadId1 = Thread.CurrentThread.ManagedThreadId;
            Console.WriteLine("当前线程1ID为：" + threadId1);
            int result = await Task.FromResult(42);
            int threadId5 = Thread.CurrentThread.ManagedThreadId;
            Console.WriteLine("当前线程5ID为：" + threadId5);
            await Task.Delay(3000);
            Console.WriteLine("Calculations done");
            int threadId2 = Thread.CurrentThread.ManagedThreadId;
            Console.WriteLine("当前线程2ID为：" + threadId2);
            return result;
        }
        public static async Task TT()
        {
            int threadId0 = Thread.CurrentThread.ManagedThreadId;
            Console.WriteLine("当前线程0ID为：" + threadId0);
            Console.WriteLine("Before calling CalculateValueAsync");
            Task<int> task = CalculateValueAsync();
            Console.WriteLine("After calling CalculateValueAsync");
            await task;
            int threadId6 = Thread.CurrentThread.ManagedThreadId;
            Console.WriteLine("当前线程6ID为：" + threadId6);
            await Task.Delay(3000);
            Console.WriteLine($"Result: {0}");
        }
        public static void Main()
        {
            int threadId4 = Thread.CurrentThread.ManagedThreadId;
            Console.WriteLine("当前线程4ID为：" + threadId4);
            Console.WriteLine("1");
            var test = TT();
            Console.WriteLine("2");
            test.Wait();
            Console.WriteLine("3");
            int threadId7 = Thread.CurrentThread.ManagedThreadId;
            Console.WriteLine("当前线程7ID为：" + threadId7);
        }
    }
}

打印结果如下：
当前线程4ID为：1
1
当前线程0ID为：1
Before calling CalculateValueAsync
Inside CalculateValueAsync
当前线程1ID为：1
当前线程5ID为：1
After calling CalculateValueAsync
2
Calculations done
当前线程2ID为：4
当前线程6ID为：4
Result: 0
3
当前线程7ID为：1

~~~

30. 把一个父类转为子类，针对某一元素求和

~~~c#
opDamage 为 List<IEventRecord>类型
pDamageSum = opDamage
.Cast<PlayerDamageRecord>()
.Where(val =>val.Source == PlayerDamageRecord.SourceType.Effect)
.Sum(m =>  m.Damage);
~~~

31.list使用sort排序

~~~c#
返回-1表示第一个优先级高，1表示第二个优先级高，0表示相同
list.Sort((a, b) =>
            {
                // 大的在前，return -1
                if (a.Player.Hp > b.Player.Hp)
                {
                    return -1;
                }

                if (a.Player.Hp < b.Player.Hp)
                {
                    return 1;
                }
                return 0;
            });
~~~

32.c#中不能直接把proto的字典赋值给Dictionary<int, int>，但可以

~~~c#
AccountTriggerTimes = test.GuidePrompt.ToDictionary(x => x.Key, x => x.Value);
        
~~~

33. 消息流转为pb对象  &  pb对象， json串与string的转换

```c#
头文件
using System;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using CardGameCalculator;
using Newtonsoft.Json;
using System.Collections.Generic;
using Log;
using Pb;
using WatsonWebserver;
using ProtobufSerializer = CardGameCalculator.ProtobufSerializer;
```

- 消息流转为pb对象

~~~c#

ctx是HttpContext类型
var stream = new StreamReader(ctx.Request.Data);
var body = await stream.ReadToEndAsync();
var msg = ProtobufSerializer.DeSerialize<AiAllSimulateMsg>(body);
~~~

- pb转json串

~~~c#
Ret.MirrorList.Add(ConvertData.ToJson(pb对象));
等价于
Ret.MirrorList.Add(pb对象.ToJson());
//MirrorList是list<string>类型
~~~

