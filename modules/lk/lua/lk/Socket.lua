--[[------------------------------------------------------

  lk.Socket
  ---------

  BSD Socket that uses msgpack to send Lua values.

--]]------------------------------------------------------
require 'lk.Socket_core'

local constr = lk.Socket
local mt = lk.Socket_

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

function lk.Socket(func)
  local self = constr(lk.Socket_const.TCP)
  if func then
    self.loop = lk.Thread(function()
      func(self)
    end)
  end
  return self
end
