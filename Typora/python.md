小知识

~~~python
46.python本身对数字长度没有限制，只要内存允许，但有些协议指定了int32则另谈
45.python没有switch
44.print打印有缓冲问题，print("test", flush=True)避免缓冲
43.print(b''.decode('utf-8'))没问题
42.if、for、while不会创建新作用域，所以if里面第一次出现的变量也能在if外直接用，python作用域规则只有在函数内或者类中会创建新作用域
41.字典的key可以是数字也可以是字符串，'12'和12是不同的key
40.对于pb数据rpush的时候必须用SerializeToString转为字节序，rpush只接受string或者数字传参，再用ParseFromString进行解析
39.rpush字符串或者字节序等价，存入的是字符串，取时会转为字节序， 如.rpush('jt', '你')等价.rpush('jt', '你'.encode('utf-8')), lpop时候得到的是 b'\xe4\xbd\xa0'
38.'""','true','123','null'这些都是json字符串，不是必须'{}'或者'[]'，但空字符串、"test"、"汉字"不行
37.对象必须定义才能用，比如b不行必须是b = 0才能print(b)
36.比如某函数参数为(*args, param = false), 那么传参时就传(param = true)来指定param这个变量的值
35.dir(对象/类)可以看到这个对象/类的所有的方法(内置或自定义)
34.for表达式中还可以做一层if判断如：[ x+y for x in 'abc' if x!='a' for y in 'lmn' if y!='l']
33.字符串中false,null是可以被json.loads认出来的，比如'{"test": false}', 而False不行,False是python独有的，在线json解析中也不认识False
32.base64.b64decode传入二进制或者字符串形式都可以，base64.b64encode必须传字节序
31.引入上级目录的包sys.path.append(os.path.abspath('..'))
30.for item in d.items();item[0]为key[1]为value, for key, value in d.items()也可
29.redis的value，设置的时候是int，取出来也是str类型的字节序，即使 Redis 在内部使用了 int 编码存储这个整数值，客户端获取到的值仍然是字符串的字节序形式
28.json.loads(info.get('field', '{}'))，用'{}'和"{}"都可，主要是里面内容要用"", 参数可以是字节序形式
27.if param is None 和 if not param不一样， 前者明确要求是None对象，后者只要为空就成立(比如"",[],{}等)，如果明确知道返回对象是None就用前者，避免为""等其他情况出现误判, 后者的用法更常见一些
26.import pdb, 运行到pdb.set_trace会进入类似gdb调试的环境，方法一样
25.isdigit()只能判断整数，浮点数不行
24.r开头的字符串不会解释转义字符比如 str = r'"123\\"'打印出来就是"123\\"
23.字典和list要转为json字符串必须使用json.dumps, 就算dict对象中用的双引号也不行，直接str(dict对象)得到的是是单引号, 比如redis设置一个value时，如果不dumps，存进去的字符串就用的是单引号，那么loads加载它时就会报错, 如果是[1, 2]就没事，如果是['1', "2"](单双引号都不行)，就必须dumps，如果存储进去的是一个双引号的json字符串就没事，比如hmset('jt', 'tt', '{"test": 1}')
22.赋值语句中用and，会返回第一个为假的值，如果所有值都为真，就返回最后一个值，这就是短路求值，比如a = 4,b = 5,print(a and b) 打印结果为5， 如果是用or就是找第一个为真的值，如果都为假酒返回最后一个值，int(2 or 0 and 1)返回2，or运算第一个值为真值就直接返回了，不会看or 0和and 1了，除非把(2 or 0)括起来
21.可以自定义简单的proto协议通过ParseFromString解析别的复杂proto协议拿到感兴趣字段内容
20.格式化字符串中的:用于指定格式化方式如f'{b:02x} 而函数参数的中的:只是提示作用
19.下载包用pip比如pip install kafka-python，python3用pip3
18.json对象也是可迭代对象，跟字典一样的用法，items()返回一个元组(key,value)
17.json字符串只能是'{"test": 1}'，不能是'test'，因为json要求双引号包裹字段
16.getattr可以获取任何对象的属性，支持默认值，HasField用于protobuf中
15.json的写法类似于python的字典，比如{'test':123}，这个引号不能少，不像dict方法
14.打印某个import的模块位置，比如import fluent再print(fluent.__file__)
13.python的'_'和go中一样，表示占位
12.pb定义的字段名长啥样，python中用的时候就用啥样, 比如user_id
11.pb协议判断某个字段是否有值用.HasField("field")
10.pass和...都可以表示占位符
9.int类型可以表示任意大小的整数，只要内存允许，不区分什么long之类的
8.可迭代对象分割 ', '.join(f"{key} = ${key}" for key in 可迭代对象)
7.''和""没有区别，详情看笔记
6.id(变量名) 可以打印变量的地址
5.当执行list1 = [1, 2, 3] 时，Python会在内存中创建一个列表对象 [1, 2, 3]，然后list1会引用它，list2 = list1，那么list2也会引用这块内存，此时如果执行list1 = list3，会指向list3引用的内存，不影响list2继续引用[1, 2, 3]。
4.列表原地修改返回None，比如L=L.append('a')，会导致L为None, go中的append返回的是一个新的切片地址, 对于原地修改的都不要去直接等于返回值, dict的update也是None
3.深拷贝用copy.deepcopy(对象)，copy.copy(对象)是浅拷贝
2.python支持多返回值，本质上是通过元组实现的
1.del 变量名 # 删除变量
~~~

