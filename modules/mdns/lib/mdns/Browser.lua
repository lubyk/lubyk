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

function mdns.Browser(service_type, func)
  local self = constr(service_type)
  self.thread = lk.Thread(function()
    while true do
      sched:waitRead(self:fd())
      -- This calls DNSServiceProcessResult
      func(self:getService())
    end
  end)
  return self
end
