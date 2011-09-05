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
  if func then
    self.addDevice = func
    self.removeDevice = func
  end
  return self
end