35.时间相关

~~~python
import time
import datetime
print(time.time()# 1726041712.1244211 
print(int(time.time()))# 1726041712
print(datetime.datetime.now())# 2024-09-11 16:01:52.126141
print(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))# 2024-09-11 16:01:52
      
# 计算时间差
starttime = time.perf_counter()
endtime = time.perf_counter()
timedifference = endtime - starttime
print(f"修改后执行时间差: {timedifference:.6f} 秒")
~~~

34.模块包

~~~python
2.包的目录必须要有__init__.py这个文件(可以没有内容)，否则导入无效
1.import的如果不是文件而是目录则成为包
~~~

33.模块

~~~python
7.当对导入比如上一级可以 from .. import module
6.不同python进程有独立的地址空间，比如两个python程序同时引用同一个模块module，比如该模块有变量a = [0]，就算是可变对象，这两个进程的module.a也是各自独立的，此时如果把module内容改为a = [0, 1], 这两个进程加载的内容还是a = [0], 此时如果进程A重新reload了module，那么进程A就会重新加载为a = [0, 1]，而进程B还是a = [0]
5.重新加载一次模块内容(比如中途进行了修改，有点热更的意思)，
import importlib 
importlib.reload(模块名)
4.import相当于一个变量名引用了整个模块对象，比如import module1，那么之后就可以用module1.filed来访问它里面的所有成员，而from module1 import a， 只是用变量a来指向了module1中变量a指向的内容(相当于多了一次引用)，from module1 import * 相当于在本文件中重新定义了一遍变量名并和module1中的变量指向同样的内容
3.用import或者from module import 来引用别的模块
2.不能存在循环引用
1.模块的搜索路径都在sys.path中
~~~

32.内存管理

~~~python
对象存储在堆区,变量是对对象的引用
比如a = 10, b = 10,实际上只有一份数据10存储在堆上，变量a和变量b在栈上，它们指向相同的对象, 使用print(id(a))、print(id(b))可以看到结果相同。
# 跟c++不一样的是，int a = 10，10直接存储在栈中，a是对这块区域的修饰名，int b = 10是不同的栈区域
~~~

31.作用域

~~~python
2. 函数中如果不用global那么变量就是局部的，变量名作用域查找顺序为:本地->上一层def->全局->内置
（LEGB 规则）
Local: 当前函数的局部作用域。
Enclosing: 外部嵌套函数的作用域。
Global: 全局作用域。
Built-in: 内置作用域。
1. global会把变量名作用域提升至全局，nonlocal会将变量名作用域提升至外层的def(外层必须也是def)
case1:
y = 33
def f1(*args, test = False):
    x = 10
    global y
    def f2():
        nonlocal x
        print(x, y)
    x = 20
    return f2()
f1() # 20, 33
case2:
def func():
  a = 0
  b = 1
  def func1():
    nonlocal a,b # 限定了查找只能在func1所在的作用域（即func的本地作用域），且要求名字a,b已经存在。
  	a='a' #对a,b的赋值会影响到func中的a,b
case3:
y = 33
def f1(*args, test = False):
    global c
    c = 20
f1()
print(c) # 20 如果此行跟f1()换位置就报错
~~~

30.函数

~~~python
10.可变对象要避免作为默认传参， 
def func(x = []):
  print(x)
  x.append('x')
