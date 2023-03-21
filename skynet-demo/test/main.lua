local skynet = require "skynet.manager"

skynet.start(function ()
    skynet.newservice("debug_console", 8000)
    if true then
        skynet.uniqueservice("controld")
        local sharetable = require "skynet.sharetable"
        local hotfixd = skynet.uniqueservice("hotfixd")
        skynet.fork(function ()
            while true do
                local names = skynet.call(hotfixd, "lua", "watch")
                sharetable.update(table.unpack(names))
            end
        end)
        local function dump(t)
            skynet.error("begin to dump table ...")
            for k, v in pairs(t) do
                skynet.error("k:",k,"v:",v)
            end
            skynet.error("dump table end")
        end
        local tab = sharetable.query("gamecfg/name.lua")
        dump(tab)
        skynet.dispatch("lua", function (_, source, cmd, ...)
            skynet.error("recv message from", source, cmd)
            if cmd == "share" then
                dump(tab)
            end
        end)
        skynet.register(".main")
    end
    if false then
        math.randomseed(math.tointeger(skynet.time()*100), skynet.self())
        local zset = require "zset"
        local rank = zset.new()
        do
            local zsetd = skynet.newservice("zsetd")
            skynet.send(zsetd, "lua", "sync", rank:getlud(), 10000)
        end
        do
            local zsetd = skynet.newservice("zsetd")
            skynet.send(zsetd, "lua", "sync", rank:getlud(), 20000)
        end
        local i = 0
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
        skynet.timeout(2000, ready_exit)
    end
    if false then
        local function timeout()
            print("hello world", skynet.time())
            skynet.timeout(100, timeout)
        end
        skynet.timeout(100, timeout)
    end
    if false then
        local master = skynet.newservice("master")
        print("now time", skynet.time())
        local idx, name = skynet.call(master, "lua", "get_idx", "mark")
        print(skynet.time(), "main recv get_idx:", idx, name)
    end
    if false then
        local socket = require "skynet.socket"
        local res, err = socket.open("127.0.0.1", 6379)
        if not res then
            print(err)
        else
            print("hello")
        end
    end
    if false then
        local zset = require "zset"
        local rank = zset.new();
        rank:zadd(50, "mark")
        rank:zadd(80, "darren")
        rank:zadd(100, "king")
        rank:dump()
        skynet.error("count:", rank:zcount())
        rank:zrem("king")
        rank:dump()
        skynet.error("king:", rank["king"])
        rank:zmadd({["vico"]=60, ["milo"]=70})
        rank:dump()
        rank:zmrem({"vico"})
        rank:dump()
        skynet.error("vico:", rank["vico"])

        rank:zremrange(0,-1)
        skynet.error(rank:zcount())
        skynet.error("milo", rank["milo"])
        rank:zmadd({["vico"]=60, ["milo"]=70, ["mark"]=50, ["darren"]=80, ["king"]=100})
        rank:dump()
        skynet.error("mark rank:", rank:zrank("mark"))
        do
            local rets = rank:zrange(1,100)
            for k, v in pairs(rets) do
                skynet.error("ranks1:", k, v)
            end
        end
        do
            local rets = rank:zrangebyscore(60,80)
            for k, v in pairs(rets) do
                skynet.error("ranks2:", k, v)
            end
        end
    end
end)
