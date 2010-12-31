--[[------------------------------------------------------

  rk.Service
  ----------

  This is a ZeroMQ based connection announced with Bonjour
  (zmq + mDNS). A Service is a two way asynchronous zmq
  publisher and subscriber.

--]]------------------------------------------------------

local lib = {}
lib.__index = lib
rk.Service  = lib

-- FIXME: this is stupid. Fix zmq.Receive to accept 0 as port number.
local port_nb = 7000
setmetatable(lib, {
  -- new method
 __call = function(table, name, service_type, callback, port)
  if not callback then
    callback = service_type
    service_type = rubyk.service_type
  end
  if not callback then
    -- dummy
    callback = function() end
  end
  local instance = {name = name, browser = rk.ServiceBrowser(service_type), callback = callback, connections = {}}

  port_nb = port_nb + 1
  instance.port = port or port_nb

  -- receives zmq packets
  instance.receiver = zmq.Subscriber(function(message)
    -- we do not pass callback directly so that we can update the function with instance.callback=..
    instance.callback(instance, message)
  end)

  -- sends zmq packets
  port_nb = port_nb + 1
  instance.publisher = zmq.Publisher(string.format("tcp://*:%i", (port or port_nb) + 1))
  -- announce publisher
  instance.registration = mdns.Registration(service_type, name, (port or port_nb) + 1)

  setmetatable(instance, lib)
  return instance
end})


function lib:connect(remote_name)
  for _, connection in ipairs(self.connections) do
    if connection.remote_name == remote_name then
      return
    end
  end
  local connection = {local_service = self, remote_name = remote_name}
  table.insert(self.connections, connection)
  self.browser:connect_service(self, connection)
end

function lib:send(message)
  self.publisher:send(message)
end

function lib:connected()
  for _, connection in ipairs(self.connections) do
    if not connection.connected then
      return false
    end
  end
  return true
end

