--[[------------------------------------------------------

  mimas.Window
  ------------

  ...

--]]------------------------------------------------------
require 'mimas.Widget'
mimas.Window_ = mimas.Widget_

local constr  = mimas.Widget_constr
function mimas.Window(...)
  return mimas.bootstrap('Window', constr, ...)
end

