--[[------------------------------------------------------

  mimas.CheckBox
  --------------

  ...

--]]------------------------------------------------------
local constr    = mimas_core.CheckBox
local lib       = mimas_core.CheckBox_
mimas.CheckBox_ = lib

local MousePress = mimas.MousePress
function mimas.CheckBox(title, parent, func)
  local self
  if not func then
    if type(parent) == 'function' then
      func = parent
      parent = nil
    end
  end
  if parent then
    self = constr(title, parent:widget())
  else
    self = constr(title)
  end
  if func then
    self.click = func
  end
  return self
end

