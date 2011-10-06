--[[------------------------------------------------------

  mimas.Window
  ------------

  ...

--]]------------------------------------------------------
require 'mimas.Widget'
mimas.Window_ = mimas.Widget_

local constr  = mimas_core.Widget
function mimas.Window(...)
  return mimas.bootstrap('Window', constr, ...)
end

