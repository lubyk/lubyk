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
    print('Start browse thread')
    local fd = self:fd()
    while true do
      sched:waitRead(fd)
      printf("Something on %i.", fd)
      local service = self:getService()
      if service then
        private.notifyService(self, service)
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
        while true do
          sched:waitRead(fd)
          local info = service:info()
          print(info.op, info.name, info.host, info.port)
          if info then
            service.last_info = info
            -- only notify once for now
            if not service.added then
              service.added = true
              self:addDevice(info)
            else
              -- update
              print('UPDATE', info.name, info.host, info.port)
            end
          end
        end
      end)
    end
  elseif self.found[name] then
    print('REMOVE', name)
    -- only remove once
    local service = self.found[name]
    service.thread:kill()
    self.found[name] = nil
    self:removeDevice(service.last_info or {name = name})
  end
end

