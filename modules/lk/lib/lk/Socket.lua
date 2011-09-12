--[[------------------------------------------------------

  lk.Socket
  ---------

  BSD Socket that uses msgpack to send Lua values.

--]]------------------------------------------------------
require 'lk.Socket_core'
constr    = lk.Socket
local lib = lk.SubClass(lk, 'Socket')
lk.Socket = lib

lib.type = 'lk.Socket'
lib.TCP  = lk.Socket_const.TCP
lib.UDP  = lk.Socket_const.UDP

function lib.new(func)
  local self = constr(lib.TCP)
  if func then
    sched:thread(function()
      func(self)
    end)
  end
  return setmetatable(self, lib)
end

function lib:bind(host, port)
  self.host    = host
  self.port    = self.super:bind(host, port)
  self.super:setNonBlocking()
  self.sock_fd = self.super:fd()
  return self.port
end

function lib:recvLine()
  local buffer = self.buffer
  while true do
    if buffer then
      local line, rest = string.match(buffer, '([^\r\n]*)\r?\n(.*)')
      if line then
        self.buffer = rest
        return line
      end
    end
    sched:waitRead(self.sock_fd)
    local data = self.super:recv('*a')
    if buffer then
      buffer = buffer .. data
    else
      buffer = data
    end
  end
end

function lib:send(data)
  local buffer = data
  while true do
    local sent = self.super:send(data)
    if sent == string.len(data) then
      break
    else
      data = string.sub(data, sent + 1)
    end
    sched:waitWrite(self.sock_fd)
  end
end

function lib:accept()
  sched:waitRead(self.sock_fd)
  local cli = self.super:accept()
  cli.sock_fd = cli.super:fd()
  return setmetatable(cli, lib)
end
--[[
--=============================================== ???? old stuff
local function checkLoop(self)
  if not self.loop then
    error("Loop not set")
  end
end

function mt:shouldRun()
  checkLoop(self)
  return self.loop:shouldRun()
end

function mt:start()
  checkLoop(self)
  self.loop:start()
end

function mt:quit()
  checkLoop(self)
  self.loop:quit()
end

function mt:kill()
  checkLoop(self)
  self.loop:kill()
end

function mt:join()
  checkLoop(self)
  self.loop:join()
end
--]]
