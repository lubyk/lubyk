--[[------------------------------------------------------

  mimas.Label
  -----------

  ...

--]]------------------------------------------------------
require 'mimas'

local constr = mimas.Label_core
mimas.Label_core = nil
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

