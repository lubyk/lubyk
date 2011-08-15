--[[------------------------------------------------------

  lk.Socket
  ---------

  BSD Socket that uses msgpack to send Lua values.

--]]------------------------------------------------------
require 'lk.Socket_core'
require 'worker'

local constr = lk.Socket
local worker = worker
function lk.Socket(func)
  local instance = constr(worker, lk.Socket_const.TCP)
  if func then
    instance:loop(func)
  end
  return instance
end
