--[[------------------------------------------------------

  mimas.LineEdit
  --------------

  ...

--]]------------------------------------------------------
local constr = mimas_core.LineEdit
local mt = mimas_core.LineEdit_
local worker = worker
--function mt:__newindex

function mimas.LineEdit(title, parent)
  title = title or ''
  local self
  if parent then
    self = constr(worker, title, parent:widget())
  else
    self = constr(worker, title)
  end
  return self
end

