--[[------------------------------------------------------

  mimas.Label
  -----------

  ...

--]]------------------------------------------------------
local constr = mimas_core.Label
local mt     = mimas_core.Label_
mimas.Label_ = mt

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

local resize = mt.resize
function mt:resize(w, h)
  h = h or self:height()
  resize(self, w, h)
end
