--[[------------------------------------------------------

  mimas.Window
  ------------

  ...

--]]------------------------------------------------------
local constr = mimas_core.Window
local worker = worker
function mimas.Window(...)
  return constr(worker, ...)
end
