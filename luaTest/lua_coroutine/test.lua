co = coroutine.create(function(a,b)
    print(coroutine.status(co), "start") -- 执行代码 coroutine状态为running
    print("co", a, b)
    print(coroutine.status(co), "end")  --执行代码， coroutine状态为running 
end)

print(coroutine.status(co)) --刚创建的coroutine状态为suspend
coroutine.resume(co, 1, 2)  --启动coroutine,将跳转到coroutine的function执行
print(coroutine.status(co)) --coroutine执行完毕，状态为dead

function foo(a)
    print("foo", a) -- foo 2
    return coroutine.yield(2 * a) -- return :a, b
end

co = coroutine.create(function(a, b)
        print("co-body", a, b) -- co-body 1 10
        local r = foo(a + 1)
        
        print("co-body2", r)
        local r, s = coroutine.yield(a + b, a - b)
        
        print("co-body3", r, s)
        return b, "end"
end)

print("main", coroutine.resume(co, 1, 10)) --true, 4
print("- --- -")
print("main", coroutine.resume(co, "r")) --true, 11 -9
print("- --- --")
print("main", coroutine.resume(co, "x", "y")) -- true 10 end
print("- --- ---")
print("main", coroutine.resume(co, "x", "y")) -- false cannot resume dead coroutine