func() # []
func() # ['x']
func() # ['x', 'x']
9.静态检测局部变量
x='global'
def func():
  print(x) # 会报错，python认为此函数中有局部变量x，但执行这里时发现x还未声明，尽管有全局的也不行
  x = 3
8.函数注解只有在def中有效， lambda中无效
def func(a:'a', b:float = 4.0, c:float) -> int:
  return a + b + c
7.函数递归要比for循环慢并且空间消耗大，但优点是可以遍历人意形状的结构
6.函数闭包：核心它不是记录了变量的值，而是追踪了这个变量
case1:
def f1():
  x = 99
  def f2():
      print(x)
  x = 12
  return f2
action=f1()
action() # 12
case2:
def func():
  acts=[]
  for i in range(5):
      acts.append(lambda x:i**x)
  return acts
acts=func()
acts[0](2) #调用时才开始查找i,此时i最后被记住的值是4
5.def语句可以出现在任何语句可以出现的地方，比如if语句执行后就可以写个def，def内嵌套def也可以
4.return可有可无，没有时默认返回None
3.函数对象允许任意的熟悉添加，比如有函数testfunc, 可以test.func.filedname = 1
2.函数是对象类型(和dict、list一样)，def执行完后生成一个函数对象，函数名只是这个函数对象的一个引用
1.函数内的代码在运行时才会解析，也就是说写了不存在的变量(如a = b/return A()，但b未声明，类A也不存在)只要不执行这个函数，并且函数内没有语法错误，就不会报错
~~~

29.生成器

~~~python
生成器函数运行代码随时间产生一系列的值，而不是一次性计算它们。这会节约内存并允许计算时间分散
def genFunc(num):
    for i in range(num):
        yield i**2

gen = genFunc(3) # 返回一个迭代器
print(gen.__next__()) # 0 等效于print(next(gen)) 
print(gen.__next__()) # 1
print(gen.__next__()) # 4
print(gen.__next__()) # 抛异常
~~~



28.迭代器

~~~python
4.常用方法：sum(list对象)求和，any(iterable)任一为真，all(iterable)所有都为真
3.map()、sorted()、filter()等内置函数底层都是用.__next__()来迭代的，它们都返回的是迭代器对象，但range()返回的不是迭代器，可用dir()看到它没有.__next__方法
2.任何对象只要实现了.__next__()，则它就是一个迭代器对象，文件对象就是一个迭代器对象，而dict和list等只是可迭代对象，本身不是迭代器，需要使用iter(可迭代对象)来得到自己的迭代器
1.可迭代对象不一定是序列(如dict)，但是序列一定是可迭代对象(如list)
~~~

27.可变对象和不可变对象

~~~python
1.Python中，所有变量都是通过引用传递的，只是python会区分可变对象(list、dict、set)和不可变对象(int、str、元组)，可变对象修改才会影响外部,自定义对象是否可变取决于它的成员变量是否可修改(比如设置了@property并且未提供修改方法则为不可变对象)
2.Python 的这种行为可以类比于 C++ 中传递一个 const 变量。尽管传递的是引用，但由于对象本身是不可变的，所以不能在函数内部修改它。
3.这种机制是其语言的特殊性，通过严格区分可变对象和不可变对象，确保了不可变对象在函数内部的修改不会影响到外部变量

case:核心是要理解lst和my_list是两个变量
def modify_list(lst):
    lst.append(4)  # 修改了原来的列表对象
    lst = [5, 6, 7]  # lst 现在引用一个新的列表对象
    print("Inside function:", lst) # [5, 6, 7]

my_list = [1, 2, 3]
modify_list(my_list)
print("Outside function:", my_list) # [1, 2, 3, 4]
~~~

26.获取redis数据

~~~python
Redis 是一个键值存储系统，所有数据都以字节串的形式存储。
当你从 Redis 获取数据时，无论数据的原始类型是什么，它们都会以字节串的形式返回。
case:
hgetall得到是字典，但key和value都是字节序如{b'grade': b'5'}，用以下方式可以转出来
str = {k.decode('utf-8') : v if v is None else v.decode('utf-8') for k , v in result.items()}
print(json.dumps(str))
~~~

25.装饰器

