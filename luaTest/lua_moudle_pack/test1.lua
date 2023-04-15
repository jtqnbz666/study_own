local moduleName = ...
-- print(package.path)   --打印lua模块的加载路径有哪些
-- print(package.cpath)  --打印c/c++模块(.so文件)的加载路径有哪些
--打印参数...
for i = 1, select("#", ...) do
    print(select(i, ...))
end

local M = {} -- 局部的变量
_G[moduleName] = M -- 将这个局部变量最终复制给模块名
complex = M

function M.new(r, i) return {r = r, i = i} end

-- 定义一个变量i
M.i = M.new(0, 1)
    function M.add(c1, c2)
    return M.new(c1.r + c2.r, c1.i + c2.i)
end

function M.sub(c1, c2)
    return M.new(c1.r - c2.r, c1.i - c2.i)
end

package.loaded[moduleName] = M --可以代替return，如果没有return就会把这里面的指返回出去
-- return complex
