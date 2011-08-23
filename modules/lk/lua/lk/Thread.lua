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
  local self = constr()
  if func then
    self.run = func
    self:start()
  end
  return self
end
