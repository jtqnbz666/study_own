#!/usr/local/bin/lua
function factorial1(n)
	if n == 0 then
		return 1
	else 
		return n * factorial1(n - 1)
	end
end
print(factorial1(5))
factorial2 = factorial1
print(factorial2(5))



function testFun(tab, fun)
	for k, v in pairs(tab) do
		print(fun(k,v));	 --内部调用匿名函数
	end
end

tab =  {key1 = "val1", key2 = "val2"}

-- 调用testFun函数， 向此函数的参数1 传入表tab， 参数二传入匿名函数

testFun(tab, 
		function(key, val)
			return key.."="..val
		end
);