~~~python
1. 装饰器可以带来更好的封装、数据验证、只读属性、延迟计算和灵活性，C#中也有这种机制，在get和set中添加逻辑。
2. 不使用装饰器可以随意修改类中的属性，没有任何验证或逻辑控制
3. property装饰器：用于将类的方法转换为属性，允许定义 getter、setter 和 deleter 方法。它是一个描述符，本质上是生成一个property类对象赋值给类成员比如val，让val拥有这个类的方法如果没有对应的方法比如val.setter，那这个val的值就不能改变了
4.wrapper装饰器(函数)：接收一个函数对象，并返回一个新的函数对象
5.类装饰器(类)：接收一个函数对象，并返回一个新的函数对象
def add_repr(cls):
    cls.__repr__ = lambda self: f"{self.__class__.__name__}({self.__dict__})"
    return cls
@add_repr
class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age
p = Person("Alice", 30)
print(repr(p))  # 输出: Person({'name': 'Alice', 'age': 30})

~~~

24.初始化对象

~~~python
__init__，一个类对象创建的时候会执行这个方法
# 传入多个参数(固定数量)
class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age
person = Person(name="Alice", age=30)
# 单星号 （元组
def example(*args):
    print(args)
example(1, 2, 3)  # 输出: (1, 2, 3)
# 双星号（字典
def example(**kwargs):
    print(kwargs)
example(a=1, b=2, c=3)  # 输出: {'a': 1, 'b': 2, 'c': 3}
# 单双星同时使用
class Example:
    def __init__(self, *args, **kwargs):
        self.args = args
        self.kwargs = kwargs
# 创建实例
example = Example(1, 2, 3, a=4, b=5) # 如果再加一个参数6就报错
print(example.args)   # 输出: (1, 2, 3)
print(example.kwargs) # 输出: {'a': 4, 'b': 5}
~~~

23.下划线区别

~~~
单下划线前缀（_var）：表示受保护的变量或方法，表示不建议外部直接访问。
双下划线前缀（__var）：用于名称改写，表示类私有变量或方法，避免子类覆盖或访问。
双下划线前后缀（__var__）：用于特殊方法或变量，通常由 Python 内部使用或定义。
~~~

22.range

~~~python
2.for val in range(5) # 实质上是0-4，从0开始，并且不包含5
1.range返回的是一个可迭代器对象(非迭代器)，range(0,10,2) 分别表示起始、终止、步长，有三种形式，可以只有1个或2个参数，并且是惰性求值的
2.range 函数生成的是一个 range 对象，这个对象在内存中不会存储所有的整数，而是按需生成每个整数。这种惰性求值的特性使得 range 在处理大范围的整数时非常高效。
r = range(1000000)
l = list(range(1000000))
print(sys.getsizeof(r))  # 内存大小：48
print(sys.getsizeof(l))  # 内存大小：8000056
~~~

21.真值

~~~
有个说法是被视为 False，但它并不等于布尔值 False
以下是在bool上下文中被视为False的值，但只有0/0.0/0j和False相等
None
0
0.0
0j
''（空字符串）
[]（空列表）
()（空元组）
{}（空字典）
set()（空集合）
frozenset()（空冻结集合）
~~~

20.赋值

~~~python
1. 左边和右边的数量要一致，Lua中可以不一致
2. 配合*来实现左右数量不一致
a, *b = "abcd"，则b = [b, c, d], 左边只能有一个带有*号

对于不可变类型（如整数），a += 1 和 a = a + 1 是等价的，并且都会生成一个新的对象。
对于可变类型（如列表），a += [1] 会修改原来的对象，而 a = a + [1] 会生成一个新的对象。
~~~

19.utf-8编码

~~~python
char = "你"
utf8_bytes = char.encode('utf-8')
print(utf8_bytes)
print(' '.join(f'{b:02x}' for b in utf8_bytes))
# 打印结果
b'\xe4\xbd\xa0'
e4 bd a0
# 代码解释
f'{b:02x}' 表示将变量 b 的值格式化为两位小写十六进制字符串，不足两位时用零填充
~~~

18.文件操作

~~~shell
# 读取
filehandle = open(filename, 'r/w/a')
.read() # 读取整个文件，返回一个字符串
.read(n) # 读取n个字节到一个字符串
.readline() # 读取一行到一个字符串
.readlines() # 读取整个文件，一行一个字符串，可以用for迭代

# 写入
with open('result.txt', 'ab') as f:
  for message in messages:
    f.write((message + '\n').encode('utf-8'))

~~~

17.基本数据结构总结

