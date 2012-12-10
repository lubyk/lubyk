--[[------------------------------------------------------

  lk.Socket
  ---------

  BSD Socket that uses msgpack to send Lua values.

--]]------------------------------------------------------
require 'lk.core'
local lib    = lk.Socket_core
lk.Socket    = lib

-- Rewrite new
local constr = lib.new
function lib.new(func)
  local self = constr(lib.TCP)
  self:setNonBlocking()
  self.clients = {}
  if func then
    self.thread = lk.Thread(function()
      func(self)
    end)
  end
  return self
end

local bind = lib.bind
function lib:bind(host, port)
  self.host    = host
  self.port    = bind(self.super, host, port)
  self.sock_fd = self:fd()
  return self.port
end

local connect = lib.connect
function lib:connect(...)
  local super = self.super
  local ok = connect(super, ...)
  self.sock_fd = super:fd()
  if not ok then
    sched:waitWrite(self.sock_fd)
    super:connectFinish()
  end
end

local recv = lib.recv
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
    buffer = recv(self.super, '*a')
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
    local data = recv(self.super, '*a')
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

local send = lib.send
function lib:send(data)
  local buffer = data
  while true do
    local sent = send(self.super, data)
    if sent == string.len(data) then
      break
    else
      data = string.sub(data, sent + 1)
    end
    sched:waitWrite(self.sock_fd)
  end
end

local accept = lib.accept
function lib:accept(func)
  sched:waitRead(self.sock_fd)
  local cli = accept(self.super)
  cli:setNonBlocking()
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
  return cli
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
