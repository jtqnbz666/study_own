local skynet = require "skynet"
local queue = require "skynet.queue"
local socket = require "skynet.socket"

local cs = queue()

local tinsert = table.insert
local tremove = table.remove
-- local tconcat = table.concat
local CMD = {}

local queues = {}

local resps = {}

local function sendto(clientfd, arg)
    -- local ret = tconcat({"fd:", clientfd, arg}, " ")
    -- socket.write(clientfd, ret .. "\n")
    socket.write(clientfd, arg .. "\n")
end

function CMD.ready(client)
    if not client or not client.name then
        return skynet.retpack(false, "准备：非法操作")
    end
    if resps[client.name] then
        return skynet.retpack(false, "重复准备")
    end
    tinsert(queues, 1, client)
    resps[client.name] = skynet.response()  -- 细节延时返回，返回值是一个函数
    if #queues >= 3 then
        local roomd = skynet.newservice("room")
        local members = {tremove(queues), tremove(queues), tremove(queues)}
        for i=1, 3 do
            local cli = members[i]
            resps[cli.name](true, roomd) -- true 发送应答， PTYPE_RESPINSE;
            resps[cli.name] = nil
        end
        skynet.send(roomd, "lua", "start", members)
        return
    end
    sendto(client.fd, "等待其他玩家加入")
end

function CMD.offline(name)
    for pos, client in ipairs(queues) do
        if client.name == name then
            tremove(queues, pos)
            break
        end
    end
    if resps[name] then
        resps[name](true, false, "退出")
        resps[name] = nil
    end
    skynet.retpack()
end

skynet.start(function ()
    skynet.dispatch("lua", function(session, address, cmd, ...)
        local func = CMD[cmd]
        if not func then
            skynet.retpack({ok = false, msg = "非法操作"})
            return
        end
        cs(func, ...)
    end)
end)
