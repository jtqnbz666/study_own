#!/usr/local/bin/lua
--[[
lua中的table是由一个哈希表和一个数组构成的
当插入一个元素时，如果是键值对，则放入哈希表中，如果是值则放入数组中
想下边在这种情况
哈希表:{[1] = 1, [3] = 3, [5] =5, [6] = 6};
数组:{2 ,4}
将数中的元素转移到哈希表
数组中的元素变为[1] = 2, [2] = 4
所以合并下来
此时的哈希元素为: {[1] = 2, [2] = 4, [3] = 3, [5] = 5, [6] = 6}
]]--
local t = {
    [1] = 1,
    2,
    [3] = 3,
    4,
    [5] = 5,
    [6] = 6
}
print("pairs ---->")
    for k, v in pairs(t) do
print(k,v)              --2,4,3,5,6  不一定是这个顺序
    end

    print("ipairs ---->")
    for k, v in ipairs(t) do    -- 2,4,3  一定是，因为键值从[4]就没有了
print(k,v)
    end


print("test_ipairs ---->")
local test = {
    [1] = 1,
     "HHA",
     [3] = nil,     --虽然有key[3] 但value为nil， 所以ipairs不会继续打印
     "laotie",
     [5] = 5,
    [6] = 6
}
for k, v in ipairs(test) do
    print(k, v)     --1 "HHA"  2 "laotie"
end


-- 还有别的遍历方式
--[[
for i = 1, #(test) do

end

for i = 1, test.maxn(test) do       --而且现在好像舍弃这个函数了

end
]]--
--这两种都是不好的，因为"#" 必须从表中键值为[1]的地方开始计算，而且键值不能中断，更不能直接没有[1]
-- maxn只是获取表中的”最大整数值“如果有字符串会有影响，并且可能造成资源浪费，多次循环
