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

local function handleDevice(self, service)
  if service.op == 'add' then
    self:addDevice(service)
  else
    self:removeDevice(service)
  end
end

local function notifyService(self, service)
  if service.op == 'add' then
    if not self.found[service.name] then
      -- only add once
      self.found[service.name] = true
      self.func(self, service)
    end
  else
    if self.found[service.name] then
      -- only remove once
      self.found[service.name] = nil
      self.func(self, service)
    end
  end
end

function mdns.Browser(service_type, func)
  local self = constr(service_type)
  self.found = {}
  self.func  = func or handleDevice

  self.thread = lk.Thread(function()
    local fd = self:fd()
    while true do
      sched:waitRead(fd)
      if self:getServices() then
        -- This calls DNSServiceProcessResult
        local service = self:nextService()
        while service do
          notifyService(self, service)
          service = self:nextService()
        end
      end
    end
  end)
  return self
end
