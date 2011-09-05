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
function lk.Timer(interval, func)
  local self = constr(interval)
  if func then
    self.tick = func
  end
  return self
end
