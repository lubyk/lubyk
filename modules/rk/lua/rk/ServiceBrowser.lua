--[[------------------------------------------------------

  rk.ServiceBrowser
  -----------------

  This is a ZeroMQ based connection announced with Bonjour
  (zmq + mDNS).

--]]------------------------------------------------------
require 'mdns.Browser'

local lib = {}
lib.__index = lib
rk.ServiceBrowser  = lib

print("Loading ServiceBrowser")

local browsers = {}

--- Helper function to subscribe a service to a given device
local function subscribe_to_service(service, device)
  service.receiver:subscribe(string.format('tcp://%s:%i', device.host, device.port))
end

--- Helper function triggered whenever a new remote service is found on the network.
-- If the service can be used (on an interface we want to use), the service
-- is added to the list of services for the current service type.
--
-- If there is any pending connections matching the service name, create connections.
local function browser_add_remote_service(self, remote_service)
  self.services[remote_service.name] = remote_service
  local pending = self.pending[remote_service.name]
  if pending then
    for k, service in ipairs(pending) do
      -- service found, bind
      subscribe_to_service(service, remote_service)
    end
    self.pending[remote_service.name] = nil
  end
end

--- Creates an instance of the ServiceBrowser
-- Keep a hash of created browser by service_type
-- Always return the same browser for the given service_type
--
setmetatable(lib, {
  -- new method
 __call = function(table, service_type)
  local instance = browsers[service_type]
  if instance then
    return instance
  end
  instance = {service_type = service_type, pending = {}, services = {}}
  instance.browser = mdns.Browser(service_type, function(remote_service)
    if remote_service.add then
      browser_add_remote_service(instance, remote_service)
    end
  end)
  browsers[service_type] = instance
  setmetatable(instance, lib)
  return instance
end})

--- Connect a Service to another remote service.
--
function lib:connect_service(service, remote_name)
  local remote_service = self.services[remote_name]
  if remote_service then
    subscribe_to_service(service, remote_service)
  else
    local pending = self.pending[remote_name]
    if not pending then
      pending = {}
      self.pending[remote_name] = pending
    end
    table.insert(pending, service)
  end
end
