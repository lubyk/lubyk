--[[------------------------------------------------------

  mimas.Slider
  ------------

  Async slider.

--]]------------------------------------------------------
local constr  = mimas_core.Slider
local mt      = mimas_core.Slider_
mimas.Slider_ = mt

function mimas.Slider(slider_type, func, parent)
  local instance
  if type(func) == 'userdata' then
    parent = func
    func = nil
  end
  if parent then
    instance = constr(worker, slider_type, parent:widget())
  else
    instance = constr(worker, slider_type)
  end
  if func then
    instance:set_callback(func)
  end
  return instance
end

