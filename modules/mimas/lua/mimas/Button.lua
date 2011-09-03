--[[------------------------------------------------------

  mimas.Button
  ----------------

  ...

--]]------------------------------------------------------
local constr  = mimas_core.Button
local mt      = mimas_core.Button_
mimas.Button_ = mt

function mimas.Button(title, parent, func)
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

