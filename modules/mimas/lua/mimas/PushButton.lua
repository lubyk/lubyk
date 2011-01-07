--[[------------------------------------------------------

  mimas.PushButton
  ----------------

  ...

--]]------------------------------------------------------
local constr = mimas_core.PushButton
local mt = nil
function mimas.PushButton(title, parent, func)
  local instance
  if not func then
    if type(parent) == 'function' then
      func = parent
      parent = nil
    end
  end
  if parent then
    instance = constr(title, parent:widget())
  else
    instance = constr(title)
  end
  if func then
    local callback = mimas.Callback(func)
    callback:connect(instance, 'clicked')
    -- callback will be deleted with button
    instance:add_to_env(callback)
  end
  return instance
end

