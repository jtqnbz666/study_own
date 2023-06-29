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



