--[[------------------------------------------------------

  lk.Socket
  ---------

  BSD Socket that uses msgpack to send Lua values.

--]]------------------------------------------------------
require 'lk.Socket_core'
local constr = lk.Socket
local lib    = lk.SubClass(lk, 'Socket')
lk.Socket    = lib

lib.type = 'lk.Socket'
lib.TCP  = lk.Socket_const.TCP
lib.UDP  = lk.Socket_const.UDP

function lib.new(func)
  local self = constr(lib.TCP)
  self.super:setNonBlocking()
  self.clients = {}
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
  self.sock_fd = self.super:fd()
  return self.port
end

function lib:connect(...)
  local ok = self.super:connect(...)
  self.sock_fd = self.super:fd()
  if not ok then
    sched:waitWrite(self.sock_fd)
    self.super:connectFinish()
  end
end

function lib:recv(len)
  if not len or len == '*l' then
    return self:recvLine()
  end
  local buffer = self.buffer or ''
  local res    = ''
  while true do
    local blen = string.len(buffer)
    if blen >= len then
      res = res .. string.sub(buffer, 1, len)
      if len < blen then
        self.buffer = string.sub(buffer, len+1)
      else
        self.buffer = nil
      end
      return res
    end
    -- we need more
    sched:waitRead(self.sock_fd)
    buffer = self.super:recv('*a')
  end
end

function lib:recvLine()
  local buffer = self.buffer
  while true do
    if buffer then
      local line, rest = string.match(buffer, '^([^\r\n]*)\r?\n(.*)$')
      if line then
        self.buffer = rest
        return line
      end
    end
    sched:waitRead(self.sock_fd)
    local data = self.super:recv('*a')
    if not data then
      -- connection closed
      -- FIXME: What should we do here ? error ? reconnect ?
      sched:abort()
    end
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
    -- protect from GC
    self.clients[cli.sock_fd] = cli
    cli.thread = lk.Thread(function()
      func(cli)
      -- forget about the client
      self.clients[cli.sock_fd] = nil
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
