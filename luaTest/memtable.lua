#!/usr/local/bin/lua 
-- pairs 和 ipairs的区别
-- ipairs遍历table时，键值从1递增，当键值中断(如[1]过了就是[3])或遇到nil时退出
-- pairs 会遍历表中所有键值对，且元素是根据哈希算法来排序的，所以打印出来的顺序是乱的
-- 只有table和userdata可以独立的设置原表
 
--[[ 1.由一个 哈希表 与一个 数组 构成，当插入一个元素时，
 如果元素是键值对，则将其放入哈希表中，如果是值则放入数组中。
 2.hash表上会有冲突，通过一个链表的形式组织冲突的元素。
--]]

t = {}
mt = {}
setmetatable(t,mt);
getmetatable(t);

t = setmetatable({[2] = 3}, {
    __index = function(t,key)
        if key == "foo" then
            return 0
        else
            return table[key]
        end
    end
});

other = {foo = 3}
-- t = setmetatable({}, {__index = other})
print(t.foo)  -- 0
print(t.bar)  -- nil
print(t[2])   --3


    -- newindex 
-- 当对一个table中不存在的索引赋值时，解释器就会查找__newindex元方法
other = {}
-- setmetatable 会将第二个表设置为第一个参数的表的原表
-- 并且会将第一个参数的表返回
t = setmetatable({}, {__newindex = other})
t.foo = 3

t = setmetatable({}, {
    __newindex = function(t, key, value)
        if type(value) == "number" then
        rawset(t, key, value * value)
    else
        rawset(t, key, value)
    end
end
})

t.foo = "foo"
t.bar = 4
t.la = 10
print(t.foo)
print(t.bar)
print(t.la)



--运算符  乘法
t = setmetatable({1,2,3}, {
    __mul = function(t, other)
    new = {}
    
    for i = 1, other do
        for _, v in ipairs(t) do 
        table.insert(new, v) 
        end
    end

    return new
end
})

t = t * 2 -- 将上边的1，2，3  变成了1，2，3，1，2，3
for v,k in pairs(t) do
    print(v,k) --这种方式会把key和value一起打印出来   
end
    

--    __call 的原方法
-- 当table名字作为函数名字的时候会被调用
-- 下边可以观察到，本来t是一个table，却当作函数来使用

t = setmetatable({}, {
    __call = function(t, a, b, c, whatever)
    return (a +b +c) * whatever
end
})
print(t(1,2,3,4))



-- 元方法 __tostring
-- 它可以定义一个table转换成字符串，常和print配合使用
-- 如果不使用__tostring元方法，默认的print(表名) 会打印处一串地址
t = setmetatable({1, 2 ,3}, {
    __tostring = function(t)
        sum = 0
        for _, v in pairs(t) do 
        sum = sum + v 
        end
        return "Sum:"..sum
end
})

print(t)

-- 结合以上例子，编写一个2D矢量类:
Vector = {}
Vector.__index = Vector
-- 这步必须要，因为Vector是一个元表，当这个元表对应的普通表找不到对应的函数的时候
-- 就会去这个元表的__index中查找，这样设置后，__index中就包含元表的所有函数，即整个元表
-- 需要理解一点 __index只是一个元方法，当出现在普通表中找不到函数的情况
-- 就会去找这个普通表对应的元表的__index方法，来进一步寻找，就和__tostring, __call 一样
-- 元方法只是普通表遇到特定情况时的解决方案，所以需要设置 元表.__index = 元表


function Vector.__add(a, b)
    if type(a) == "number" then
        return Vector.new(b.x + a, b.y + a)
    elseif type(b) == "number" then
        return Vector.new(a.x + b, a.y + b)
    else 
        return Vector.new(a.x + b.x, a.y + b.y)
    end
end

function Vector.__sub(a,b)
    if type(a) == "number" then
        return Vector.new(b.x - a, b.y - a)
    elseif type(b) == "number" then
        return Vector.new(a.x - b, a.y - b)
    else 
        return Vector.new(a.x - b.x, a.y - b.y)
    end
end

function Vector.__mul(a,b)
    if type(a) == "number" then
        return Vector.new(b.x * a, b.y * a)
    elseif type(b) == "number" then
        return Vector.new(a.x * b, a.y * b)
    else 
        return Vector.new(a.x * b.x, a.y * b.y)
    end
end

function Vector.__div(a,b)
    if type(a) == "number" then
        return Vector.new(b.x / a, b.y / a)
    elseif type(b) == "number" then
        return Vector.new(a.x / b, a.y / b)
    else 
        return Vector.new(a.x / b.x, a.y /  b.y)
    end
end

function Vector.__eq(a,b)
    return a.x == b.x and a.y == b.y
end

function Vector.__lt(a, b)
    return a.x < b.x or (a.x == b.x and a.y < b.y)
end

function Vector.__le(a, b)
    return a.x <= b.x and a.y <= b.y
end

function Vector.__tostring(a)
    return "(" .. a.x .. "," .. a.y .. ")"
end

function Vector.new(x,y)
    return setmetatable({x = x or 0, y = y or 0}, Vector)
end

function Vector.distance(a, b)
    return (b - a):len()
end

function Vector:clone()
    return Vector.new(self.x, self.y)
end

function Vector:unpack()
    return self.x, self.y
end

function Vector:len()
    return math.sqrt(self.x * self.x + self.y * self.y)
end

function Vector:lenSq()
    return self.x * self.x + self.y * self.y
end

function Vector:normalize()
    local len = self:len()
    self.x = self.x / len
    self.y = self.y / len
    return self
end

function Vector:normalized()
    return self / self:len()
end

function Vector:rotate(phi)
    local c = math.cos(phi)
    local s = math.sin(phi)
    self.x = c * self.x - s * self.y
    self.y = s * self.x + c * self.y
    return self
end

function Vector:perpendicular()
    return Vector.new(-self.y, self.x)
end

function Vector:projectOn(other)
    return (self *other) * other / other:lenSq()
end

function Vector:cross(other)
    return self.x * other.y - self.y * other.x
end

setmetatable(Vector,{__call = function(_, ...)
        return Vector.new(...) 
        end
        })

