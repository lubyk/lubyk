--[[------------------------------------------------------

  lk.Client
  ---------

  The Client can subscribe and send requests to named
  services (discovered on the network with mDNS).

  TODO: Requests should be sent asynchronously with the
  zmq.Push sockets or synchronously with zmq.Req.

--]]------------------------------------------------------

local lib = {type='lk.Client'}
lib.__index = lib
lk.Client   = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, service_type, callback)
  if not callback then
    callback     = service_type
    service_type = lubyk.service_type
  end
  if not callback then
    -- dummy
    callback = function() end
  end

  local self = {callback = callback, subscriptions = {}}
  self.browser = lk.ServiceBrowser(service_type):addDelegate(self) 

  --======================================= SUB client

  self.sub = zmq.SimpleSub(self.callback)

  setmetatable(self, lib)
  return self
end})

function lib:addDevice(service)
  -- resolve pending
  local subscription = self.subscriptions[service.name]
  if subscription and not subscription.connected then
    subscription.subscriber:connect(service.sub_url)
    subscription.connected = true
  end
end

function lib:removeDevice(service)
  -- remove connection
  local subscription = self.subscriptions[service.name]
  if subscription then
    -- TODO: do we need to disconnect ?
    subscription.connected = false
  end
end

function lib:subscribe(remote_name)
  if self.subscriptions[remote_name] then
    -- allready subscribed, abort
    return
  end
  local subscription = {subscriber = self.sub}
  self.subscriptions[remote_name] = subscription
  local service = self.browser.services[remote_name]
  if service then
    self.sub:connect(service.sub_url)
    subscription.connected = true
  end
end

function lib:request(service_name, ...)
  local service = self.browser.services[service_name]
  if not service then
    return false, string.format('Cannot request to %s (service not found)', service_name)
  end
  return service.req:request(...)
end

function lib:send(service_name, ...)
  local service = self.browser.services[service_name]
  if not service then
    return false, string.format('Cannot send to %s (service not found)', service_name)
  else
    service.push:send(...)
    return true
  end
end

function lib:connected()
  for _, connection in pairs(self.subscriptions) do
    if not connection.connected then
      return false
    end
  end
  return true
end

