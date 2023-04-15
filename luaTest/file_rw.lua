#!/usr/local/bin/lua
-- 读文件
local file1 = io.input("1.txt") -- 当前目录"1.txt"必须存在,会返回这个文件的句柄
-- "*a"读入全部 "*n"按数字读入， "*l"按行读入，也是默认方式 ，"*n"表示读n个
local str = io.read("*a")
print(str)

-- 写文件
local file2 = io.output("2.txt") -- 当前目录"2.txt"不需要存在
io.write(str)
io.flush()
io.close()


-- 利用这几个函数封装一个文件复制的函数
function copy(fileA, fileB)
    local file1 = io.input(fileA)
    if not file1 then print("fileA","不存在")
    return end
    local str = io.read("*a")
    local file2 = io.output(fileB)
    io.write(str)
    io.flush()
    io.close()
end

for line in io.lines("1.txt") do
    print(line)
end

copy("1.txt", "2.txt")


-- 通过返回的文件句柄对当前文件进行操作， 上边的io.read()等函数可以指定任意文件
local f = io.open("3.txt", "a+")
f:write("happy New Year!")
f:flush()
f:seek("end", -1) -- 定位到文件末尾前一个字节
local str = f:read(1)
print(str)
f:close()
