--[[------------------------------------------------------

  rk.Timer
  ------

  The Timer contains a callback to execute a function at
  regular intervals. This file helps hide worker dependency
  and simplifies timer creation.

--]]------------------------------------------------------
require 'rk.Timer_core'
require 'worker'

local constr = rk.Timer
local worker = worker
function rk.Timer(interval, func)
  return constr(worker, interval, func)
end
