--[[------------------------------------------------------

  rk.Service
  ----------

  This is a ZeroMQ based connection announced with Bonjour
  (zmq + mDNS).

--]]------------------------------------------------------
require 'mdns.Registration'
require 'zmq.Receive'
require 'zmq.Send'

require 'rk.ServiceBrowser'

local lib = {}
lib.__index = lib
rk.Service  = lib

-- FIXME: this is stupid. Fix zmq.Receive to accept 0 as port number.
local port_nb = 7000
setmetatable(lib, {
  -- new method
 __call = function(table, name, service_type, callback)
  local instance = {name = name, browser = rk.ServiceBrowser(service_type), callback = callback}

  port_nb = port_nb + 1
  instance.port = port_nb

  -- receives zmq packets
  instance.receiver = zmq.ReceiveSocket(string.format("tcp://localhost:%i", port_nb), function(message)
    -- we do not pass callback directly so that we can update the function with instance.callback=..
    instance.callback(instance, message)
  end)

  -- sends zmq packets
  port_nb = port_nb + 1
  instance.publisher = zmq.SendSocket(string.format("tcp://*:%i", port_nb))
  -- announce publisher
  instance.registration = mdns.Registration(service_type, name, port_nb)

  setmetatable(instance, lib)
  return instance
end})


function lib:connect(device_name)
  self.browser:connect_service(self, device_name)
end
