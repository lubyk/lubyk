--[[------------------------------------------------------

  mimas.GLWindow
  --------------

  A window to draw OpenGL content.

--]]------------------------------------------------------
local constr  = mimas_core.GLWidget
function mimas.GLWindow(...)
  return mimas.bootstrap('GLWindow', constr, ...)
end
