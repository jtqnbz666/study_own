1. =>的含义

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

12.c#的函数注册与lambda表达式

~~~c#
p.OnBeforeEndGame += (rank, userData, battleRewardInfo, isSpecial) =>{balaba}

+=表示给OnBeforeEndGame函数注册时间， 后边则是lambda表达式
~~~

13.c#的Task类型

~~~c#
//1.开始一个异步操作 
var task = Task.Run(() =>
{
    // 这里是异步操作
    System.Threading.Thread.Sleep(1000);
});
//2.等待异步操作结束

task.Wait()

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

15.Action的使用

~~~c#
Action<int, int> myAction = (x, y) => Console.WriteLine("Values: " + x + " and " + y);
myAction(5, 10);  // Outputs: Values: 5 and 10

lambda和action的关系
// 定义一个 Action
Action<int> myAction = (x) => { Console.WriteLine(x); };

// 定义一个 lambda expression
Func<int, int> myFunc = x => x * x;
这两者之间并没有本质上的冲突或者对立，相反，它们经常一起被使用。
~~~

16.值传递和引用传递(ref和out)

17.把string按照',' 分割为int类型数组

~~~c#
string[] minionIdsStr = minionIds.Split(',');
int[] minionIdsInt = Array.ConvertAll(minionIdsStr, int.Parse);
~~~



18.获取随机种子

~~~c#
 Random random = new Random(DateTime.Now.Millisecond);
 var seed = (uint)random.Next();;
~~~

19. 把string类型转为int

~~~C#
var num = int.parse(str)
~~~

20. ? 和 ？？的区别

~~~c#
? 以防空值
?? 左边如果为null，则取右边的值
string name = person?.Name ?? "Unknown";
~~~



21. delegate 委托是使用

~~~c#
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

