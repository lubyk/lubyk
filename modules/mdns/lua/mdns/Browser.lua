--[[------------------------------------------------------

  mdns.Browser
  ------------

  This class calls a callback whenever new devices for a
  given service type appear or are removed on the network
  (this is an implementation of the ZeroConf or Bonjour
  browser).

--]]------------------------------------------------------
require 'mdns.Browser_core'
require 'worker'
local worker = worker

local constr = mdns.Browser
function mdns.Browser(service_type, func)
  return constr(worker, service_type, func)
end