小知识



10.异常捕获

``` python
try 

except Exception as e:
    print(e)
```



9.字符串操作

~~~python
1.len() 返回长度
2.支持加， 与整数N相乘即原字符串重复N次
3.支持分片, S[m:n], 从m开始到n(不包含n), -1表示最后一个字符, S[:]为所有, S[m:n:k]中的k表示k步取一个元素(k为第一个元素), 如果k为负数则从右往左数,S[::-1]为反转字符串
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
~~~



7.字典(只能判断相等，无法比较大小)

~~~python
1. 创建 
d = dict(a=3, b=4) 生成字典 {'a':3, 'b':4}, 也可以配合zip比如dict(zip(['a','b'],[3,4]))实现同等效果， 也可以用.fromkeys()方法，比如dict.fromkeys(['a','b'],3)生成字典{'a':3,'b':3}，如果不要3就是{'a':None,'b':None}

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
d1.update(d2) # 合并两个字典
d.pop(key) # 删除key并返回该元素的值
del d[key] # 删除key但是不返回该元素的值
d[key]=value # 存在则修改，不存在就增加
8. 迭代
for key in d:
    print(d[key]) # print(key,'==>',d[key])
~~~

6.set (唯一、不可变无序、可迭代、不支持索引、不要求各个元素都是相同类型)

~~~python
1. 创建 (x=set(iter)) # iter为任何序列或可迭代对象
x=set([1,2])
y=set([3,4])
{1,2,3,4}等价于set([1,2,3,4]), 空set必须用set([])或set({}), {}为空字典, []为空list，单独用他们都不表示空set
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

~~~
1.python3中，除法(不管是整数出发还是float除法)都会保留小数部分，python2.7整数除法不会保留小数部分。
2. 两个除号 '//' 表示floor向下取整
~~~

4.数字类型转换

~~~python
1.整数转为字符串：hex(intx), oct(), bin(),str(), 将intx转换为16/8/2/10进制表示的字符串，也可以格式化比如'%x' % 64结果为'40'
2.字符串转为整数：int(strx, base) 将字符串strx根据base进制转换为整数，默认为10， 也可以用eval比如eval('0x40')结果为64
3.float("10.1")	 str转为转换浮点型
~~~

3.python可以在旧版本中开启新版本的特性

~~~
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
i#号键也要加上

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

