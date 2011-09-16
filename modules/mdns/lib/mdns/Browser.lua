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

function mdns.Browser(service_type, func)
  local self = constr(service_type)
  func = func or handleDevice

  self.thread = lk.Thread(function()
    while true do
      sched:waitRead(self:fd())
      -- This calls DNSServiceProcessResult
      func(self, self:getService())
    end
  end)
  return self
end
