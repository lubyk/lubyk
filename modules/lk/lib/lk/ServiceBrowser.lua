--[[------------------------------------------------------

  lk.ServiceBrowser
  -----------------

  The ServiceBrowser listens for incoming services and
  publishes them in the application with ipc://service_type
  url.

  TODO: Merge with ProcessWatch.

--]]------------------------------------------------------
local lib = {type='lk.ServiceBrowser'}
lib.__index = lib
lk.ServiceBrowser  = lib

-- FIXME: use weak tables (and use a finalizer to remove from list)
local browsers = {} --setmetatable({}, {__mode='v'})

--- Creates an instance of the ServiceBrowser
-- Keep a hash of created browser by service_type
-- Always return the same browser for the given service_type
--
setmetatable(lib, {
  -- new method
 __call = function(lib, service_type)
  local instance = browsers[service_type]
  if instance then
    return instance
  end
  instance = {
    service_type = service_type,
    pending  = {},
    services = {},
  }

  instance.browser = mdns.Browser(service_type)
  instance.browser.delegates = setmetatable({}, {__mode='v'})
  function instance.browser:addDevice(remote_service)
    remote_service.fullname = remote_service.name
    local found_service = instance.services[remote_service.fullname]
    if not found_service then
      -- start a new thread to announce object (so we do not risk crashing the
      -- listening thread)
      lk.Thread(function()
        remote_service.req = zmq.Req()
        -- new device
        remote_service.url = string.format('tcp://%s:%i', remote_service.ip, remote_service.port)
        remote_service.req:connect(remote_service.url)
        remote_service.info = remote_service.req:request(lubyk.info_url)
        if not remote_service.info then
          -- Server died on us
          return
        end
        remote_service.sub_url  = string.format('tcp://%s:%i', remote_service.ip, remote_service.info.pub)
        remote_service.push_url = string.format('tcp://%s:%i', remote_service.ip, remote_service.info.pull)
        remote_service.push = zmq.Push()
        -- do not keep unsent messages on quit
        remote_service.push:setsockopt(zmq.LINGER, 0)
        remote_service.push:connect(remote_service.push_url)
        instance.services[remote_service.fullname] = remote_service
        for _, delegate in ipairs(self.delegates) do
          delegate:addDevice(remote_service)
        end
      end)
    end
  end

  function instance.browser:removeDevice(remote_service)
    remote_service.fullname = remote_service.name
    local found_service = instance.services[remote_service.fullname]
    if found_service then
      -- removed device. Start a new thread in case an error occurs to avoid 
      -- crashing the listening thread.
      lk.Thread(function()
        for k, delegate in ipairs(self.delegates) do
          delegate:removeDevice(remote_service)
        end
        instance.services[remote_service.fullname] = nil
      end)
    end
  end
  browsers[service_type] = instance
  setmetatable(instance, lib)
  return instance
end})

function lib:addDelegate(delegate)
  table.insert(self.browser.delegates, delegate)
  -- send all notifications ?
  return self
end
