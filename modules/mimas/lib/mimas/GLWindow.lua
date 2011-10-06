--[[------------------------------------------------------

  mimas.GLWindow
  --------------

  A window to draw OpenGL content.

--]]------------------------------------------------------
-- Make sure we load alterations of the metatable
require 'mimas.GLWidget'
-- Constr is the original constructor, not the bootstraping alias
local constr  = mimas_core.GLWidget
function mimas.GLWindow(...)
  return mimas.bootstrap('GLWindow', constr, ...)
end
