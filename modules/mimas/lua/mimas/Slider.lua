--[[------------------------------------------------------

  mimas.Slider
  ------------

  Async slider.

--]]------------------------------------------------------
require 'mimas.Slider_core'
require 'worker'
local worker = worker
local constr = mimas.Slider
function mimas.Slider(type, func)
  local instance = constr(worker, type)
  if func then
    instance:set_callback(func)
  end
  return instance
end

