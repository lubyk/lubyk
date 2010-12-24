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
  return constr(worker)
end