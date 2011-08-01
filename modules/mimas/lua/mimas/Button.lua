--[[------------------------------------------------------

  mimas.Button
  ----------------

  ...

--]]------------------------------------------------------
local constr = mimas_core.Button
local mt = nil
local worker = worker
function mimas.Button(title, parent, func)
  local self
  if not func then
    if type(parent) == 'function' then
      func = parent
      parent = nil
    end
  end
  if parent then
    self = constr(worker, title, parent:widget())
  else
    self = constr(worker, title)
  end
  if func then
    self.click = func
    -- THIS IS LEGACY CODE THAT SHOULD BE REMOVED (with code in Button).
    -- local callback = mimas.Callback(func)
    -- callback:connect(self, 'clicked')
    -- -- callback will be deleted with button
    -- self:add_to_env(callback)
  end
  return self
end

