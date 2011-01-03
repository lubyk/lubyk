--[[------------------------------------------------------

  rk.Thread
  ---------

  OS Threads.

--]]------------------------------------------------------
require 'rk.Thread_core'
require 'worker'

local constr = rk.Thread
local worker = worker
function rk.Thread(...)
  return constr(worker, ...)
end
