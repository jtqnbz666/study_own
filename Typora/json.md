### 小知识

~~~
json不能有注释，yaml可以有
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

