--[[require "Luaclib"   --linux����ʱ��Ҫע�͵�

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
ʵ����lua��������
local path = "luaclib.dll"    
local f = package.loadlib(path,"luaopen_luaclib")   -- ����luaopen_luaclib����  
f()  
--]]
--]]

local mylib = require("Luaclib")  --��Ӧ��teste.c�еİ���      


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

