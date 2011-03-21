--[[------------------------------------------------------

  lk.Timer
  ------

  The Timer contains a callback to execute a function at
  regular intervals. This file helps hide worker dependency
  and simplifies timer creation.

--]]------------------------------------------------------
require 'lk.Timer_core'
require 'worker'

local constr = lk.Timer
local worker = worker
function lk.Timer(interval, func)
  local instance = constr(worker, interval)
  if func then
    instance.tick = func
  end
  return instance
end
