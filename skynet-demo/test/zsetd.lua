local skynet = require "skynet"

local zset = require "zset"

local CMD = {}

function CMD.sync(lud, start)
    local rank = zset.new(lud)
    math.randomseed(math.tointeger(skynet.time()*100), skynet.self())
    local i = start
    local function write_zset()
        i = i + 1
        local score = math.random(1,10000)
        local member = "mark-"..i
        rank:zadd(score, member)
        skynet.error("score", score, "member", member)
        skynet.timeout(100, write_zset)
    end
    skynet.timeout(100, write_zset)
    local function ready_exit()
        rank:dump()
        skynet.exit()
    end
    skynet.timeout(2500, ready_exit)
end

skynet.start(function ()
    skynet.dispatch("lua", function (_, _, cmd, ...)
        local f = assert(CMD[cmd])
        f(...)
    end)
end)
