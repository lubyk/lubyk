--[[------------------------------------------------------

  mimas.PushButton
  ----------------

  ...

--]]------------------------------------------------------
require 'mimas'
require 'mimas.PushButton_core'

local constr = mimas.PushButton
local mt = nil
function mimas.PushButton(title, parent)
  local instance
  if parent then
    instance = constr(title, parent:widget())
  else
    instance = constr(title)
  end
  return instance
end

