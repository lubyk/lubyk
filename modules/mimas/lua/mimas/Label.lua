--[[------------------------------------------------------

  mimas.Label
  -----------

  ...

--]]------------------------------------------------------
local constr = mimas_core.Label
local mt = nil
function mimas.Label(title, parent)
  title = title or ''
  local self
  if parent then
    self = constr(title, parent:widget())
  else
    self = constr(title)
  end
  return self
end

