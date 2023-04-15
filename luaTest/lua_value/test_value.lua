-- 例子1
x = 1
y = x
y = 2
print(x)
-- 例子2 
function chang(x)
    x = 10
end
y = 1
chang(y)
print(y)

-- 例子3
x = "test"
y = x
x = "show"
print("x")

-- 例子4
x = {abc = "123", 456}
y = x
y = {abc = "666"}
print(x.abc)

-- 例子5
function show()
    print("show some thing ")
end

function move()
    print("move to")
end

x = show
y = x
x = move
y()         --show some thing
x()         -- move to


-- 例子6
x = {123,"hhha"}
y = x[1]
x[1] = 666
print(y)    --123


--事实证明， 只有例子4才会传引用，其它都是传值