~~~python
# 不可变说的是不能修改对象，如增加删除序列元素
python的通用序列包括:字符串、列表、元组
字符串：不可变对象，元素只能是字符
列表：可变对象，元素类型不限
元组：不可变对象，元素类型不限(比如list，list值变化，元组跟着变)
序列通用操作有：1.索引 2.分片 3.加法 4.乘法

# set和dict不是序列
集合不是序列，它是可变对象，但是元素只能是不可变类型。
字典也不是序列，它是可变对象，其元素的值是不限类型，但是键必须是不可变类型。

# 拷贝(以list举例，其他都一样)
list1 = [1, 2, 3]
list2 = [1, list1, 3]
list3 = copy.copy(list2)
list4 = list(list1) # 也是浅拷贝，其他的例如set(),dict()也一样
list5 = list4 # 这不是拷贝了，二者引用同一块内存，引用类型理解为c++的指针就行
如果修改list3[0]=2，那么list2[0]依然是1, 而如果修改了list1, 则list2[1]和list3[1]都会跟着改变，这就是浅拷贝，可以理解为c++中拷贝一个类对象的时候，类的普通变量(比如int类型)也都是独立的，但如果是指针类型就是公用的
~~~

16.lambda表达式 (lambda 参数:做的事)

~~~python
# 1.配合filter使用, filter返回迭代器所以必须用dict或list等接收
dic = {'a':3,'b':4}
dic_test = dict(filter(lambda item: item[0] == 'a', dic.items()))
# 2.配合sorted的排序规则(key=str表示字符串排序，key=int表示整数排序),最后一个参数reverse默认为False
words = ["apple", "banana", "cherry", "date"]
sorted_words = sorted(words, key=lambda x: len(x))
# 3.配合map使用(返回迭代器，工作原理是遍历传入的可迭代对象)
numbers1 = [1, 2, 3]
numbers2 = [4, 5, 6]
summed_numbers = map(lambda x, y: x + y, numbers1, numbers2)
print(list(summed_numbers))  # 输出: [5, 7, 9]
numbers = [1, 2, 3, 4]
squared_numbers = map(lambda x: x ** 2, numbers)
print(list(squared_numbers))  # 输出: [1, 4, 9, 16]
~~~

15.元组(tuple)

~~~python
对于python的集合(set)不支持放入可变元素如dict和list， 但set本身是可以增加或者移除元素的，而元组刚好相反，它可以放入可变元素，但在初始化的时候就决定好了元素的个数，后续不能再增加或减少。

list1 = [1, 2, 3]
list2 = [4, 5, 6]
set1 = {1, 2, 3} 
tup = (list1, list2, set1)
print(tup) # ([1, 2, 3], [4, 5, 6], {1, 2, 3})
# 此时tup的元素不能再变了，但元素的值可变，比如list1.append(7),那么tup也会跟着变化

# 创建
set1 = {1, 2, 3}  # 注意这是集合set而不是字典dict 
tup = tuple(set1)
print(tup) # (1, 2, 3) ，用list也是一样的结果

# 1.支持索引访问 
tup[0]
# 2.支持分片访问
tup[:2]
# 3.len返回长度
len(tup)
# 4.判断是否存在
3 in tup
# 5.可迭代
for item in tup:
  print(item)
# 6.搜索值所在位置
tup.index([1, 2, 3])
# 7.统计出现次数
tup.count([1, 2, 3])
~~~

14.列表操作（不限制对象类型）

~~~python
1. 和字符串功能接近，支持相加，支持与整数想乘，支持索引, 支持分片
2. 迭代
list1 = [1, 2, 3, 4]
for item in list1
3. count(val)是否包含，reverse翻转，sort排序，append追加，extend(参数为可迭代对象)插入多个元素，pop末尾弹出，pop(idx)弹出指定位置元素，del list[idx]删除置顶位置， pop和del区别在于，del不会返回被删的元素
# 通过值删除(处理找到的第一个)
.remove(val)
list1 = [1, 2, 3, 2, 4, 2, 5]
list1 = [x for x in list1 if x != val] # 删除所有
# 查找某元素位置(处理找到的第一个)
.index(val)
# 指定位置插入
.insert(index,val)
~~~

13.解析带有proto序列化数据的json内容

~~~python
# 感悟:其实str、proto和json属于三种不同的类型，如果要把str转为proto就得先ParseFromString， 要把str转为json就得先json.loads

import json
from google.protobuf.json_format import MessageToDict
from service_pb2 import PlayBackPackage # 需要解析的proto源

