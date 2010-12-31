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

  -- subscribe to zmq packets
  instance.subscriber = zmq.Subscriber(function(message)
    -- we do not pass callback directly so that we can update the function with instance.callback=..
    instance.callback(instance, message)
  end)

  -- receive zmq packets
  print("Receiver", string.format("tcp://localhost:%i", (port or port_nb) - 1))
  instance.receiver = zmq.ReceiveSocket(worker, zmq.REP, function(message)
    -- we do not pass callback directly so that we can update the function with instance.callback=..
    print("Receive...", message)
    instance.callback(instance, message)
  end)
  instance.receiver:bind(string.format("tcp://localhost:%i", (port or port_nb) - 1))

  -- used to send zmq packets
  instance.sender = zmq.Sender()

  -- sends zmq packets
  port_nb = port_nb + 1
  instance.publisher = zmq.Publisher(string.format("tcp://*:%i", port or port_nb))
  -- announce publisher
  instance.registration = mdns.Registration(service_type, name, port or port_nb)

  setmetatable(instance, lib)
  return instance
end})


function lib:connect(remote_name)
  local connection = self.connections[remote_name]
  if connection then
    return
  end
  connection = {local_service = self, remote_name = remote_name}
  self.connections[remote_name] = connection
  self.browser:connect_service(self, connection)
end

function lib:send(service_name, message)
  local service = self.browser.services[service_name]
  if not service then
    print(string.format('Cannot send to %s (service not found)', service_name))
    return
  end
  self.sender:bind(service.rcv_uri)
  print('Bound to', service.rcv_uri)
  self.sender:send(message)
  print('Sending done')
end

function lib:notify(message)
  self.publisher:send(message)
end

function lib:connected()
  for _, connection in pairs(self.connections) do
    if not connection.connected then
      return false
    end
  end
  return true
end

