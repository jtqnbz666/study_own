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