def function():
  # 如果playback对象是字符串，需要先执行
  # entity=PlayBackPackage()
  # entity.ParseFromString(result['playback'] )
  # SerializeToString表示序列化为string，然后用上边这个反解
  
   entity = result['playback'] 
  # 判断内容是否是某个proto类型对象
  if isinstance(entity, PlayBackPackage):
  # 将Protobuf对象转换为字典
   	dict = MessageToDict(entity,preserving_proto_field_name=True)
  # 将字典对象转为json串
  	result_json = json.dumps(dict, ensure_ascii=False, indent=4)
  # 将字符串写入文件
  	with open('jt.json','w',encoding='utf-8') as f:
      f.write(result_json)
      
~~~

12.各类型转换

~~~python
# 将字符串转换为字节
if isinstance(seq, str):
    seq = seq.encode('utf-8')  
# 将字节转换为字符串
if isinstance(seq, bytes):
    seq = seq.decode('utf-8')  
~~~

11.初始化

~~~python
class PlayBackEntity:
    def __init__(self):
        self.mainuid = -1              
        self.other_attribute = None     # 其他属性
        
# 调用
if __name__ == "__main__": # 如果是直接执行该文件__name__就会是"__main__", 如果不要这句，被别的模块导入时也会执行以下内容
  entity = PlayBackEntity()
  print(entity.mainuid)

~~~

10.异常捕获

``` python
try:

except Exception as e:
    print(e)
```

9.字符串操作

~~~python
1.len() 返回长度
2.支持加， 与整数N相乘即原字符串重复N次
3.支持分片, S[m:n], 从m开始到n(不包含n), -1表示最后一个字符, S[:]为所有, S[m:n:k]中的k表示k步取一个元素(k为第一个元素), 如果k为负数则从右往左数,S[::-1]为反转字符串
4.常用in判断是否在某字符串中出现，如 if 'jt' in 'jttest':
~~~

8.""和''的关系

~~~python
字符串用""和''包起来没有本质区别，主要是方便在字符串中包含引号而不需要转义(如果包含\本身还是需要转义的)，比如字符串中包含单引号就用双引号来包裹字符串，反之一样。
print(str)  # 打印字符串的内容，不含最外层引号
print(repr(str))  # 打印字符串的表示形式，含最外层引号，
case1:
str='who\'s jiangtao\\\''
print(str) # who's jiangtao\'	 不带最外层引号
print(repr(str)) # "who's jiangtao\\'"  带有最外层引号，但这里第一个\是给第二个\转义的，'不需要转义，因为是用""包着的, ''包"也一样不需要转义
case2:
str="who's jiangtao\"\""
print(str)  # who's jiangtao"" 
print(repr(str)) # 'who\'s jiangtao""'  # 可以看到原字符串是用""包着的，打印出来的却是''包着的， python会自动选择合适的引号确保正确且可读

# 如果希望字符串中包含引号，则有两种办法：
最外围的引号和字符串中的引号使用不同的格式。如"who's name is jt?"
采用转义字符, 如'who\'s name is jt?'

# case
str = '"123\"' # 是否加\打印出来都是"123"
~~~

7.字典(只能判断相等，无法比较大小, 不要求key是同类型)

~~~python
# key的值可以是字符串或者数字，'3'和3是不同的key
1. 创建 
# 直接定义
d = {3:3,'3':3,'a':3}
# dict方法
d = dict(a=3, b=4) 生成字典 {'a':3, 'b':4}, 也可以配合zip比如dict(zip(['a','b'],[3,4]))实现同等效果

dict中不能用数字或者带引号的值(如3, '3', 'a')作为key，但zip可以，比如d = dict(3='a', a=3, '3'='3')是错误的(我理解是dict中对于key是没加引号的，比如字符a直接写的就是a，数字3和字符3就会有歧义, key不能加引号比如'a')，d = dict(zip(['a',3,'3'],[3,'a','3']))是正确的

# .fromkeys()方法
比如dict.fromkeys(['3',3],6)生成字典{'3':6,3:6}，如果不要6就是{'3':None,3:None}


# 生成字典
{k:v for k,v in zip(['a','b','c'],[1,2,3])}，
{k:v for k,v in [('a',1),('b',2),('c',3)]}

