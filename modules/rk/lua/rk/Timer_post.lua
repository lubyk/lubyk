--[[------------------------------------------------------

  rk.Timer
  ------

  The Timer contains a callback to execute a function at
  regular intervals.

--]]------------------------------------------------------
local constr = rk.Timer
function rk.Timer(interval, func)
  return constr(worker, interval, func)
end
