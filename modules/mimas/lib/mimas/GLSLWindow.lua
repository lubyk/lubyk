--[[------------------------------------------------------

  mimas.GLSLWindow
  --------------

  A window to draw modern OpenGL content.

--]]------------------------------------------------------
-- Make sure we load alterations of the metatable
require 'mimas.GLSLWidget'
-- Constr is the original constructor, not the bootstraping alias
local constr  = mimas_core.GLSLWidget
function mimas.GLSLWindow(...)
  return mimas.bootstrap('GLSLWindow', constr, ...)
end

