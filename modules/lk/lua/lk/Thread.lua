--[[------------------------------------------------------

  lk.Thread
  ---------

  OS Threads.

--]]------------------------------------------------------
require 'lk.Thread_core'
require 'worker'

local constr = lk.Thread
local worker = worker
function lk.Thread(func)
  local instance = constr(worker)
  if func then
    instance:start(func)
  end
  return instance
end
