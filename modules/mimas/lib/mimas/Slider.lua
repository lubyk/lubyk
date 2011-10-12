--[[------------------------------------------------------

  mimas.Slider
  ------------

  Async slider.

--]]------------------------------------------------------
local constr  = mimas_core.Slider
local mt      = mimas_core.Slider_
mimas.Slider_ = mt

function mimas.Slider(slider_type, func, parent)
  local self
  if type(func) ~= 'function' then
    parent = func
    func = nil
  end
  if parent then
    self = constr(slider_type, parent:widget())
  else
    self = constr(slider_type)
  end
  if func then
    self.sliderChanged = func
  end
  return self
end

