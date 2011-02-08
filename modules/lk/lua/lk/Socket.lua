--[[------------------------------------------------------

  lk.Socket
  ---------

  BSD Socket.

--]]------------------------------------------------------
require 'lk.Socket_core'
require 'worker'

local constr = lk.Socket
local worker = worker
function lk.Socket(type, func)
  local instance = constr(worker, type)
  if func then
    instance:loop(func)
  end
  return instance
end
