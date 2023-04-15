-- test1，和test2都有函数前边的限定词， 如M.add 这里讲诉如何去掉限定词的修饰

-- 去掉限定词的修饰
--[[local moduleName = ...      --当这个lua模块被调用的时候，... 代表的就是当前模块的名称，比如现在是hignmethod

for i = 1, select("#", ...) do
    print(select(i, ...))
end

local M = {} --局部变量
_G[moduleName]= M   --将或者局部变量赋值给模块名

package.loaded[moduleName] = M
setfenv(1, M)
]]--
--module(...)         --上边的所有代码可以通过这一行来实现
module(..., package.seeall) --相当于多加了一个setmetatable(M,{__index = _G})
function new(r, i) return {r = r, i = i} end
-- 这之后
function add(c1, c2)
    return new(c1.r + c2.r, c1.i + c2.i)
end
--等价于
--[[function M.add(c1, c2)
    return new(c1.r + c2.r, c1.i + c2.i)
end]]--


--[[
-- 方法一
setmetatable(M,{__index = _G})
setfenv(1, V)

--方法二
local _G = _G
setfenv(1, M)
--当我们需要访问前一个变量时，_使用_G.方法  去访问
--方法三 ，也是最正规的

--将需要用到的函数或者模块声明为局部变量
local sqrt = math.sqrt 
local io = io  --io库
--再改变环境变量
setfenv(1, M)
    ]]--


    --module函数
       --[[ 在定义一个模块时，前面的几句代码都是一样的，就分为以下几步：
        从require传入的参数中获取模块名；
        建立一个空table；
        在全局环境_G中添加模块名对应的字段，将空table赋值给这个字段；
        在已经加载table中设置该模块；
        设置环境变量。
           ]]--
-- 可以使用module(...) 来代替这些过程

--  module(..., package.seeall)
    --相当于加上了setmetatable(M, {__index = G})
