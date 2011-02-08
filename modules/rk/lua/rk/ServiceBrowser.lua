--[[------------------------------------------------------

  rk.ServiceBrowser
  -----------------

  The ServiceBrowser listens for incoming services and
  publishes them in the application with ipc://service_type
  url.

--]]------------------------------------------------------
local lib = {}
lib.__index = lib
rk.ServiceBrowser  = lib

local browsers = {}

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

  -- We must bind before any client does a subscribe.
  instance.pub = zmq.Pub(string.format('inproc://%s', service_type))

  instance.browser = mdns.Browser(service_type, function(remote_service)
    local found_service = instance.services[remote_service.name]
    if remote_service.op == 'add' then
      if not found_service then
        local req = zmq.Req()
        -- new device
        remote_service.url = string.format('tcp://%s:%i', remote_service.host, remote_service.port)
        -- XXX
        req:connect(remote_service.url)
        remote_service.info = req:request(lubyk.info_url)
        remote_service.sub_url  = string.format('tcp://%s:%i', remote_service.host, remote_service.info.pub)
        remote_service.push_url = string.format('tcp://%s:%i', remote_service.host, remote_service.info.pull)
        remote_service.push = zmq.Push()
        -- do not keep unsent messages on quit
        remote_service.push:setsockopt(zmq.LINGER, 0)
        remote_service.push:connect(remote_service.push_url)
        -- XXX
        instance.services[remote_service.name] = remote_service
        instance.pub:send(lubyk.add_service_url, remote_service.name)
      end
    elseif found_service then
      -- removed device
      instance.pub:send(lubyk.rem_service_url, remote_service.name)
      instance.services[remote_service.name] = nil
    end
  end)
  browsers[service_type] = instance
  setmetatable(instance, lib)
  return instance
end})

