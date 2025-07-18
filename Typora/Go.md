**好用的框架**

~~~go
1. ini文件读取相关：ini库，"github.com/go-ini/ini"
~~~

**小知识**

~~~go
21.空struct{}的用途：空struct节省内存一般作为占位符使用，表示这里并不需要一个值1.比如用map来表示集合的时候，value可以用struct{}类型，2.chanel控制并发时，只需要一个信号并不需要传递值，也可以用struct代替，3.声明只包含方法的结构体
20.go中使用const来表示枚举值
19.判断两个slice是相等的可以通过反射reflect.DeepEqual
(a, b)但比较影响性能，一般直接遍历slice的每个元素
18.同一函数多个defer遵循后写先出(栈)， 如果在defer中修改变量值， 如果是无名返回值则不会修改成功，因为go的机制是在返回的时候会创建一个临时变量保存返回值，defer修改了局部变量并没有修改返回值，但如果是有名返回值就不会创建临时变量，defer中对该变量值修改就能对返回值产生影响
17.局部变量不一定在栈上，go编译器会做逃逸分析，比如以下就会分配在堆上
func foo() *int {
	v := 11
	return &v
}
func main() {
	m := foo()
	println(*m) // 11
}
16.init()函数执行时机：import –> const –> var –> init() –> main()
15.切片是引用类型， 当参数传递也是引用类型，但如果使用了append函数会返回一个新的切片地址，不会对原切片产生影响，所以给人一种值类型的错觉
func test(arr *[]int32) {
	*arr = append(*arr, 9)
}

