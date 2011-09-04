--[[------------------------------------------------------

  mimas.Action
  ------------

  An action is a way to connect a callback with some menu
  or shortcut.

--]]------------------------------------------------------
local constr  = mimas_core.Action
local mt      = mimas_core.Action_
mimas.Action_ = mt

function mimas.Action(title, parent, func)
  local self
  if not func then
    if type(parent) == 'function' then
      func = parent
      parent = nil
    end
  end
  if parent then
    self = constr(title, parent:object())
  else
    self = constr(title)
  end
  if func then
    self.trigger = func
  end
  return self
end
