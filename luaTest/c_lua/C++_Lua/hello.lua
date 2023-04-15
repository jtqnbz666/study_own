--[[require "Luaclib"   --linux运行时需要注释掉

print("test")
local avg,sum =ss.average(1,2,3,4,5)
print(avg,sum)  -- 3 15  
ss.sayHello()   -- hello world!
str = "i am so cool"
tbl = {name = "shun", id = 2020104}
function add(a, b)
    return a + b
end


print("nihao")


--[[require "luaclib"
实际上lua会这样做
local path = "luaclib.dll"    
local f = package.loadlib(path,"luaopen_luaclib")   -- 返回luaopen_luaclib函数  
f()  
--]]
--]]

local mylib = require("Luaclib")  --对应于teste.c中的包名      


print("test")
local avg,sum =mylib.average(1,2,3,4,5)
print(avg,sum)  -- 3 15  
mylib.sayHello()   -- hello world!
str = "i am so cool"
tbl = {name = "shun", id = 2020104}
function add(a, b)
    return a + b
end


print("nihao")

