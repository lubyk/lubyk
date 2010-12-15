--[[------------------------------------------------------

  mdns.Browser
  ------------

  This class registers a service name for a give service
  type and calls a callback when the service is announced.
  (this is an implementation of the ZeroConf or Bonjour
  registration).

--]]------------------------------------------------------
require 'mdns.Registration_core'
require 'worker'
local worker = worker

local constr = mdns.Registration
local function dummy_callback()
end
function mdns.Registration(service_type, name, port, func)
  func = func or dummy_callback
  return constr(worker, service_type, name, port, func)
end