--[[------------------------------------------------------

  rk.Thread
  ---------

  OS Threads.

--]]------------------------------------------------------
require 'rk.Thread_core'
require 'worker'

local constr = rk.Thread
local worker = worker
function rk.Thread(func)
  local instance = constr(worker)
  if func then
    instance:start(func)
  end
  return instance
end
