--[[------------------------------------------------------

  mdns.Browser
  ------------

  This class calls a callback whenever new devices for a
  given service type appear or are removed on the network
  (this is an implementation of the ZeroConf or Bonjour
  browser).

--]]------------------------------------------------------
require 'mdns.Browser_core'
-- mdns.Service mt must be loaded
require 'mdns.Service'
local lib = mdns.Browser_
local constr = mdns.Browser
local private = {}

function mdns.Browser(service_type, func)
  local self = constr(service_type)
  if func then
    self.addDevice = func
    self.removeDevice = func
  end

  self.found = {}

  self.thread = lk.Thread(function()
    local fd = self:fd()
    while true do
      sched:waitRead(fd)
      local service = self:getService()
      while service do
        private.notifyService(self, service)
        service = self:getService()
      end
    end
  end)
  return self
end

--=============================================== Dummy implementations
function lib:addDevice(service)
end

function lib:removeDevice(service)
end

--=============================================== PRIVATE
function private:notifyService(service)
  local name = service:name()
  if service:isAdd() then
    if not self.found[name] then
      -- only add once
      self.found[name] = service
      service.thread = lk.Thread(function()
        local fd = service:fd()
        -- Resolve the service name
        while true do
          sched:waitRead(fd)
          if self.found[name] then
            -- Hasn't been removed before resolution.
            local info = service:info()
            if info then
              service.info = info
              -- only notify once for now
              self:addDevice(info)
              break
            end
          end
        end
        -- The mdns.Service should be deleted as soon as possible now so that we free the
        -- socket filedescriptor.
        service:__gc()
        self.found[name] = true
      end)
    end
  elseif self.found[name] then
    -- only remove once
    self.found[name] = nil
    self:removeDevice({name = name})
  end
end

