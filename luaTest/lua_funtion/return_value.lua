--函数调用的一种特殊情况，若一个函数只有一个参数，并且此参数是一个字符串或table构造式，那么圆括号可以省略


-- 以下打印方式等价

print("hello World")
print "hello World"
print [[hello world]]

function f(test)
    for k, v in pairs(test) do
        print(k, v)
    end
end

-- 可以这样调用函数
f{x =10, y = 15}

--细节
--[[
调用函数时提供的实参数量可以与形参数量不同
lua会自动调整实参的数量，以匹配参数表的要求，
若”实参多余形参“则舍弃多余的实参，若实参不足，则多余的形参初始化为nil
    --]]

-- 多重 返回值的坑
--lua允许多返回值，不够则用nil表示，多了则舍弃
print("多重返回值")
function test() return "a","b" end
x = 10
y = 20
z = 30
x,y,z = test()
print(x,y,z) -- 注意这里变成了   a b  nil 而不是 a b 30

--如果一个函数调用不是一系列表达式的最后一个元素，那么将只产生一个值
x, y = test(),20        --这里test()只返回一个值 
x, y = test(), 20, 30   --30这个值被丢弃
print(x, y)     --是a 20  而不是 a  b


--用table构造式可以完整的接受所有结果
function test1()  end
function test2() return "a" end
local table = {test1()}     --空表  t = {}
local table1 = {test2()}    --t = {"a"}
local table2 = {test()}     --t = {"a", "b"}

--如果函数调用不是最后一个元素，同样只返回一个值
local t = {test1(),test(),6}    --t[1] = nil, t[2] = "a", t[3] = 3  因为这里test()只返回一个值
for k, v in pairs(t) do
    print(k,v)              -- 2   a,  3    6
end


--  变长参数
print("变长参数")

-- 打印所有参数
function printtest(...)
    for k, v in ipairs{...} do
        print(k, v)
    end

end

printtest(1,2,3,4)



print("闭包")
--内嵌函数
function newCounter()
    local i = 0
    return function() -- 匿名函数
        i = i + 1
        return i
    end
end

c1 = newCounter()
print(c1())
print(c1())

function newCounter(i)
    return function()
        i = i + 1
        return i
    end
end

c1 = newCounter(10)
print(c1())
print(c1())

-- 非全局的函数
print("非全局的函数")
--函数不仅可以存放在全局变量中，还可以把函数存放在table字段中或者局部变量中
lib = {}
lib.foo = function(x, y) return x + y end
lib.goo = function(x, y) return x - y end

--存放在局部变量中
local f = function() print("test") end
local function f()  print("test2") end
