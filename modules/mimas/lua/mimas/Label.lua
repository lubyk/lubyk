--[[------------------------------------------------------

  mimas.Label
  -----------

  ...

--]]------------------------------------------------------
require 'mimas'
require 'mimas.Label_core'

local constr = mimas.Label
local mt = nil
function mimas.Label(title, parent)
  local instance
  if parent then
    instance = constr(title, parent:widget())
  else
    instance = constr(title)
  end
  return instance
end

