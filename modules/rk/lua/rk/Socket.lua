--[[------------------------------------------------------

  rk.Socket
  ---------

  BSD Socket.

--]]------------------------------------------------------
require 'rk.Socket_core'
require 'worker'

local constr = rk.Socket
local worker = worker
function rk.Socket(type, func)
  local instance = constr(worker, type)
  if func then
    instance:loop(func)
  end
  return instance
end