2. 索引
d[key]
3. 判断是否存在
key in d
4. 迭代
d.keys()返回key序列，d.values()返回值序列，d.items()返回(key, value)序列
5. 浅拷贝 
d.copy
6. 获取键的值
d.get(key, default_value) 如果不存在则返回default_value,如果是d[key]的方式，如果key不存在则抛出KeyError异常
7. 字典的操作
d1.update(d2) # 合并，d2存在d1不存在则加入d1，d1存在d2不存在不管，都存在就用d2的值更新，
d.pop(key) # 删除key并返回该元素的值
del d[key] # 删除key但是不返回该元素的值
d[key]=value # 存在则修改，不存在就增加
8. 迭代
for key in d:  # 默认只返回key
    print(d[key]) # print(key,'==>',d[key])
for key, value in d.items():
    print(key, value) 
~~~

6.set (唯一、不可变无序、可迭代、不支持索引、不要求各个元素都是相同类型)

~~~python
1. 创建 (x=set(iter)) # iter为任何序列或可迭代对象
x=set([1,2])
y=set([3,4])
{1,2,3,4}等价于set([1,2,3,4]), 空set必须用set([])或set({}), {}为空字典, []为空list，单独用[]、{}都不表示空set
2. 增加 .add('a')    移除 .remove('a')  
3. 包含 e in x (返回bool)
4. 并集 (x | y)
.update(iter) # 比如iter为'abc'，则集合中会多'b','c'
5. 交集 (x & y)
.intersection(iter) # 返回新的set对象，iter为任何可迭代对象
6. 异或 (x ^ y), 超集(x > y), 子集(x < y)
7. set只能包含不可变的对象，因此列表对象、字典对象、set对象均不能作为set的元素。
8. 遍历set并且赋值
# 第一种方式
result = [v for v in 集合] # 第一个v表示要放入result的值，如果把第一个v改为str(v)，那么result的到的结果就全是字符串形式
# 第二种
result = []
for x in setX:
    result.append(x)
~~~

5.简单计算

~~~python
5.三元表达式 Y if X else Z,当X为真时，表达式为Y，反之表达式为Z (X ? Y : Z)
4.复数比如 z = 3 + 4j, 3是实数，4是虚数，j是虚数的后缀
3.x**2表示x的2次方
2. 两个除号 '//' 表示floor向下取整，math.ceil()向上取整
1.python3中，除法(不管是整数除法还是float除法)都会保留小数部分，python2.7整数除法不会保留小数部分。
~~~

4.数字类型转换

~~~python
1.整数转为字符串：hex(intx), oct(), bin(),str(), 将intx转换为16/8/2/10进制表示的字符串，也可以格式化比如'%x' % 64结果为'40'
2.字符串转为整数：int(strx, base) 将字符串strx根据base进制转换为整数，默认为10， 也可以用eval比如eval('0x40')结果为64
3.float("10.1")	 str转为转换浮点型
~~~

3.python可以在旧版本中开启新版本的特性

~~~python
from __future__ import 特性名
~~~

2.None是特殊的python对象，总是False，None是所有函数和方法的默认返回值

1.两种格式化方式

~~~
print("%d,%d"%(1,2))
print("{},{}".format(1,2))
~~~

2.

x = [3,4,5,1,2]

y = x[ : ]     y得到的是x的副本，x.sort(),y不会改变

z= x  		z与x指的是同一个列表，x.sort()，x.append(), z会跟着改变，但如果对x重新赋值， z不会跟着边，只有对x进行修改的时候如x.sort()，x.append()，z才会跟着变化

3.

python中的 () 和 [] 所以可以通过下标0开始
但对于字典{}, 索引必须要有键值，不能从0开始

### 脚本

str.strip(); 删除字符串前后的空格或特殊字符
lstrip 左边， rstrip 右边

splice = line.split("\t", 1)  以"\t"进行分割， 仅分割一次
line = splice[2]

json和字符串转换

~~~python
#简单使用
字符串转json
json_list = json.loads(string)

json转字符串
new_string = json.dumps(json_list,ensure_ascii=False,indent=2)
#ensure_ascii=False是为了防止中文乱码， indent=2, 2表示换行后的前置缩进，打印好看

import json

str = '''
[{
    "name": "Tom",
    "gender": "male"
}, {
    "name": "Jack",
    "gender": "male"   
}]
'''
#将字符串转为json格式
print(type(str))
data = json.loads(str)
print(type(data))
data["江涛"]="666"
str = json.dumps(data, ensure_ascii=False)
print(data)
print(str)

# 文件中的使用
import json
# 从文件中读取原始的 JSON 数据
with open('/Users/a123/project328/gm-tool/dealDivideLevel.json', 'r') as file:
    json_data = file.read().replace('\n','')
