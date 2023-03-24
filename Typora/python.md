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

line.split("\t", 1)  以"\t"进行分割， 仅分割一次



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

