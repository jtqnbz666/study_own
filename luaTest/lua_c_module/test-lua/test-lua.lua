local tab1 = {
    ["mark"] = 100,
    ["king"] = 90,
}


local tab2 = {"mark", "king", 1, (function() end)}

for k, v in pairs(tab1) do
    print(k, v)
end

for _, v in ipairs(tab2) do
    print(v)
end

local tab3 = setmetatable ({
    ["mark"] = 100,
    ["king"] = 90,
}, {
    __index = function (t, k)
        return "nil1000"
    end,
    __newindex = function (t, k, v)
        rawset(t, k, v)
    end,
})

tab3["vico"] = 1000

print("set vico = 1000+++++", tab3["vico"])

local function iteractor(NUM, START)
    local i = START or 0
    local max = NUM or 16
    return function ()
        i = i % max + 1
        return i
    end
end

local iter = iteractor()

for i = 1, 20 do
    print(iter())
end