#重点是合理使用replace，如果原文件就是json对象形式并且每行都换行了，就可以用   
    
# 将 JSON 字符串解析为 Python 对象， 如果是json对象而不是字符串就不用这行
#data = json.loads(json_data)

# 将 Python 对象转换为 JSON 字符串（包含缩进和换行符）
new_json_str = json.dumps(json_data, ensure_ascii=False, indent=2)

# 将新的 JSON 字符串写入文件
with open('/Users/a123/project328/gm-tool/new.json', 'w') as file:
    file.write(new_json_str)
~~~

4. 删除最后两个字符

~~~python
str = "test\r\n"
str = str[:-2]
print(str) # "test" ,\r\n 分别算一个字符
~~~

5.  str 和 int类型的转换, and 和 or的使用， 多条件判断

~~~python
str1=1
str2=str(str1) # "1"

if (int(str2) > 10 and  int(str) > 12):
    print("666")

if (int(str2) > 10 or  int(str) > 12):
    print("666")

if (int(str2) > 10 and  int(str) > 12) or (int(str2) > 10 and  int(str) > 12):
    print("666")
~~~

6. 递归处理json串

~~~python
import json
#json文件发送形式
SendRegisterVerificationCodejson_txt = """
{
  "header":{
    "funcNo": "IF010002",
  },
  "payload": {
    "mobileTel": "13817120001"
  }
}
"""
date_json = json.loads(SendRegisterVerificationCodejson_txt)
print(date_json)
print("*"*10)

dic ={}
def json_txt(dic_json):
    if isinstance(dic_json,dict): #判断是否是字典类型isinstance 返回True false
        for key in dic_json:
            if isinstance(dic_json[key],dict):#如果dic_json[key]依旧是字典类型
                print("****key--：%s value--: %s"%(key,dic_json[key]))
                json_txt(dic_json[key])
                dic[key] = dic_json[key]
            else:
                print("****key--：%s value--: %s"%(key,dic_json[key]))
                dic[key] = dic_json[key]

json_txt(date_json)
print("dic ---: "+str(dic))
~~~

7.从标准输入接收输入

~~~python
for line in sys.stdin:
    if not line:
       break
    line = line.strip()
    try:
        ...
    except:
        pass

~~~

8.遍历json串的所有key

~~~python
js_new = json.loads(line_str)
     js_new["all_contexts"] = line_strpr
     dic={}		#用dic来形成新的json串
     for key in js_new:
         strtmp=js_new[key]
         dic[key]=str(strtmp)
      outStr=json.dumps(dic, ensure_ascii=False)
      print(outStr)

~~~

9.find查看是否包含

~~~python
s = "This be a string"
if s.find("is") == -1: # 没找到
    print "No 'is' here!"
else:
    print "Found 'is' in the string."
~~~

10.ascii与utf-8编码问题

~~~python
在python脚本第一行加上   # -*- coding: utf-8 -*-
#号键也要加上

直接js = json.loads(line)  #此时这个line已经被转为utf-8了，
str = json.dumps(js, ensure_ascii=False)	#这样就是utf-8字符串了

~~~

11.json与字符串细节

~~~python
如果
field1 = js['name']  #如果你的name对应的value是一个json对象，那么可以直接把field1当成json对象使用， 否则必须把field1转化为json对象才可以进一步解析field1中的子字段
~~~

12. 如果需要把一个已经含有转义字符的字符串搞成正常json串

~~~
import redis
import ast

# 连接到特定的Redis容器
redis_client = redis.Redis(host='localhost', port=6379, password='', db=0)

# 读取文件内容
with open('opDamage.txt', 'r') as file:
    new_str = file.read().strip()

# 将字符串转换为字典, 核心就是这步
new_dict = ast.literal_eval(new_str)

# 获取所有以AIMirror_KnockoutPool为前缀的key
#keys = redis_client.scan_iter("AIMirror_KnockoutPool*")
keys = redis_client.scan_iter("AIMirror_KnockoutPool_1_8_117_All|Devil|Insect|Mutant|Neutral|Wild_Devil|Wild")
# 遍历每个key并进行操作
for key in keys:
    # 获取list的长度和元素
    list_len = redis_client.llen(key)
    for index in range(list_len):
        # 将元素替换为新字符串
        redis_client.lset(key, index, new_dict)
~~~

