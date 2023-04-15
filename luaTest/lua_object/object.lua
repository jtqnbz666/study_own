local Account = {value = 100}
function Account:new(o) 
    --这里使用了:, 可以省略self参数，可以理解为self是c++中的this指针
    --原本形式Account.new(self, o);
    o = o or {}
    setmetatable(o, self);
    self.__index = self
    return o
end

function Account:display()
    self.value = self.value + 100 
    print(self.value)
end

local a = Account:new{} --相当于使用Account创建了一个对象

a:display() -- 100  --相当于a.display(a);
a:display() -- 200  
--a:display()相当于a.display(a)相当于

--上边可以看到value字段是Account中的
--所以a.value = getmetatable(a).__index(value);


--继承
local CA = {value = 100}  -- 父亲

function CA:new(o)
   o = o or {}
   setmetatable(o, self) 
   self.__index = self
   return o
end

function CA:display()
    print(self.value)  
end

function CA:addValue(v)
    self.value = self.value + v    
end

local CLittleA = CA:new()

local s = CLittleA:new{value1 = 10}

CLittleA.display = function()
    print("test")
end

CLittleA:display()
s:display()


-- 多重继承
-- 在多个父类中查找字段K
local function search(k, pParentList)
    for i = 1, #pParentList do
        local v = pParentList[i][k]
        if v then
            return v
        end
    end
end

function createClass(...)
    local c = {} -- 新类
    local parents = {...}
    
    --类在其元表中搜索方法
    setmetatable(c, {__index = function(t, k) return search(k, parents) end})
    
    -- 将c作为其实例的元表
    c.__index = c
    
    -- 为这个新类创建一个新的构造函数
    function c:new(o)
        o = o or {}
        setmetatable(o, self)
        -- self.__index = self 这里不用设置了，上边已经设置了
        return o
    end

    --返回新的类(原型)
    return c
end

--一个简单的类CA
local CA = {}
function CA:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function CA:setName(strName)
    self.name = strName
end

--一个简单的类CB
local CB = {}
function CB:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function CB:getName()
    return self.name
end

--创建一个c类，它的父类是CA和CB
local c = createClass(CA, CB)

-- 使用c类创建一个实例对象
local objectC = c:new{name = "jt"}

--设置objectC对象一个新的名字
objectC:setName("wh")
local newName = objectC:getName()
print(newName);