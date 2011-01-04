--[[------------------------------------------------------

  mimas.Application
  --------------

  GUI Application.

--]]------------------------------------------------------
require 'mimas.Application_core'
require 'worker'
local worker = worker
local constr = mimas.Application
function mimas.Application()
  local instance = constr(worker)
  instance:setStyleSheet([[
    QLabel { background: transparent; color: #ccc; }
  ]])
  return instance
end