func main() {
	var mySli = []int32{1, 2, 3, 4}
	test(&mySli)
14.redis操作中没有值会返回redis.Nil的有
Get(key); HGet(key, field); LIndex(key, index); ZScore(key, member); SIsMember(key, member); exists(key) 
不会返回redis.Nil的有
Set(key, value, expiration); SetNX(key, value, expiration); HSet(key, field, value); HSetNX(key, field, value); ZAdd(key, score, member)
13.map遍历是无序的，即时遍历同一个map多次, slice是有序的
12.对字符串切片排序, sort.Strings(切片)
11.range是可以遍历空slice或空map的，不会抛异常，平常报错是因为 range *nil了，如果对nil使用解引用是会报错的
10.strings.Split("", ",") 返回的切片大小是1
9.strconv.ParseBool(str)，只有字符串为"1"/"true"才能解析为true
8.json.Unmarshal如果内容为""会出错
7.时间戳是没有时区概念的
6.copy可以拷贝切片对象
5.gorm方法表有数据之后再修改表的索引，可能重新同一字段重复索引，可以删除不要的索引
alter table table_name drop index idx_name;
4.创建gorm的索引比如`gorm:"type:varchar(100);index:idx_name,unique"` //注意;和,的区别，如果把里面的,换成;就会创建两个索引，一个是unique创建的，一个是index创建的， 或者直接换成uniqueIndex:idx_name
3.go的main()执行前，会先执行init函数(这个init函数是别的包中的也是一样)， init函数执行前会先初始化全局变量
2.时间处理上，用2006那个时间，只要写上固定的时间，format得到的时间戳就是utc时间
3.把字符串以某个符号分割. strings.Split(str, ",")， 如果是切片合为字符串：strings.Join(切片, ","), 不会在最后多加一个","
4.fmt.Sprintf("%v", levelConf.Id)给字符串格式化
5.如果改了某个框架的东西，go mod tidy
go get gitlab.hoxigames.com/hoxi-games/hoxi-server@develop  最后的develop表示develop分支
6.两个切片合并。 append(chan1, chan2...)
7.%v无法打印出切片内容，而是打印他的地址， 可以用%+v或%#，+v打印struct可以同时打印字段名和值，而普通的%V只打印值
8.遍历切片的时候，比如, 'for i, entity := range Entities'如果Entities是指针类型，就可以通过entity进行修改，如果不是指针类型，那么对entity的修改是不会反映到Entities上的
9.使用ret := RawData{} 和 ret := &RawData{}来调用自己的指针方法，二者都可以，没有区别
10.更新依赖：go get -u； 清除缓存：go clean -modcache
11. 目前10位是s， 13为时间戳才是毫秒
12. 记得养成加recover的习惯， 注意定时任务那里
13. go中得到interface{}对象如果本身是json，可以用json.RawMessage来接收
14. 传过来的数据如果是用interface{}类型来接收的数字，一定是float64,需要自己再转成需要的, 如果不用interface{}, 那么你可以用任意类型来接收，比如是个整数，你可以用int32,int,int64, 如果是小数，就用float相关的， 这里还有小细节，就是接收的这个结构体的字段必须是大写字母开头的
15.如果HonorRecord是指针类型对象，*HonorRecord = *test 和 HonorRecord = test是等价的，是浅拷贝， 
以下是深拷贝
HonorRecord = proto.Clone(test).(*model.UserHonorRecord)
16.如果对一个字符串进行了json.marshal，他就会多一层引用，有时候不能这么搞，可以使用strconv.Unquote给他去掉这层引用
17.打印堆栈观察信息
var buf [10240]byte
n := runtime.Stack(buf[:], false)
fmt.Printf("=== Stacktrace ===\n%s\n", buf[:n])
~~~

**go mod vendor**

``` go
1.将所有依赖打包在项目中，避免依赖版本变动导致问题。
2.离线构建，在没有网络连接的情况下也能构建项目，比如现在引用了一个本地自定义开发的包(不是从远端获取的)，此时需要打包一个docker镜像，就需要把自定义的包使用go mod vendor到项目中才能成功打包镜像。
```

实用技能

~~~go

9.架构迁移时出现包循环调用问题，新创建一个包比如utilsBridge, 然后在这里面定义函数比如var GetTotalUserData func(userID uint64) *pb.UserData， 在main函数初始化它比如utilsBridge.GetTotalUserData = utils.GetTotalUserData
8.var test []string, 不给test任何内容也是可以range的，但如果是var test *[]string, 使用 range *test 就会抛异常
6.选择json.Unmarshal而不是proto.Unmarshal,因为前者支持任意对象
5.nil不能直接类型转换，但可以配合ok
tt := interface{}(nil) // 可以通过这种方式给对象赋值为nil
res, ok := tt.(*model.UserRLActivity)
4.将字符串分割
"github.com/elliotchance/pie/v2"
pie.Map(strings.Split(skinconf.Price, "|"), func(s string) int32 {
		res, _ := strconv.Atoi(s)
		return int32(res)
})

在切片中根据条件找出某个元素
pie.FindFirstUsing(user.FinishAdventureInfo.ChapterFinishInfo, func(chapterInfo *pb.ChapterInfo) bool {
			return chapterInfo.ChapterId == task.ExtraInfo.ChapterId && pie.Contains(chapterInfo.CustomsPass, task.ExtraInfo.CustomsId)
		})

3. 判断某个key是否存在于map中
rank, exists := rankMap[msg.Uid]
if exists {
  //如果有排名就记录一下
  heroRankMap[hero.Id] = rank
}

4. int与string转换
int32-->string strconv.Itoa(int(num))
uint64-->string strconv.FormatUint(uid, 10)		
string-->uint64 strconv.ParseUint(fieldArr[1], 10, 64)	//十进制，64位
string-->int32 num, err := strconv.Atoi(str)

5.
rank, exists := rankMap[msg.Uid]
if exists {
  //如果有排名就记录一下
  heroRankMap[hero.Id] = rank
}

6. string和[]byte可以随时转， 比如
jsonData, _ := json.Marshal(guidePrompt)
	userData.UserGuideInfo.GuidePromptJs = string(jsonData)， 反过来也是一样
json.Unmarshal([]byte(userData.UserGuideInfo.GuidePromptJs), &restoredData)
7. json.Marshal配合结构体生成json对象
type MiID struct {
	MID string `json:"mid"`
}
miID := MiID{
  MID: "666",
}
miIDJson, err := json.Marshal(miID)
这样得到的miIDJson的json串就是"{/"mid/":/"666/"}"
		
8.如果有匿名字段，json.Unmarshal时要注意格式，比如
Unmarshal函数需要一个指向结构体的指针，才能完成对结构体的修改
type SyncOrderReq struct {
	QueryParam
}
type QueryParam struct {
	OutOrderId string `json:"out_order_id"`
}
ExtraParamJsonStr的内容为
extraParam := map[string]string{
    "out_order_id": msg.OutTradeNo,
}
ExtraParamJsonStr, _ := json.Marshal(extraParam)


json.Unmarshal([]byte(orderInfo.ExtraParamJsonStr), syncOrderReq)//这里必须用syncOrderReq，不能是syncOrderRe.QueryParam.或者syncOrderRe.QueryParam.OutOrderId

9.redis的返回
value, err := global.ConnRedis.Get(context.Background(), "tt").Result()
if err == redis.Nil {
		logrus.Infof("test%v", err)
} else if err != nil {
  logrus.Infof("test:%v", err)
} else {
  logrus.Infof("%v", oldScore)
}
10.csv文件写入，‘,’会自动切换下一个格子
str := ""
for i := 0; i < len(res); i++ {
  str += fmt.Sprintf("%v,%v\n", i+1, res[i])
}
os.WriteFile(resFile, []byte(str), 0666)
~~~

泛型编程

~~~go
// 这个函数只能不是公共的，不能是某个对象的，比如func (p *leaderboardStruct) ParseHashDataFromJson [T any](key, member string) *T
func ParseHashDataFromJson[T any](key, member string) *T {
	var boardData T
  json.Unmarshal([]byte(res), &boardData)
	return &boardData
}

~~~





**[]*model.UserHero和[]model.UserHero的区别**

~~~go
type UserHero struct {
    Name string
    Level int
}
func main() {
    heroes := []*UserHero{
        {Name: "Hero A", Level: 10},
        {Name: "Hero B", Level: 20},
    }   
    ptrSlice := []*UserHero{}
    valSlice := []UserHero{}
    for _, hero := range heroes {
        ptrSlice = append(ptrSlice, hero) // 将指针追加到切片
        valSlice = append(valSlice, *hero) // 将指针指向的值追加到切片
    }
    heroes[0].Name = "Updated Hero A"
    fmt.Println(ptrSlice[0].Name) // 输出：Updated Hero A
    fmt.Println(valSlice[0].Name) // 输出：Hero A
}
~~~



将map[int32]int32以json存入mysql再取出来

~~~go
存进去
jsonData, err := json.Marshal(guidePrompt)
res := global.MysqlIns.Where("user_id = ?", userID).
		Updates(&model.UserGuideInfo{
			GuidePromptJs: string(jsonData),
		})

取出来
var restoredData map[string]int32
	err := json.Unmarshal([]byte(user.GuidePromptJs), &restoredData)
	if err != nil {
		logrus.Fatal(err)
	}

	for key, value := range restoredData {
		intKey, err := strconv.ParseInt(key, 10, 32)
		if err != nil {
			logrus.Fatal(err)
		}
		backmsg.GuidePrompt[int32(intKey)] = value
	}
~~~



### c++, go, lua, shell, python不同语言遍历区别

~~~go
//python中
for i in range(10):
	print(i)
//c++ 中
for(int i = 0; i < 10; i ++) 


//go  中
//简单遍历
for  i := 0; i < 10; i ++ {
    
}

for {		//无线循环
    
}

//遍历数组类型
for k, v := range array {
    //k表示索引下表，v是对应的值
}

//lua 中
//简单遍历
for i = 1,  10, 2 do //10表示最大值，2表示每次步数
	
end

//遍历数组类型
local t = {2,3,4,5,6}
for k, v in pairs(t) do
	print(k,v)
end


//shell 中
//遍历数组类型
cnt=1
for i in "$@" //可以不加""
do
	echo "$cnt:$i"
	((cnt++))
done

for loop in 1 2 3 4 5
do
	echo "the value is: ${loop}"
done

//遍历普通类型
for((i=10;i<40;i++))
do
	...
done


//c#中
foreach (var val in list) {
  
}
~~~

go时间

~~~go
strconv.FormatInt(time.Now().Unix(), 10) //1685094260

uint64(time.Now().UTC().UnixNano()) / uint64(time.Millisecond)//时间戳1685094260742

date := time.Now().Format("20060102150405")
fmt.Println(date)//获取当前时间20230510112501



//获取到时分秒 local表示把UTC转为当地时间
  year, month, day := time.Now().UTC().Date()
	hour, minute, second := time.Now().UTC().Local().Clock()
	unix := time.Date(year, month, day, hour, minute, second, 0, time.UTC)

	fmt.Println(unix)	//二者一样
	ret, _ := time.Parse("2006/1/2 15:04:05", "2023/5/29 17:29:00")
	fmt.Print(ret)		//二者一样




~~~

把字符串时间转为时间戳

~~~go
func main() {
	// 定义需转换的时间格式
	layout := "2006/01/02 15:04:05" //"2006-01-02 15:04:05"

	// 定义需转换的时间字符串
	str := "2021/10/28 13:16:12"  //2021-10-28 13:16:12.123
  //不管是用/ 还是 - 只要大家都一样就行

	// 使用 Parse 将时间字符串转换为 Time 类型
	t, err := time.Parse(layout, str) //这里的t是Time类型，不是string类型
	if err == nil {
    t = t.UTC()	//重点：这种写死的时间，执行.UTC()还是一样的结果
	}
  
	// 使用 Unix 函数将 Time 类型转换为时间戳
  //timestamp := t.Unix() 精确到秒
	timestamp := uint64(t.UnixNano()) / uint64(time.Millisecond)
	fmt.Println(timestamp)
}

~~~

把时间戳转为utc

~~~go
	time.Date(1970, time.January, 1, 0, 0, 0, 0, time.UTC).Add(time.Duration(时间戳) * time.Second).UTC()
//把time.Second换成millisecond就是毫秒
~~~

求时间差(天数) 

~~~go
毫秒为单位
dayTime := timeUtil.MakeTimeStamp(time.Now()) - useInfo.LastPlayTime
if math.Floor(float64(dayTime/(24*60*60*1000))) > 30 
~~~



goNb的点

~~~go
1.select中ctx.Done() 机制
2.defer 机制
3.wg sync.Waitgroup ; wg.Add(1), wg.Done(), wg.Wait()机制
4.反射
5.做一个web后端特别容易
~~~

go处理http消息的三种方式

~~~
本质上都是去ServeHTTP()，
1.http.ListenAndServe("localhost:9999", s), 对s这个对象实现ServeHTTP()方法
2.http.HandleFunc("路径"， 处理函数)
3.http.Handle(registryPath, s) 对这个s对象实现ServeHTTP方法
~~~





### c++和go定义对象的区别 

~~~go
c++
struct student {
    int age
    string name
}

go 
type student struct {
    age int
    name string
}

c++ 定义对象
student a = {18, "jt"};

go 定义对象
a := student {
    age : 18,
    name : "jt",
}


为什么go需要在 {} 前边加上 结构体名字， 因为它前边没有加类型
~~~

c++和go的函数指针的区别

~~~go
go中
type Func func(string,  int)

c++中
typedef void(*Func)(string, int) 	//c
typedef std::function<void(string, int)> Func;
using Func = function<void(string, int)>;
~~~

c++和go的类型转换的区别

~~~go
go中
type String string 
str := String("test") 或者 str := string("test")

接口型函数
type Getter interface{
    Get(key string)([]byte, error)
}

type GetterFunc func(key string) ([]byte, error) 

func (f GetterFunc) Get(key string) ([]byte, error) {
    return f(key)
}

var f Getter = GetterFunc(func(key string) ([]byte, error) {
    return []byte(key), nil
})

但go中也可以向c++一样
检测某个结构体是否满足接口的功能需求
var _ PeerGetter = (*httpGetter)(nil)

c++中
string tmp = "test"
string str = (string)tmp;
~~~



### 日志打印

~~~go
import "log"
 
func main() {
	log.SetPrefix("[logusage]")
	
    //日期和文件完整路径
    log.SetFlags(log.Ldate|log.Llongfile)
 	
	log.Println("i am log")
	log.Fatalln("i am fatal")
}
打印结果：
[logusage]2020/07/02 F:/go/src/algorithm/logusage/main.go:9: i am log
[logusage]2020/07/02 F:/go/src/algorithm/logusage/main.go:10: i am fatal
~~~

**模板templete的使用**

~~~go
package main

import "fmt"
import "html/template"
import "os"

func main() {

        type person struct {
                Id      int
                Name    string
                Country string
        }

        liumiaocn := person{Id: 1001, Name: "liumiaocn", Country: "China"}

        fmt.Println("liumiaocn = ", liumiaocn)

        tmpl := template.New("tmpl1")
        tmpl.Parse("Hello {{.Name}} Welcome to go programming...\n")
        tmpl.Execute(os.Stdout, liumiaocn)
}
~~~



### sync系列

**sync.WaitGroup**

~~~go
配合上下文一起使用， 上下文主要是针对特定条件来进行下一步操作，而sync.WaitGroup是为了保证每一个任务都结束了再进行下一步操作。二者并不冲突，并且能一起配合带来更好的操作。
var wg sync.WaitGroup
//主要有两个操作 wg.Add()  和 wg.Done()

//上下文
ctx, cancel := context.WithTimeout(context.Background(), 10 * time.Second)
//返回的ctx是上下文，cancel是用于中断上下文的函数， 也可以等超时后自动调用cancel， 效果就是会触发 ctx.Done(), 那么就可以用select来监听这个事件，当出现这个事件的时候，表示时间到期或者手动cancel取消了上下文。

每次开一个任务的时候都会执行wg.Add() , 任务执行完后 都执行wg.Done， 当wg.Wait()等于0时，结束阻塞

//上下文还可以嵌套，比如ctx1 依赖 ctx， ctx2 依赖 ctx1， ctx3 依赖 ctx2， 那么当ctx1这个上下文中断后，ctx2 和 ctx3 都会跟着中断， 但是ctx 不会受到影响
~~~

### sync.Once 

```go
var once sync.Once

func once1() {
    fmt.Println("once1")
}
func once2() {
    fmt.Println("once2")
}
for i := 0; i < 5; i ++ {
    once.Do(once1) //只有它会执行
    once.Do(once1) 
    once.Do(once2)
    fmt.Printf("i = %d", i) //每一次都会执行
}
```

### 



### select多路复用

~~~go
//与上边的sync.WaitGroup 和 context.Context 结合使用
func work(ctx context.Context, str string) {
    for {
        select {
        case <- ctx.Done():  
       //本质上是一个管道，如果关闭管道就会触发，因为可以读取关闭的管道
      //若管道没有关闭就会一直阻塞在这里
        // 触发它的方式有两种，要么手动cancel取消，要么超时
            fmt.Println("退出", str)
            return 
        default://默认走这里，否则就会阻塞在上边ctx.Done()
            fmt.Println("循环中")
            time.Sleep(time.Second)
        }
    }
}
~~~



### map方法

~~~go
var a1 map[string]string
var b1 map[interface{}]interface{}
var m1 map[[]byte]string //报错,不支持以[]byte作为key
~~~

### 输入参数解析flag.String()

~~~go
test.go文件
 backup_dir := flag.String("b", "/home/default_dir", "backup path")
 debug_mode := flag.Bool("d", false, "debug mode")

 flag.Parse()

fmt.Println("backup_dir: ", *backup_dir)
fmt.Println("debug_mode: ", *debug_mode)

// 第一个，命令行参数名称，如-b -help，这里就是 -docDownloadUrl.  第二个是默认值。 如果输入-help 或者 其他的就会以第三个参数的内容提示你

比如
 go run test.go -b /home/jt
那么就是输出 
dir:  /home/jt
mod ; false

再比如
go run test.go -help // 或者其他不认识的 -..  就输出下边的提示
  -b string
        backup path (default "/home/default_dir") //第三个参数提示
  -d    debug mode							   //第三个参数提示
                     
          

~~~

而在c++中是这样的

~~~c++
#include <iostream>
#include <gflags/gflags.h>

DEFINE_bool(isvip, false, "If Is VIP");
DEFINE_string(ip, "127.0.0.1", "connect ip");
DECLARE_int32(port);
DEFINE_int32(port, 80, "listen port");

int main(int argc, char** argv)
{
  google::ParseCommandLineFlags(&argc, &argv, true);
  std::cout<<"ip:"<<FLAGS_ip<<std::endl;
  std::cout<<"port:"<<FLAGS_port<<std::endl;
  if (FLAGS_isvip)
  {
      std::cout<<"isvip:"<<FLAGS_isvip<<std::endl;
  }
  google::ShutDownCommandLineFlags();
  return 0;
                  
}     
~~~







### 字节序和字符串相互转换

~~~go
var s1 = "hello world" 
var b = []byte(s1)
var s2 = string(b)
fmt.Println(b)	//[104 101 108 108 111 32 119 111 114 108 100]
fmt.Println(s2) //hello world
~~~

### go协程

~~~go
func hello(name string) {
    for {
        fmt.Println("hello world:", name)
        time.Sleep(1 * time.Second)
    }
}
go hello("jt") // 加上go 的关键字, 把函数放到协程中执行
~~~

### 异常捕获

~~~go
//以保护方式运行一个函数
func protectRun(entry func()) {
    defer func() {
        //捕获异常
        err := recover()
        switch err.(type) {
        case runtime.Error:
            fmt.Println("runtime error", err)
        default:	//非运行时错误
            fmt.Println("error:", err)  
        }
    }()
    entry()
}

//定义一个崩溃时传递的上下文信息
type panicCtx struct {
    function string 
}

//手动触发panic
protectRun(func() {
    //使用panic传递上下文
    panic(&panicCtx {		//error: &{手动触发panic}
        "手动触发panic"
    })
  
})

//故意制造空指针访问错误
protectRun(func() {
  var a *int
  *a = 1// 对空指针操作,触发异常
// runtime error: invalid memory address or nil pointer dereference
})
~~~





### 结构体标签

~~~go
type student struct {
    Name string `json:"name"`
    Age	 int 	`json:"age"`
}

func main() {
    var stu = Student{Name:"jt", Age:18}
    //注意这种初始化的方式，如果是
    var stu = Student{
        Name:"jt",
        Age:18,  // 则这里必须加，
    }
    
    data, err := json.Marshal(stu)
    fmt.Println("stu:", string(data))
    //打印结果 stu:  {"name":"jt","age":18}
}
~~~



### make 方法

~~~go
make 的第二个参数表示长度， 第三个参数表示容量
intSlice := make([]int, 0)
mapStringInt := make(map[strint]int) //加了数字也没用
ch 	:= make(chan int, 4)

输入类型：chan<- *ProducerMessage
输出类型：<-chan *ProducerMessage
~~~

### chanel管道使用

~~~go
<-chan int 表示输出类型，  chan<- int 表示输入类型
1.不能向一个已关闭的管道写数据
2.从已经关闭的空管道读数据不会阻塞会返回管道数据，没有则返回0，select中的ctx.Done()也是通过这个机制实现的。
3.从未关闭的空管道读数据会阻塞。 特例：如果是select有default分支则不会阻塞select。

4.若管道的大小为1， 那么如果没有把里面的这个元素读取出来，但仍然往里面写就会阻塞。

5.不能直接往一个大小为0的管道写入数据， 但如果这个大小为0的管道存在一个读它的协程， 那么这种情况是可以向大小为0的管道写数据的。
func receive(ch chan int) {
	value := <-ch // 读管道
	fmt.Println(value)
}

func main() {
	ch := make(chan int) //大小为0
	go receive(ch)
	ch <- 1.0 //写管道   如果把它和上一行的位置交换就报错
    
    
6.确保管道写安全的最好方式就是由负责写管道的协程自己来关闭管道
7.多协程采用wg *sync.waitgroup，每个发送者都wg.Add(1); send完毕后wg.Done(); 调用wg.Wait()阻塞，当所有的send都结束了再关闭管道。
    
8.遍历管道 , 不会主动退出循环，除非管道ch被关闭。
    for v := range ch {
        
    }
~~~





### switch-case 操作()

~~~go
默认每个case后都跟着break(未显示出来)， 就算啥都不写也不会继续往下执行
switch a {
case 1:
    fmt.Println("1")
case 2:
    fmt.Println("2")
 	fallthrough  //通过它就能继续执行下一个case
case 3:
    fmt.Println("3")
}

// 检测异常错误时
switch err.(type) {
	
}
~~~





### 数组转切片

~~~go
细节是转化后，切片和数组的地址是一样的

var a = [10]int{1, 2, 3, 4}

b := a[1:5]
fmt.Printf("%p\n", b)
//	0xc00001e0a8

fmt.Printf("%p\n", &a[1])
//	0xc00001e0a8

fmt.Println(b)//[2,3,4,0]

var slice []int
var arr [5]int = [...]int{1, 2, 3, 4, 5}
slice = arr[:]//得到所有
slice = slice[1:]//从第一个开始
slice = slice[:len(slice)-1]//截掉最后一个

// 增加元素
slice = append(slice, 100) 
~~~

### 切片

~~~go
常用函数  cap ， len
type slice struct { //切片的底层结构就是数组指针，长度，容量
    ptr *[100]int
    len int
    cap int
}

func make(s slice, cap int) slice {
	s.ptr = new([100]int)
	s.cap = cap
	s.len = 0
	return s
}
~~~



### 数组

~~~go
var a1 = [5]int{1, 2, 3, 4, 5}
var a2 = [...]int{38, 283, 48, 38, 348, 387, 484}
var a3 = [...]int{1: 100, 3: 200}
var a4 = [...]string{1: "hello", 3: "world"}

遍历二维数组
var a [2][5]int = [...][5]int{{1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}}

for row, v := range a {
    for col, v1 := range v {
        fmt.Printf("(%d, %d) = %d", row, col, v1)
    }
}
~~~



### 指针数组传参

~~~go
func test(arr *[5]int) {
    (*arr)[0] = 100
}
var a [5]int 
test(&a) 
~~~

### 匿名对象

~~~go
go中一个结构体中如果包含另一个结构体的匿名对象， 那么相当于继承了它的所有属性和方法。

举例一：
type Person struct {
    Name string
    Age  int 
}
type Student struct {
    score	string
    Age 	int
    Person	//匿名对象
}
//如果要初始化这个匿名对象， 可以理解为Student结构体中有一个匿名对象的名字和结构体名字一样

//如果匿名对象中有和当前类相同的成员，如果是匿名的方式优先调用自己的同名成员

stu := &Student {
    //如果是*Person 这里就换成Person: &Person{}而不是*Person: &Person{}
    //如果引用的别的包的结构体，把前缀去掉就可以了，
    //如果同时引用了两个别的包的结构体，并且是同名的，需要对其中一个明确的命名，不然编译会报错
	
    //也可以按照顺序直接初始化，但按顺序就必须初始化每个对象，或者用有名对象
    Person: Person { 
        Name: "jt",
        Age : 18,
    }, // 这里必须加逗号
}


若采用一行的方式
stu3 := Student{score: "100", Age: 20} //20紧接着可以不加逗号， 如果是分行的，必须所有元素都要加上逗号

//这种初始化方式是错误的，不管是否是匿名对象都不行
stu3 := Student{score: "100", Age: 20, {"jt", 19}} 


举例二：

type father struct {
    a int
    b int
}
type son struct {
   	 *father
     b int 
}


tt := &son{b: 10}
tt.father = &father{a: 6, b: 12} // 注意这里的使用
fmt.Println(tt.a)
可以直接调用father中的a，如 tt.a
fmt.Println(tt.b) //默认是son的b
fmt.Println(tt.son.b) //显式调用father的b
//由此可见，在son中可以实现方法来覆盖father的方法达到重写的效果
~~~



### defer执行顺序

~~~go
后调用的defer先执行
defer fmt.Println(1)
defer fmt.Println("second")
打印结果
second
0
~~~

### 可变参数传递

~~~go
// 参数形式
变参的本质其实是slice
func add(a int, val ...int) int {
	var sum int = a;
	for i := 0; i < len(arg); i ++ {
		sum += arg[i]
	}
}

func test(a int, val ...int) {
	//传递的时候是这种形式， 和c++格式一致
	add(1, val...)
}


c++ 中
template<class... T>   // T是模板参数包
void function(T ... args)    // args是函数参数包
{
 	func(args...);
}

go中
func (c *Context) String(values ...interface{}) {
	c.Writer.Write([]byte(fmt.Sprintf(values...)))
}
~~~



### 函数是值类型

~~~go
type handlerfunc func(int, int) int

func operator(op handlerfunc, a int) (int, int) {
    return op(1, 2)
}

//以下这种显示给出参数的不用返回值
func calc(a, b int) (sum int, avg int) {
	sum = a + b
	avg = (a + b) / 2
	return
}

~~~







### 循环遍历

~~~go
str := "I Like HH"
for k, v := range str {
    fmt.Printf("索引:%d, 值:%c", k, v) 
    //v 是字符
}
//第二种
for i := 0; i < 10; i ++ {
    
}
~~~



### string和[]byte的转换

~~~
bytes := []byte("I love hh")
str := string(bytes)
~~~



### 定义多个对象

~~~go
//const 类型
const year = 10
const(
	Man = 1
	femmale = 2
)
//type
type people struct{}

type(
    people struct{}
    
    superman struct{}
)
~~~



### 交换法则

~~~go
first := 100
second := 200

first, second = second, first
~~~

### 接口

~~~go
类型转换
var user = map[string]interface{} {
	"age": 30,
	"address": "beijing"
	"married": true,
}
//类型转换
可以这样写的原因是因为interface{} 具有一个空方法， 类似与java中的object对象
var age = user["age"].(int)
var address = user["address"].(string)
var married = user["married"].(bool)


获取类型
func TestType(items ...interface{}) {
    for k, v := range items {
        switch v.(type) {
            case string:
            //
        	case int:
            //
        }
    }
}
TestType("jt", 666)


通过反射修改对象的真实值
type Rect struct {
    Width int
    Height int
}

var a interface{}
var r = Rect{50, 50}
a = &r
var rx = a.(*Rect) 得到一个*Rect对象
r.Width = 100
r.Height = 100
fmt.Println("r:", r) //{100 100}
fmt.Println("rx": rx)//{100 100}
fmt.Printf("rx:%p, r:%p\n", rx, &r)
//rx:0xc0000180a0, r:0xc0000180a0

接口型函数
定义一个函数类型 F，并且实现接口 A 的方法，然后在这个方法中调用自己。这是 Go 语言中将其他函数（参数返回值定义与 F 一致）转换为接口 A 的常用技巧。
type Getter interface{
    Get(key string)([]byte, error)
}

type GetterFunc func(key string) ([]byte, error) 

func (f GetterFunc) Get(key string) ([]byte, error) {
    return f(key)
}

var f Getter = GetterFunc(func(key string) ([]byte, error) {
    return []byte(key), nil
})

~~~

go中import前边加符号区别

~~~
1.加 . 表示省去前缀，比如fmt.Println()写成Println()
2.加 _ 表示只调用里面的init函数
3.加 任意字符串 表示取别名 (针对包名和路径名不相同也可以用这种方式来调用) 
~~~





## 封装gin

给http注册处理函数两种方式

~~~ 
第一种
http.HandleFunc("/", indexHandler)

log.Fatal(http.ListenAndServe(":9999", nil)， 这里就写nil就行了

第二种
engine := new(Engine)
log.Fatal(http.ListenAndServe(":9999", engine)
必须给engine 实现一个ServeHTTP方法，并且参数格式为
func (engine *Engine) ServeHTTP(w http.ResponseWriter, req *http.Request) {}

~~~



### 编译跳转问题

vscode上

更改代理设置
go env -w GOPROXY="https://proxy.golang.org,direct"

⬇

go env -w GOPROXY=https://goproxy.cn,direct

ctrl + shift + p 输入setting， 找到Docs Tool 改成guru

### 包管理方式

像fmt等包都在GOROOT/src里面找， 若设置了GO111MODULE=auto，当使用go mod 时， 就不会去GOPATH目录(但会搜索GOROOT)， 如果不使用go mod，就会同时去GOPATH  和 GOROOT 下寻找， 这种情况下，使用go get 下载的资源放在GOPATH/src目录下

**当找不到问题的时候，去看看GOROOT和GOPATH是否配置错误，把GOROOT改成当前使用的go版本对应的路径**



lua 是单线程协程， go是 多线程协程， 协程都是用户态线程， Go中的协程是抢占式的。

如果让Go的一个协程协程 空循环， 这个协程并不会一直在循环中， 会被抢占出来去做别的事情。

sudo tc qdisc add dev ens33 root netem delay 500ms

Go中的conn.Read(buf) 和 linux 的read阻塞不是同一个东西
对于前者， 协程发生切换(add fd到epoll监听可读)，有可读事件后一样是由epoll_wait去通知

对象池 用处
收到http请求， 创建一个parser ， 复用它(注意复用的时候需要先reset重置它)

Golang中得雪花算法衍生得sonyflaFke
只有当current大于elapsedTime，才会将current赋值给elapsedTime，也就是说elapsedTime是一直增大的，即使时钟回拨，也不会改变elapsedTime。
如果没有发生时间回拨，就是sf.elapsedTime = current，自增id满了以后，这个单位时间内不能再生成id了，就需要睡眠一下，等到下一个时间单位。
**当发生时间回拨**，sequence自增加1。当sequence加满，重新变为0后，为了防止重复id，将elapsedTime+1，这个时候elapsedTime还大于current，睡眠一会儿 。 去看代码可以发现， 它得这种设计不会出现重复。

拓展：关于发生时间回拨：

~~~markdown
原生的Snowflake算法是完全依赖于时间的，如果有时钟回拨的情况发生，会生成重复的ID，市场上的解决方案也是非常多的：最简单的方案，就是关闭生成唯一ID机器的时间同步。

使用阿里云的的时间服务器进行同步，2017年1月1日的闰秒调整，阿里云服务器NTP系统24小时“消化”闰秒，完美解决了问题。

如果发现有时钟回拨，时间很短比如5毫秒,就等待，然后再生成。或者就直接报错，交给业务层去处理。

可以找2bit位作为时钟回拨位，发现有时钟回拨就将回拨位加1，达到最大位后再从0开始进行循环。

个人比较推荐的是最后一个方案找2bit位作为时钟回拨位，发现有时钟回拨就将回拨位加1，达到最大位后再从0开始进行循环。
~~~

go mod

~~~
 GO111MODULE
 // 若是auto，如果在gopath/src下，但是存在go.mod文件，就还是采用的go.mod里require的包， 未在gopath/src下， 自然是采用的go.mod里require的包
 //若是on， 不管在不在gopath/src下，都采用的go.mod里require的包
 //若是off，就是老规矩(会同时去gopath/src,和goroot/src下寻找)， 并且项目必须在gopath/src下， 因为下载的外来的包资源会存放在gopath/src下
 
 //需要注意一点，若使用go mod 方式， 下载下来的包会放在gopath/pkg中，而不是gopath/src中。 所以有时候找不到包，需要考虑你采用的是哪种模式，因为外部下载的资源有两种存在路径
 
 //对于go mod 的方式， 虽然不会去gopath/src下找资源， 但是会去goroot/src下寻找（联想一下， 因为fmt那些包就在那里）
 
go mod init demo 

go mod tidy 
//整理现有的依赖，使用此命令来下载指定的模块，并删除已经不用的模块 
//比如你的任意一个包中包含了外部链接
如：import("github.com/gin-gonic/gin"),但go.mod文件中没有， 使用此命令会去分析你的包中含有的外部链接并自动下载（会为你下载在gopath/pkg中，若使用以前的方法，是使用go get 然后会把资源下载在gopath/src中）。


exclude //用于go.mod文件中，表示不用某个版本， 使用go mod tidy 自动更新最新版本

replace A v1.0.0  => B v1.0.0     //右半部分可以换成本地路径
~~~



go build

~~~
主要用于编译代码，输出可执行文件，比如将源码打包成可执行文件部署线上服务
//如果是普通包(非main包)， 只做检查， 不产生可执行文件
//如果是main包，生成可执行文件， 默认生成的可执行文件名为项目名(go mod里面)

//命令： go build main.go

// -o 参数指定可执行文件名称

//交叉编译
在linux生成window   exe文件
GOOS=windows GOARCH=amd64 go build  -o demo.exe mian.go
反之
GOOS=linux GOARCH=amd64 go build  -o demo mian.go
~~~

go run

~~~c
比如 有两个包都是package main , 那么go run 的时候要把两个go文件都加入命令行参数
比如main.go用到了test.go内的函数， 并且他们都是package main 
如: go run main.go test.go
~~~

为什么需要go mod init demo  // demo 随便写成什么

~~~c
若是以前的方式，项目放在gopath/src中， 系统会默认给你的包中加上整个路径， 但若使用go mod的方法， 这个demo的名字就相当于替换了gopath/src  
~~~



## 反射

Golang提供了一种机制，在编译时**不知道类型的情况下**，可**更新变量**、运行时**查看值**、**调用方法**以及直接对他们的**布局进行操作**的机制，称为反射。

以结构体对象为例，比如c++中最多只能得到对象类型， 但go可以得到结构体中的每一个字段类型，已经字段名

再比如， go可以反射一个切片类型， 直接对这个切片的布局进行改变，比如增加元素等操作。



reflect.valueof 获取数据的值如果为空返回0  ,

 reflect.typeof 获取值的类型，如果为空返回nil

~~~go
对于 reflect.ValueOf 

若要修改对象的值:
var a int = 10;
reflect.Valueof(&a)
v := a.Elem();
v.SetInt(19)
// 以上两句可合并为 a.Elem().SetInt(19)



对于 reflect.TypeOf

Name() 结构体名字比如， student
Kind() struct 
NumField()结构体字段总数
Field(i),从0开始获取结构体对象所包含的key

~~~



### 普通对象反射

~~~go
x := 5
v := reflect.ValueOf(x)
fmt.Println(v.Kind()) //int
fmt.Println(v.Name()) // 错误，没有这种写法

t := reflect.TypeOf(x)
fmt.Println(t.Kind()) //int
fmt.Println(t.Name()) // int


type MyInt int	
var x MyInt = 5
v := reflect.TypeOf(x)
fmt.Println(v.Kind())	// int
fmt.Println(v.Name())   // MyInt

v := reflect.ValueOf(x)
fmt.Println(v.Kind())	// int
fmt.Println(v.Name())   // 错误，没有这种写法

拓展：
type MyInt int	
//NB的地方在于go可以通过这种方式给MyInt设置函数方法
func ( *MyInt) test() {
    fmt.Println("test")
}
~~~



### 结构体对象的反射

~~~go
//对于结构体对象
s := student{
    Name : "jt"
    Age : 18,
}
v := reflect.ValueOf(s)
fmt.Println(v.Kind()) //struct 
fmt.Println(v.Name()) // 错误，没有这种写法

t := reflect.TypeOf(s)
fmt.Println(t.Kind()) //struct 
fmt.Println(t.Name()) // student

如果把t := reflect.TypeOf(s)中的s换为 &s
v := reflect.ValueOf(&s)
fmt.Println(v.Kind()) //ptr
fmt.Println(v.Name()) // 错误，没有这种写法

t := reflect.TypeOf(s)
fmt.Println(t.Kind()) //ptr
fmt.Println(t.Name()) // 空白


对于TypeOf
//针对对象的某一个字段. 对象.Field(i).Name, 对象.Field(i).Type， 这里要注意只有Typeof对象的某一个字段有这两个方法，ValueOf没有。
// Name  string
v := reflect.TypeOf(s) //若换成&s,就没有下边的方法,除非加上Elem()
fmt.Println(v.Field(0).Name) // Name
fmt.Println(v.Field(0).Type) // string

对于ValueOf
//可以 v.Interface.(student)， 将reflect.Value对象转化为student
v := reflect.ValueOf(s) //和上边一样，若换成&s,就没有下边的方法,除非加上Elem()
fmt.Println(v.Field(0)) // jt
fmt.Println(v.Field(0).Interface()) // jt , 也对
fmt.Println(v.Field(1)) // 18



//不管是结构体对象还是普通对象， 如果是引用类型的对象， 那么就在对象后边跟上Elem()，就和 非引用 的情况一样。
//有一个使用Elem()的细节

typ := reflect.TypeOf(a)
val := reflect.New(typ).Elem()
//val := reflect.ValueOf(&a).Elem(), 这句和上边的两句等效，区别在于，上边两句是借助a对象的类型，也就是创建了一个student结构体类型的新对象， 而这句是直接用了a对象本身。

val.FieldByName("Name").SetString("jtsg")// 正确
val.FieldByName("Age").SetInt(20)// 正确

tp := val.Interface().(student) //必须借助一个临时对象转换一下，因为下边用到tp.Name, tp.Age， 这里有一个细节，如果上边没有加Elem(),这里就得变成val.Elem().Interface().(student) 
fmt.Println(tp.Name, tp.Age)

//如果把上边的注释打开， 就相当于修改了a的本体
//fmt.Println(a.Name, a.Age) // "jtsg", 20 这样我们就修改了a本体

如果把上边的
val := reflect.New(typ).Elem()
//val := reflect.ValueOf(&a).Elem()
换成
val := reflect.New(typ)
//val := reflect.ValueOf(&a)

那么
val.FieldByName("Name").SetString("jtsg")// 正确
就得换成
val.Elem().FieldByName("Name").SetString("jtsg")// 正确

~~~

**对比以上两种不同类型的反射**

~~~go
细节就是ValueOf出来的，没有Name()方法，所以一般直接用TypeOf方法来获取对象的类型(Kind()) 和 名字(Name())

不管对象是一个指针还是普通对象
Typeof都有Name(), Kind() 方法， 而ValueOf只有Kind()方法
如果是TypeOf的普通对象 //Name()表示student, Kind()表示student
如果是TypeOf的指针对象//Name()表示空白, Kind()表示ptr
如果是ValueOf的普通对象 //Name()报错, Kind()表示student
如果是ValueOf的指针对象//Name()报错, Kind()表示ptr

针对对象内的某一个字段 Filed(i)
TypeOf对象，Field(i).Type 和 Field(i).Name, 如果是指针对象，必须在前边加上Elem()，否则报错
ValueOf对象一样的道理，并且只能获取到具体的值，Filed(i) 或者 Filed(i).Interface()都可以， 如果是指针对象，就在前边加上Elem()
~~~





示例代码：

~~~go
package main

import (
	"fmt"
	"reflect"
)

type Student struct {
	Id   int
	Name string
}

func (s Student) Hello(){
	fmt.Println("我是一个学生")
}

func main() {
	s := Student{Id: 1, Name: "咖啡色的羊驼"}

	// 获取目标对象
	t := reflect.TypeOf(s)
    fmt.Println( "Name is", t.Name(), "and kind is", t.Kind())
    
    //如果把上边s 换成 &s
    t := reflect.TypeOf(&s)
    fmt.Println( "Name is", t.Elem().Name(), "and kind is", t.Elem().Kind())
    
	//打印结果
    //Type is Student and kind is struct
    
    // 获取这个结构体类型的大小 t.Size()

	// 获取目标对象的值类型
	v := reflect.ValueOf(s)
    
	// .NumField()来获取其包含的字段的总数
    //注意这里是t， 不是 v 的 NumField()
	for i := 0; i < t.NumField(); i++ {
		// 从0开始获取Student所包含的key
		key := t.Field(i)

		// 通过interface方法来获取key所对应的值
		value := v.Field(i).Interface()

		fmt.Printf("第%d个字段是：%s:%v = %v \n", i+1, key.Name, key.Type, value)
        //第1个字段是：Id:int = 1 
	   //第2个字段是：Name:string = 咖啡色的羊驼
	}

	// 通过.NumMethod()来获取Student里头的方法
	for i:=0;i<t.NumMethod(); i++ {
		m := t.Method(i)
		fmt.Printf("第%d个方法是：%s:%v\n", i+1, m.Name, m.Type)
        //第1个方法是：Hello:func(main.Student)
	}
}

~~~





Go的反射机制可以将“接口类型的变量”转换为“反射类型的对象”，然后再将“反射类型对象”转换过去。

反射第一大定律：

~~~go
func TypeOf(i interface{}) Type	//返回类型 ， 接口
func ValueOf(i interface{}) Value//返回类型， 结构体
~~~



反射第二大定律：func (v Value) Interface() interface{}

```go
func (v Value) Interface() interface{}
//func (v Type) Interface() interface{} //没有这个
```

**以下所有实例都是以这个结构体演示**

~~~go
//注意Name Age 首字母大写， 不然访问不到
type student struct {
	Name string		
    Age int
}
~~~



typeof和valueof 方法的相似之处

~~~go
它们都有Field方法
s := {age:18, name : "jt"}
v := reflect.ValueOf(s)	//值的方式不是引用
t := reflect.TypeOf(s) //值的方式不是引用

对于TypeOf ：t.Field(1) 表示获取到(age 和 int 这一行), 此时对应的就是， t.Field(1).name 为 age , t.Field(1).Type 为 int

对于ValueOf: v.Field(1) 表示 age的具体值，这里特别需要注意，不需要使用v.Field(1).Interface()转换， v.Feild(1)已经表示具体得age得值了，如果加上Interface()反而报错。

如果把值的方式换成引用的方式，就需要再Filed()前边加上 Elem()
~~~



**说明一点，以下内容涉及反射对象的操作， 对于ValueOf类型的反射，如果要修改实例对象的值， 那么必须传递引用类型(使用reflect.New() 将TypeOf对象转为ValueOf对象也是引用类型), 修改值就要用到Elem() 函数，先获取对象的地址，而对于传递值类型的反射，就坚决不能使用Elem()**



使用New创建一个Type类型的Value对象

~~~go
obj := Student{"jt", 18}
t := reflect.TypeOf(obj)
//此时t是Type类型

v := reflect.New(t) // v是一个Type类型的Value对象，t必须是一个非引用的Type类型， 假设上边是&obj,这里得把t改为t.Elem()

//这里需要特别注意New的作用相当于
v := reflect.ValueOf(&obj) //  注意这个&
//区别在于，上边两句是借助obj对象的类型，也就是创建了一个Student结构体类型的新对象， 而这句是直接用了obj对象本身。

~~~

反射中修改结构体对象的值

~~~go
// v 接上边部分
//错误演示
v.Elem().Field(0).SetString("tt")	
v.Elem().Field(1).SetInt(20)

//正确方式 特别需要注意，成员首字母必须大写，不然访问不到
v.Elem().FieldByName("Name").SetString("tt")
v.Elem().FieldByName("Age").SetInt(20)

//第二种方式，把上边的
v := reflect.ValueOf(&obj) 
//换成
v := reflect.ValueOf(&obj.Name) // 这样就相当于只是反射了一个字符串， Age同理
//就可以
v.Elem().SetString("tt") // 直接这样就可以修改值了
~~~

打印结构体反射对象(引用)的值

~~~go
以下说的都是反射一个对象的引用类型，而不是值类型

//打印整个结构体字段
f1 := v.Elem().Interface().(student)
//f1这里后边必须加上.(student) 表示f1是一个student结构的对象，因为后边访问了f1.name 和 f1.age 
f2 := v.Interface();

fmt.Printf("f2:%+v, %s, %d\n", f1, f1.name, f1.age)
//f1: {name:"jt" age:20}, jt, 18

fmt.Println(f1)// {jt 20}

fmt.println(f2) // &{jt 20} 多了一个&符号

//打印结构体对象的单个字段
f3 := fVal.Elem().Field(1).Interface() 
fmt.Println(f3) // jiangtao
~~~



反射中修改普通对象的值

~~~go
gret := "I like hh"

// 如果是反射一个引用对象
v := reflect.ValueOf(&gret)//这里必须用&， 也正是因为必须是一个引用对象， 下边才能使用Elem()方法，获取它的真实地址。
fmt.Println(v.Elem().Interface())// "I like hh" 
// 只有引用对象才可以使用Elem()方法修改对象的值
v.Elem().SetString("hh like me")
fmt.Println(gret) // "hh like me"


//如果是反射一个非引用对象
v1 := reflect.ValueOf(gret)
fmt.Println(v1.Interface())// "I like hh" 
~~~





### gorm

遇到的坑

~~~go
1.如果使用了select，那么update只能更新select中有的字段， 不会改变其他字段，特别注意，select的多个字段是分开的，不能放在一起，比如"age", "name", 而不是"age, name", 而where是放在一个引号中的，这一点需要特别注意。
23.表第一次创建好之后，后边修改以前某些字段的属性，不会生效，要么就用新的字段名要么就删除原来的字段结构
5.使用global.MysqlIns.Model(&model.UserGuideInfo{}).
		Where("user_id = ?", userID).
		//Select("new_hand_trigger_guide_info").
		Updates(model.UserGuideInfo{NewHandTriggerGuideInfo: triggerInfoStr})， 如果triggerInfoStr为空无法更新， 必须加上Select。或者用
var userGuideInfo model.UserGuideInfo
global.MysqlIns.Model(&model.UserGuideInfo{}).Where("user_id = ?", userID).First(&userGuideInfo)
userGuideInfo.NewHandTriggerGuideInfo = triggerInfoStr
global.MysqlIns.Save(&userGuideInfo)
6. 查询语句标明某个字段的时候要么啥都不加，要么加的是` `而不是 ' '
~~~



~~~go
2.gorm中同一张表是允许存在相同名字的索引的
1.只要第一次把执行了AutoMigrate创建了表结构，后边就可以不执行这个函数，直接修改表结构(增加或者删除某个字段)也会同步产生效果
2.通过First(&user)找到一条或者Find(&[]uesr)找到所有符合条件的，就可以通过这个user结构得到查询的结果，使用select对指定字段查询时，user只要与对应字段能够匹配上即可，不是必须要求查询出来的结构体和表的结构体完全一致。



package main

import (
	"fmt"

	"gorm.io/driver/mysql"
	"gorm.io/gorm"
)

type People struct {
	Age  int
	Name string `gorm:"default:'jt'"`
	// Birthday time.Time
	Ge     string `gorm:"default:'wh'"`
	Season string
	New    int
}

type user struct {
	Age  int
	Name string
}

func main() {
	// 参考 https://github.com/go-sql-driver/mysql#dsn-data-source-name 获取详情
	dsn := "root:123456@tcp(127.0.0.1:3307)/test?charset=utf8mb4&parseTime=True&loc=Local"
	db, _ := gorm.Open(mysql.Open(dsn), &gorm.Config{})
	db.AutoMigrate(&People{}) //自动生成表
	// Birthday: time.Now(),
	//user := People{Name: "Jinzhu", Age: 18, New: 123}
	//db.Create(&user1)	//添加一条记录

	user3 := &People{}
	db.Model(&People{}).Select("season", "name").Where("age = ?", 18).First(&user3)
	fmt.Println(user3.Season)
	if user3.Season == "" {
		fmt.Println("yes")
		db.Model(&People{}).Select("season", "name").Where("age = ?", 18).First(&user3).Updates(&People{
			Season: "S1",
		}).First(&user3)

	}
	fmt.Println(user3.Season)

	var str string		//读单个字段season
	db.Model(&People{}).Select("season").Where("age = ?", 18).First(&user3).Updates(&People{
		Season: "S1",
	}).First(&str)
	fmt.Println(str)

  var u1 user		//只要字段一致(People和user都有name和age字段)，就可以读到它里面
	db.Model(&People{}).Select("name", "age").Where("age = ?  and status = 1", 18).First(&user3).Updates(&People{
		Season: "S1",
	}).First(&u1)
	fmt.Println(u1)
}


4.计算一列值的和
var price int64
db.Table("orders").Select("sum(order_amount)").Scan(&price)  //但如果有空值就会出错，可以改用一下方式
//创建一个存储查询结果的切片
var result []int64
var sum int64
db.Table("orders").Pluck("order_amount",&result )

for _,v := range result{
			sum += v
}

~~~

使用select和不使用select的区别

~~~go
// 使用select
1.select没有选中这个字段， 这个字段的值就不会被读取出来，只能读出select选择的字段的值
2.select没有选中这个字段，就算新的结构有对应字段的值也不会更新
3.select选中这个字段， 新结构中该字段为空(只要不额外赋非空值，都算空)， 也能成功更新为空
4.跟3有关系 ，如果新结构体中并没有该字段(第三点是有字段，但没复制)， 则不会更新为空
// 不使用select
1.如果select没有选中这个字段，新的结构对应字段赋值为空，gorm 不会 把它更新为空
~~~

model函数的作用：决定查询哪张表， 建议是都加上，以免出现歧义

~~~go
如果不显示指定model， 会通过后边的Where、First、Find来判断model， 让程序知道操作哪张表，如果都没有就判断updates里面的(以第一个得到的model类型为准)

前置说明：mysql中现在只有people表， 没有user表

//错误案例
1. 
var userTest user
db.Where("age = ?", 18).Find(&userTest).Updates(&People{
			Season: "S7",
			Name:   "xx",
		})
//并没有user表，会默认用第一个Find得到的模型因为是user类型，所以会去查user表，但没有，会报错
2.
people1是people类型
db.Where("age = ?", 18).Updates(&user{
			//Season: "S7",
			Name: "gg",
		}).Find(people1)
//这里同样把updates里面得到的模型作为要查询的表了， 同样报错

//正确案例
1.
var userTest = &user{}
	if user3.Season == "" {
		db.Where("age = ?", 18).Updates(&People{
			//Season: "S7",
			Name: "gg",
		}).Find(userTest)
	} //这种写法也是ok的， 可以通过Updates得到model类型是people

如果user里面的字段和people里面的字段不对应， userTest对象只能得到people里面有的对应字段的值

~~~

model中是否加引用， 别的函数比如Find是否加引用??

~~~go
1.在 .Model() 方法里是否使用 & 并不会影响对应的数据库操作
2.在Find中需要引用对象， 因为需要将查询到的值赋值到对应的地址上
~~~







### go垃圾回收

GC算法

~~~shell
最常见的垃圾回收算法有标记清除(Mark-sweep)和引用计数(Reference Count), go采用的是标记清除法，并在此技术上使用了三色标记法和写屏障技术提高了效率
~~~

**标记清除算法问题**

~~~
标记清除算法的一大问题是在标记期间，需要暂停程序（Stop the world，STW），标记结束之后，用户程序才可以继续执行。为了能够异步执行，减少 STW 的时间，Go 语言采用了三色标记法。标记期间需要暂停程序(stop the world,STW)
~~~

**三色标记法(存在假阴性)：**

~~~shell
根(root区域) 主要是指程序运行到当前时刻的栈(线程栈)和全局数据区域(静态变量，全局变量，常量)

标记开始时，所有对象加入白色集合（这一步需 STW ）。首先将根对象标记为灰色，加入灰色集合，垃圾搜集器取出一个灰色对象，将其标记为黑色，并将其指向的对象标记为灰色，加入灰色集合。重复这个过程，直到灰色集合为空为止，标记阶段结束。那么白色对象即可需要清理的对象，而黑色对象均为根可达的对象，不能被清理。当把灰色节点全部遍历完时， 仍在白色节点中的就是不可达对象也就是垃圾。
~~~

**三色标记法用到2个机制，写屏障和辅助GC**

~~~shell
1. 写屏障：STW的目的是防止GC扫描时，内存变化引起的混乱，写屏障就是让goroutine与GC同时运行的手段，虽然不能完全消除STW，但可以大大减少STW的时间，每轮GC开始会初始化一个叫做"屏障"的东西，然后由它记录第一次scan时各个对象的状态，以便第二次re-scan进行对比，引用状态变化了的对象被标记为灰色，即本轮不回收，下次GC时再确定，状态未变化的就正常进行处理。
case:
在 GC 过程中，对象指针发生了改变即C删除了对D的音乐，而A获得了D的引用(假设A已经处理过，这一轮不会再被处理，那D就没有机会回收了)，所以有了写屏障之后，D会直接标为灰色。
A (黑) -> B (灰) -> C (白) -> D (白)
↓
A (黑) -> B (灰) -> C (白) 
↓
A (黑) -> D (白) + B (灰) -> C (灰) (D轮空，所以需要写屏障技术)
2. 辅助GC：为了防止内存分配太快，而GC又相对较慢，回收的速度跟不上分配的速度，那么它依然会把用户逻辑暂停(STW)，用户逻辑暂停了以后意味着不会又新的对象出现，同时会把用户线程抢过来协助GC做一部分的工作，加快垃圾回收的速度，这就是辅助回收机制
~~~

**一次完整的 GC 分为四个阶段：**

~~~shell
1. 标记准备(Mark Setup，需 STW)，打开写屏障(Write Barrier)
2. 使用三色标记法标记（Marking, 并发）
3. 标记结束(Mark Termination，需 STW)，关闭写屏障。
4. 清理(Sweeping, 并发)
~~~

**引用计数：** c++的智能指针
1.维护引用计数的开销较大：通过原子操作保证计数的原子性
2.环形引用问题(用weak-ptr)
3.内存开销，每个对象都引入额外的内存空间存储引用数目
4.回收内存时可能引发暂停，比如一些大的数据结构

**分代GC（C#）：**
按照对象生命周期长短划分不同的代空间， 生命周期长的放入老年代，短的放入新生代，不同代有不同的回收算法和回收频率，优点是回收性能好，缺点是算法复杂，C#和Java就是这种， C#会综合使用不同的技术，如标记清除(Mark-Sweep)

### 设计模式

工厂

~~~go
package main
import "fmt"
// Shape 接口定义
type Shape interface {
    Draw()
}

// Circle 结构体实现 Shape 接口
type Circle struct{}

func (c Circle) Draw() {
    fmt.Println("Circle Draw")
}

// Rectangle 结构体实现 Shape 接口
type Rectangle struct{}

func (r Rectangle) Draw() {
    fmt.Println("Rectangle Draw")
}

// ShapeFactory 工厂接口
type ShapeFactory interface {
    CreateShape() Shape
}

// CircleFactory 结构体实现 ShapeFactory 接口
type CircleFactory struct{}

func (cf CircleFactory) CreateShape() Shape {
    return Circle{}
}

// RectangleFactory 结构体实现 ShapeFactory 接口
type RectangleFactory struct{}

func (rf RectangleFactory) CreateShape() Shape {
    return Rectangle{}
}

func main() {
    // 使用 CircleFactory 创建 Circle 对象并调用 Draw 方法
    circleFactory := CircleFactory{}
    circle := circleFactory.CreateShape()
    circle.Draw()

    // 使用 RectangleFactory 创建 Rectangle 对象并调用 Draw 方法
    rectangleFactory := RectangleFactory{}
    rectangle := rectangleFactory.CreateShape()
    rectangle.Draw()
}
~~~

