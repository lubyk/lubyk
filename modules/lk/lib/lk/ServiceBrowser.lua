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

local browsers = setmetatable({}, {__mode='v'})

--- Creates an instance of the ServiceBrowser
-- Keep a hash of created browser by service_type
-- Always return the same browser for the given service_type
--
setmetatable(lib, {
  -- new method
 __call = function(lib, service_type)
  local self = browsers[service_type]
  if self then
    return self
  end
  self = {
    service_type = service_type,
    pending  = {},
    services = {},
  }

  self.finalizer = lk.Finalizer(function()
    browsers[service_type] = nil
  end)

  self.browser = mdns.Browser(service_type)
  self.browser.delegates = setmetatable({}, {__mode='v'})
  function self.browser.addDevice(browser, remote_service)
    remote_service.fullname = remote_service.name
    local found_service = self.services[remote_service.fullname]
    if not found_service then
      self.services[remote_service.fullname] = remote_service
      -- start a new thread to announce object (so we do not risk crashing the
      -- listening thread)
      lk.Thread(function()
        remote_service.req = zmq.Req()
        -- new device
        remote_service.url = string.format('tcp://%s:%i', remote_service.ip, remote_service.port)
        remote_service.req:connect(remote_service.url)
        remote_service.info = remote_service.txt
        if not remote_service.info then
          -- Missing TXT record ?
          -- remote_service.info = remote_service.req:request(lubyk.info_url)
          -- error ?
          return
        end
        remote_service.sub_url  = string.format('tcp://%s:%i', remote_service.ip, remote_service.info.pub)
        remote_service.push_url = string.format('tcp://%s:%i', remote_service.ip, remote_service.info.pull)
        remote_service.push = zmq.Push()
        -- do not keep unsent messages on quit
        remote_service.push:setsockopt(zmq.LINGER, 0)
        remote_service.push:connect(remote_service.push_url)
        for _, delegate in ipairs(browser.delegates) do
          delegate:addDevice(remote_service)
        end
      end)
    end
  end

  function self.browser.removeDevice(browser, remote_service)
    remote_service.fullname = remote_service.name
    local found_service = self.services[remote_service.fullname]
    if found_service then
      -- removed device. Start a new thread in case an error occurs to avoid 
      -- crashing the listening thread.
      lk.Thread(function()
        for k, delegate in ipairs(browser.delegates) do
          delegate:removeDevice(remote_service)
        end
        self.services[remote_service.fullname] = nil
      end)
    end
  end
  browsers[service_type] = self
  setmetatable(self, lib)
  return self
end})

function lib:addDelegate(delegate)
  table.insert(self.browser.delegates, delegate)
  -- send all notifications ?
  return self
end
