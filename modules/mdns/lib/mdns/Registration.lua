--[[------------------------------------------------------

  mdns.Registration
  -----------------

  This class registers a service name for a give service
  type and calls a callback when the service is announced.
  (this is an implementation of the ZeroConf or Bonjour
  registration).

--]]------------------------------------------------------
require 'mdns.Registration_core'
local constr = mdns.Registration
local mt = mdns.Registration_

local function dummy()
  -- noop
end

--- We should provide a socket to inform when registration is over (callback).
-- This socket could be the default zmq.REQ socket used to by lk.Service ?
function mdns.Registration(service_type, name, port, func)
  func = func or dummy
  local self = constr(service_type, name, port)
  self.thread = lk.Thread(function()
    while true do
      sched:waitRead(self:fd())
      func(self:getService())
    end
  end)
  return self
end

