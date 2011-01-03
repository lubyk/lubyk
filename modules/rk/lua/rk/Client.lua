--[[------------------------------------------------------

  rk.Client
  ---------

  The Client can subscribe and send requests to named
  services (discovered on the network with mDNS).

--]]------------------------------------------------------

local lib = {}
lib.__index = lib
rk.Client   = lib

setmetatable(lib, {
  -- new method
 __call = function(table, service_type, callback)
  if not callback then
    callback     = service_type
    service_type = rubyk.service_type
  end
  if not callback then
    -- dummy
    callback = function() end
  end

  local instance = {browser = rk.ServiceBrowser(service_type), callback = callback, subscriptions = {}}

  --======================================= SUB client

  instance.sub = zmq.Sub(function(sub)
    -- setup function
    -- connect via ipc call to ServiceBrowser so that the latter can send messages to connect.
    sub:connect(string.format('inproc://%s', service_type))
  end, function(...)
    local url, remote_service = ...
    if url == rubyk.add_service_url then
      -- resolve pending
      local subscription = instance.subscriptions[remote_service.name]
      if subscription then
        if not instance.req then
          -- create zmq.REQ socket in the thread that's using it
          instance.req = zmq.Req()
        end

        instance.req:connect(remote_service.url)
        local sub_url = string.format('tcp://%s:%i', remote_service.host, instance.req:request(rubyk.sub_port_url))
        subscription.subscriber:connect(sub_url)
        subscription.connected = true
      end
    elseif url == rubyk.rem_service_url then
      -- remove connection
      -- ignore
    else
      -- handle other data with callback
      instance.callback(...)
    end
  end)

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
    -- FIXME: make sure this is ok (as 'sub' was not created in this thread)
    self.sub:connect(service.url)
    subscription.connected = true
  end
end

function lib:request(service_name, ...)
  local service = self.browser.services[service_name]
  if not service then
    print(string.format('Cannot send to %s (service not found)', service_name))
    return nil
  end
  self.req:connect(service.url)
  return self.req:request(...)
end

lib.send = lib.request

function lib:connected()
  for _, connection in pairs(self.subscriptions) do
    if not connection.connected then
      return false
    end
  end
  return true
end

