local skynet = require "skynet"
local socket = require "skynet.socket"

local CMD = {}

-- 测试 lua-socket.c 中读缓冲区
function CMD.step1(...)
    local step = skynet.uniqueservice("step")
    skynet.send(step, "lua", "step1", ...)
end

-- 测试 lua-socket.c 中读缓冲区
function CMD.step2(...)
    local step = skynet.uniqueservice("step")
    skynet.send(step, "lua", "step2", ...)
end

-- 测试热更新
function CMD.share(...)
    skynet.error("controld   share")
    skynet.send(".main", "lua", "share", ...)
end

skynet.start(function()
	skynet.fork(function ()
        local function split_cmdline(cmdline)
            local split = {}
            for i in string.gmatch(cmdline, "%S+") do
                table.insert(split,i)
            end
            return split
        end
        local stdin = socket.stdin()
        while true do
            local cmdline = socket.readline(stdin, "\n")
            local split = split_cmdline(cmdline)
            local command = split[1]
            if CMD[command] then
                pcall(CMD[command], select(2, table.unpack(split)))
            else
                pcall(skynet.newservice, cmdline)
            end
        end
    end)
end)
