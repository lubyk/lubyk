--[[------------------------------------------------------

  mdns.Browser
  ------------

  This class calls a callback whenever new devices for a
  given service type appear or are removed on the network
  (this is an implementation of the ZeroConf or Bonjour
  browser).

--]]------------------------------------------------------
require 'mdns.Browser_core'
local mt = mdns.Browser_
local constr = mdns.Browser
local private = {}

function mdns.Browser(service_type)
  local self = constr(service_type)
  self.found = {}

  self.thread = lk.Thread(function()
    local fd = self:fd()
    while true do
      sched:waitRead(fd)
      if self:getServices() then
        -- This calls DNSServiceProcessResult
        local service = self:nextService()
        while service do
          private.notifyService(self, service)
          service = self:nextService()
        end
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
  if service.op == 'add' then
    if not self.found[service.name] then
      -- only add once
      self.found[service.name] = true
      self:addDevice(service)
    end
  else
    if self.found[service.name] then
      -- only remove once
      self.found[service.name] = nil
      self:removeDevice(service)
    end
  end
end

