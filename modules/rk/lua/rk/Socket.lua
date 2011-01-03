--[[------------------------------------------------------

  rk.Thread
  ---------

  BSD Socket.

--]]------------------------------------------------------
require 'rk.Socket_core'
require 'worker'

local constr = rk.Socket
local worker = worker
function rk.Socket(...)
  return constr(worker, ...)
end
