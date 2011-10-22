--[[------------------------------------------------------

  mimas.Timer
  -----------

  Simple timer.

--]]------------------------------------------------------
local constr               = mimas_core.Timer
local mt                   = mimas_core.Timer_
mimas.Timer_      = mt
mimas.Timer       = constr
mimas.Timer_const = mimas_core.Timer_const

local function Timer(msec, func)
  local self = constr(msec)
  if func then
    self.timeout = func
  end
  return self
end

function mimas.Timer(...)
  return mimas.bootstrap('Timer', Timer, ...)
end

