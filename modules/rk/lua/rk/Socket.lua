--[[------------------------------------------------------

  rk.Thread
  ---------

  BSD Socket.

--]]------------------------------------------------------
require 'rk.Socket_core'
require 'worker'

local constr = rk.Socket
local worker = worker
function rk.Socket(func)
  local instance = constr(worker)
  if func then
    instance:loop(func)
  end
  return instance
end
