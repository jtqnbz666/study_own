-- 这个用来测试 lua-socket.lua 的代码
local skynet = require "skynet"

local CMD = {}

local driver = require "skynet.socketdriver"
local buffer = driver.buffer()
local pool = {}

function CMD.step1()
    local data, size
    for i=1, 16 do
        data, size = driver.str2p("mark"..i)
        driver.push(buffer, pool, data, size)
    end
end

function CMD.step2()
    local data, size
    data, size = driver.str2p("mark17")
    driver.push(buffer, pool, data, size)
end

skynet.start(function ()
    skynet.dispatch("lua", function (_, _, cmd, ...)
        local f = assert(CMD[cmd])
        f(...)
    end)
end)