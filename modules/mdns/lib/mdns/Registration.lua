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

function mt:callback()
  -- noop
end

function mdns.Registration(service_type, name, port, func)
  local self = constr(service_type, name, port)
  if func then
    self.registrationDone = func
  end
  return self
end
