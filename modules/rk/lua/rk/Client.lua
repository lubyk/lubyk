--[[------------------------------------------------------

  rk.Client
  ---------

  The Client can subscribe and send requests to named
  services (that are discovered on the network with mDNS).

--]]------------------------------------------------------

local lib = {}
lib.__index = lib
rk.Client   = lib

setmetatable(lib, {
  -- new method
 __call = function(table, service_type, callback)
  if not callback then
    callback = service_type
    service_type = rubyk.service_type
  end
  if not callback then
    -- dummy
    callback = function() end
  end
  local instance = {browser = rk.ServiceBrowser(service_type), callback = callback, subscriptions = {}}

  --======================================= SUB client
  instance.sub = zmq.Sub(function(...)
    instance.callback(...)
  end)

  setmetatable(instance, lib)
  return instance
end})


function lib:subscribe(remote_name)
  if self.subscriptions[remote_name] then
    -- allready subscribed, abort
    return
  end
  subscription = {subscriber = self.sub}
  self.subscriptions[remote_name] = subscription
  self.browser:connect(remote_name, subscription)
end

function lib:request(service_name, ...)
  local service = self.browser.services[service_name]
  if not service then
    print(string.format('Cannot send to %s (service not found)', service_name))
    return nil
  end
  return service:request(...)
end

function lib:connected()
  for _, connection in pairs(self.subscriptions) do
    if not connection.connected then
      return false
    end
  end
  return true
end

