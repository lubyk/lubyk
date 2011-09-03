--[[------------------------------------------------------

  mimas.LineEdit
  --------------

  ...

--]]------------------------------------------------------
local constr = mimas_core.LineEdit
local mt     = mimas_core.LineEdit_
mimas.LineEdit_ = mt

function mimas.LineEdit(title, parent)
  title = title or ''
  local self
  if parent then
    self = constr(title, parent:widget())
  else
    self = constr(title)
  end
  return self
end

