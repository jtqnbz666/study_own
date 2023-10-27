1.

两种格式化方式
print("%d,%d"%(1,2))

print("{},{}".format(1,2))

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
字符串转json
json_list = json.loads(string)

json转字符串
new_string = json.dumps(json_list,ensure_ascii=False,indent=2)
#ensure_ascii=False是为了防止中文乱码， indent=2打印好看

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

