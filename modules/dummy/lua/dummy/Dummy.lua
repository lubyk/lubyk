--[[------------------------------------------------------

  lk.Socket
  ---------

  BSD Socket.

--]]------------------------------------------------------
require 'dummy.Dummy_core'
require 'worker'

local constr = dummy.Dummy
local worker = worker
function dummy.Dummy()
  local instance = constr(worker)
  return instance
end
