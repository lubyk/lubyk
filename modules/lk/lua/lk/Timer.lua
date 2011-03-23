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
local function dummy_func()
end

function lk.Timer(interval, func)
  local instance = constr(worker, interval)
  if func then
    instance.tick = func
  else
    instance.tick = dummy_func
  end
  return instance
end
