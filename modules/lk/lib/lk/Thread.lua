--[[------------------------------------------------------

  lk.Thread
  ---------

  OS Threads.

--]]------------------------------------------------------
require 'lk.Thread_core'

local constr = lk.Thread
function lk.Thread(func)
  local self = constr()
  if func then
    self.run = func
    self:start()
  end
  return self
end
