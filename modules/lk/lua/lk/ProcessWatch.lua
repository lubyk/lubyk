--[[------------------------------------------------------

  lk.ProcessWatch
  ---------------

  Watches for other lk.Process and creates new
  lk.RemoteProcess when one is found. This is used to
  handle inter process messages.

--]]------------------------------------------------------

local lib = {}
lib.__index     = lib
lk.ProcessWatch = lib

setmetatable(lib, {
  -- new method
 __call = function(lib)
  local service_type = lubyk.service_type
  local self = {
    browser        = lk.ServiceBrowser(service_type),
    found_services = {},
    -- weak table
    delegates      = setmetatable({}, {__mode = 'v'}),
  }
  setmetatable(self, lib)

  --======================================= SUB client
  self.sub = zmq.SimpleSub(function(...)
    -- receive message from local ServiceBrowser
    local url, service_name = ...
    if url == lubyk.add_service_url then
      -- resolve pending
      if self.found_services[service_name] then
        -- allready found
        return
      end

      local service = self.browser.services[service_name]
      local zone, name = string.match(service_name, '^([^:]+):(.*)$')
      if name == '' then
        name = nil
      end
      
      if not zone then
        print('Error in ProcessWatch: found service without zone', service_name)
        return
      end

      service.zone = zone
      service.name = name

      self.found_services[service_name] = service

      for _, delegate in ipairs(self.delegates) do
        delegate:addService(service)
      end
    elseif url == lubyk.rem_service_url then
      local service = self.found_services[service_name]
      if service then
        -- remove
        self.found_services[service_name] = nil
        for _, delegate in ipairs(self.delegates) do
          delegate:removeService(service)
        end
      end
    end
  end)

  -- so we can get connection commands from the service browser
  self.sub:connect(string.format('inproc://%s', service_type))

  return self
end})

function lib:addDelegate(delegate)
  table.insert(self.delegates, delegate)
  for _, service in pairs(self.found_services) do
    delegate:addService(service)
  end
  return self
end
