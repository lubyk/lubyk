--[[------------------------------------------------------

  rk.Client
  ---------

  The Client can subscribe and send requests to named
  services (discovered on the network with mDNS).

  TODO: Requests should be sent asynchronously with the
  zmq.Push sockets or synchronously with zmq.Req.

--]]------------------------------------------------------

local lib = {}
lib.__index = lib
rk.Client   = lib

setmetatable(lib, {
  -- new method
 __call = function(table, service_type, callback)
  if not callback then
    callback     = service_type
    service_type = lubyk.service_type
  end
  if not callback then
    -- dummy
    callback = function() end
  end

  local instance = {browser = rk.ServiceBrowser(service_type), callback = callback, subscriptions = {}}

  --======================================= SUB client

  instance.sub = zmq.SimpleSub(function(...)
    -- receive message from remote server or local ServiceBrowser
    local url, service_name = ...
    if url == lubyk.add_service_url then
      -- resolve pending
      local remote_service = instance.browser.services[service_name]
      local subscription = instance.subscriptions[service_name]
      if subscription and not subscription.connected then
        subscription.subscriber:connect(remote_service.sub_url)
        subscription.connected = true
      end
    elseif url == lubyk.rem_service_url then
      -- remove connection
      local subscription = instance.subscriptions[service_name]
      if subscription then
        -- TODO: do we need to disconnect ?
        subscription.connected = false
      end
    else
      -- data received from remote server
      instance.callback(...)
    end
  end)
  -- so we can get connection commands from the service browser
  instance.sub:connect(string.format('inproc://%s', service_type))

  --======================================= REQ client
  instance.req = zmq.Req()

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
  self.req:connect(service.url)
  return self.req:request(...)
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

