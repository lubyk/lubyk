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
    self.thread = lk.Thread(function()
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

function lib:accept(func)
  sched:waitRead(self.sock_fd)
  local cli = self.super:accept()
  cli.super:setNonBlocking()
  cli.sock_fd = cli:fd()
  if func then
    -- start new thread
    cli.thread = sched:thread(function()
      func(cli)
    end)
  end
  return setmetatable(cli, lib)
end

function lib:shouldRun()
  return self.thread and self.thread:shouldRun()
end

function lib:quit()
  if self.thread then
    self.thread:quit()
  end
end

function lib:kill()
  if self.thread then
    self.thread:kill()
  end
end

function lib:join()
  if self.thread then
    self.thread:join()
  end
end
