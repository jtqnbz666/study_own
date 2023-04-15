-- 所有全局变量都在_G表中
    
--[[for k, v in pairs(_G) do
    print(k, v)
end

setmetatable(_G, {
        __newindex = function(_, k)
            error("attempt to wirte to undeclared vairable"..k)
        end,
        __index = function(_, k) 
            error("attemp to read undeclare variable"..k)
        end
        })

    --print(a)  --调用__index  ，因为这里的a是全局变量,但由于_G中没有a元素，所以触发_index方法
    -- a = 10      --调用__newindex

--]]

-- 非全局的变量，
--[[a = 1-- 这里创建了一个全局变量
-- 将当前环境变量改成一个新的空表
setfenv(1, {})
print(a)  -- 报错
]]--


--[[
a = 1
setfenv(1, {_G = _G})
_G.print(a) -- 输出nil， 为这个a在_G里面
_G.print(_G.a)
]]--

--使用元表__index
a = 1
local  newEnv = {}
setmetatable(newEnv, {__index = _G})

setfenv(1, newEnv)
print(a) -- 在当前环境找不到，但去_index方法可以找到
