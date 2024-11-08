### 小知识

~~~shell
9.json.dumps(dict, indent=4, ensure_ascii=False)，第二个参数表示缩进，第三个参数确保输出中包含非 ASCII 字符，比如中文，如果不要第三个参数，那么dumps出来的中文就会被编码为unicode形式，loads回来的时候能正常显示中文
8.引号内的特殊字符(比如""和\)都应该有转义字符，只要去掉一层引号，那么引号内的特殊字符都得去掉一层转义，同理只要加上一层引号，引号内的所有特殊字符都要加一层转义
7.python中的list和dict对象可以转为json对象，流程是先dumps转为json字符串，再用loads加载为json对象
6.json的json.dumps可以传入json对象或者字典，如果是字典的话不能包含二进制数据
5.str和json是两种类型，json中可以包含str对象
4.任何语言将字符串转唯json对象都应该传入一个json字符串才行，比如python的json.loads只能传入json字符串，尽管字典看着就是一个json对象但其实它不是，也不能作为json.loads的参数传递, 所以尽管一个正规的json字符串是不能一直调用json.loads的，一旦成功变为json对象就不能继续调用了，比如这个json串包含了两层转义，那就会在第二次调用时变为json对象，但json.dumps可以一直调用，每次都会多增加一层转义。
3.python中写str = "{\"test\":1}", 打印出来是{"test":1}，因为打印出来的会去掉最外层的引号，比如引号内的转义字符会被去掉一层,看着像json，但其实只是符合json条件的json字符串，在python中如果不调用json.loads(json字符串)之前，在python程序中它都只是字符串的含义
2.json中如果是字符串就要用双引号包含起来
1.json不能有注释，yaml可以有
~~~

case5

~~~python
json_str = '{"name": "Alice", "age": 30}'
data = json.loads(json_str)
data = json.dumps(data) 
teststr = json.dumps(json_str)
filehandle = open('./test.txt', 'w')
filehandle.write(data) # 文件中：{"name": "Alice", "age": 30}
filehandle.write(json_str) # 文件中：{"name": "Alice", "age": 30}
filehandle.write(teststr) # 文件中："{\"name\": \"Alice\", \"age\": 30}"

# 此时如果文件中的第一、二行被加载到程序中时会变成'{"name": "Alice", "age": 30}'， 第三行会变成"{\\\"name\\\": \\\"Alice\\\", \\\"age\\\": 30}", 单双引号的区别看python的介绍，本质上没有任何功能上的区别，只是为了显示好看
~~~

case4

~~~python
比如json文件中放的是{"name": "Alice", "age": 30}, 比如python程序加载进去的时候会加载为'{"name": "Alice", "age": 30}'， 这就跟case3中定义的json_str是一样的
~~~

case3

~~~python
json_str = '{"name": "Alice", "age": 30}'
data = json.loads(json_str)
print(data)  # {'name': 'Alice', 'age': 30}
print(json_str) # {"name": "Alice", "age": 30}
~~~

case2

~~~shell
str = "[\"{ \"heroId\": 110, \"level\": 1}\"]"
直接print出来是 ["{"heroid": 110, "level": 1}"], 可以看到双引号的里面都没有转移字符，所以这个str只支持打印出来，是不能用json.loads的，因为[]内的内容已经不符合json的规范了。
# 此时[]内的内容甚至都不是一个字符串，因为引号内的引号如果没有转义字符，两个那么第二个引号出现的时候就是第一个引号的结束符号，比如此时[]中就有三对引号
~~~

case1

~~~python
# 正常的json串
jsonstr = "[\"{ \\\"heroId\\\": 110, \\\"level\\\": 1}\"]"
jsonstr = json.loads(jsonstr)
jsonstr = jsonstr[0]
print(jsonstr) # {'heroId': 110, 'level': 1} 但它只是str对象而不是json对象
jsonstr = json.loads(jsonstr)
heroid = jsonstr.get('heroId') # 必须先加载为json对象才可以当字典用
print(jsonstr) # {'heroId': 110, 'level': 1}
~~~

json.loads和json.dumps的使用
如果一开始是json对象/字符串，使用loads方法，再dumps，得到的就是json对象/字符串
如果一开始是json对象，没有使用loads方法，直接dumps，得到的是一个json字符串
如果一开始是json字符串，没有使用loads方法，直接dumps，得到的是一个**双重转义**的json字符串

用例一：

~~~json
//divideLevel.json其实已经是json对象了(可以通过最最外层是否有引号来分辨)
原字符串：["{ \"heroId\": 110, \"level\": 1}"]
import json
with open('/Users/a123/project328/gm-tool/divideLevel.json', 'r') as file:
    json_data = file.read().replace('\n','')
//如果下边的loads方法注释，dumps(json_data),得到的就是有两重转移的字符串(不是我需要的，比如)
"[\"{ \\\"heroId\\\": 110, \\\"level\\\": 1}\"]"
#data = json.loads(json_data, indent=2)
//如果不注释上边的loads方法,dumps(data)得到的就是
[
  "{ \"heroId\": 110, \"level\": 1}"	//因为indent=2，这里可以看出来有两个缩进
]

new_json = json.dumps(json_data, indent=0)
with open('/Users/a123/project328/gm-tool/test.json', 'w') as file:
    file.write(new_json)
~~~

用例二：

前置说明

~~~json
//dealDivideLevel.json本身已经是无转义字符的json对象, 比如
{
"mirrors": [
{
"heroId": 110,
"level": 1
}
]
}
//最终结果，如果打印的是json_data，得到一个json字符串
"{\"mirrors\": [{\"heroId\": 110,\"level\": 1}]}"
//如果打印的是data，得到的就是json对象。因为indent=2可以看出缩进
{
  "mirrors": [
    {
      "heroId": 110,
      "level": 1
    }
  ]
}
~~~



~~~json

# 从文件中读取原始的 JSON 数据
with open('/Users/a123/project328/gm-tool/dealDivideLevel.json', 'r') as file:
    json_data = file.read().replace('\n','') //可以看出上边每行都有\n，处理一下

# 将 JSON 字符串解析为 Python 对象
data = json.loads(json_data)

# 将 Python 对象转换为 JSON 字符串（包含缩进和换行符）
new_json_str = json.dumps(data, ensure_ascii=False, indent=2) //如果这里打印的是json_data，那得到的就是一个json字符串, 而如果打印的是data，得到的就是json对象。

# 将新的 JSON 字符串写入文件
with open('/Users/a123/project328/gm-tool/new.json', 'w') as file:
    file.write(new_json_str)
~~~

拓展：待验证，有问题

~~~json
1.
"[\"{ \\\"heroId\\\": 110, \\\"level\\\": 1}\"]"
转为下边这种格式
["{ \"heroId\": 110, \"level\": 1}"]
import json
str_object = '[\"{ \\\"heroId\\\": 110, \\\"level\\\": 1}\"]'
json_object = json.loads(str_object.replace('\\', ''))
list_object = [json_object[0]]
print(list_object)

2.
"[\"{ \\\"heroId\\\": 110, \\\"level\\\": 1}\"]"
转为下边这种格式
['{ "heroId": 110, "level": 1}']
import ast
str_object = '[\"{ \\\"heroId\\\": 110, \\\"level\\\": 1}\"]'
list_object = ast.literal_eval(str_object)
print(list_object)
~~~

