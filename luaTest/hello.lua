#! /usr/local/bin/lua
print("hello World!")
print("Jiangtao 大帅哥!")

array = {}
for i = -2, 2 do
	array[i] = i *2
end

for i = -2, 2 do
	print(array[i])
end

arrayA = {"lua", "jiangtao", [3]  = "hhha", a = {[5] = "test"}}
--这里注意， 字符只能是a, 不能写成 "a",  python刚好是反过来的
-- 如果在arrayA 中把[3]改成[4]就不会打印"hhha"
-- 如果把[3]换成一个字符串,同样不会打印"hha"
for i = 0, 3 do
	print(arrayA[i])
end

print(arrayA.a[5])
print(arrayA["a"][5])         --这里注意一下，索引a必须要引号
-- 二维数组
array = {{1,2,3}, {4,5,6}, {7,8,9}}
for i = 1, 3 do
	for j = 1, 3 do
		print(array[i][j])
	end
end

-- table（表）
mytable = {a = "xxx", b = "yyyy", 1, 2, 3, 4}
smartMan = {
	name = "none",
	sayHello = function()
		print("大家好")
	end
}

for i = 0, 2 do
	print(mytable[i])
end
smartMan.sayHello();

print(mytable.a);

mytable = {}
print("mytable的类型是", type(mytable))

mytable[1] = "Lua"
mytable["wow"] = "修改前" -- 必须加引号
print("mytable 索引为1的元素是", mytable[1])

--必须要加引号”“ 不然为nil
print("mytable 索引为wow的元素是", mytable["wow"])
print(mytable.wow);


--如果将a赋值给b，则a和b都指向同一块内存
tab = {}
tab["a"] = nil
b = tab["a"]
print(b)


-- table 连接 concat
fruits = {"apple", "balana", "pear"}
print("连接后的字符串"..table.concat(fruits))
-- 指定连接字符
print("连接后的字符串",  table.concat(fruits,","))
--指定索引来连接字符串
print("连接后的字符串"..table.concat(fruits, ",", 2, 3))


-- table 插入和溢出操作
-- table.insert 和 table.remove
fruits = {"banana", "orange", "apple"}
-- 在末尾插
table.insert(fruits, "mango")
print("索引为4的元素为"..fruits[4])

--在索引为2的键处插入
table.insert(fruits, 2, "grapes")
print("索引为2的元素为"..fruits[2])

print("最后一个元素为"..fruits[5])

table.remove(fruits, 5)
--这里不能使用..连接，会报错
print("最后一个元素为",fruits[5])
if fruits[5] == nil then -- 这种情况下nil不需要加""
 print("hha")
end

print(type(fruits[5]))  --nil
print(type(fruits[5] == nil)) -- boolean
print(type(type(fruits[5]))) --string
print(type(nil))  -- nil
print(type(nil) == "nil") -- true
print(type(type(nil)))  --string
-- 以下情况nil要加""
print(type(X) == nil) -- false;
print(type(X) == "nil") --true;


-- table 排序
-- table.sort
fruits = {"banana", "orange", "apple", "grapes"}
print("排序前")
for k,v in ipairs(fruits) do
	print(k, v)
end

table.sort(fruits)
print("排序后")
for k,v in ipairs(fruits) do
	print(k, v)
end

-- 对table.sort 的第二个参数进行设置， 决定了排序方式
-- 对三名工会成员排序
guild = {}
table.insert(guild, {
        name = "Cladhaire",
        class = "Rogue",
        level = 70,
        })

table.insert(guild, {
        name = "Sagart",
        class = "Priest",
        level = 70,
        })

table.insert(guild, {
        name = "Mallaithe",
        class = "Warlock",
        level = 40,
        })

-- 具体实现函数
function sortLevelNameAsc(a, b)
    if a.level == b.level then
        return a.name < b.name
    else
        return a.level < b.level
    end
end

-- 测试功能
table.sort(guild, sortLevelNameAsc)

for k,v in ipairs(guild) do
    print(k, v.name)
end

-- table.maxn
function table_maxn(t)
	local mn = nil;
	for k, v in pairs(t) do
		if(mn == nil) then
			mn = v
		end
		if mn < v then
			mn = v
		end
	end
	return mn
end
tbl = {[1] = 2, [2] = 6, [3] =34, [26] = 5}
print("tbl最大值：", table_maxn(tbl))
print("tbl长度" , #tbl)


--用nil获取长度是有缺陷的，比如遇到nil 就会中断
-- 代替方法，自己写函数

function table_length(t)
	local length = 0
	for k, v in pairs(t) do
		 length = length + 1
	end
	return length
end
array = {1, nil, 2, 3, nil, 4, 5}
print(table_length(array))


-- pairs会遍历表中所有数据，但不一定有序
-- ipairs会把表当哈希表，从键值为[1]开始，当键值中断或value为nil就会中断

