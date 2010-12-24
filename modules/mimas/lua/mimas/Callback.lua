--[[------------------------------------------------------

  mimas.Callback
  --------------

  GUI event callback (can be connected to widget signals).

--]]------------------------------------------------------
require 'mimas.Callback_core'
require 'worker'
local worker = worker
local constr = mimas.Callback
function mimas.Callback(func)
  return constr(worker, func)
end