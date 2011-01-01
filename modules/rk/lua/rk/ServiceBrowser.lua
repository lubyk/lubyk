--[[------------------------------------------------------

  rk.ServiceBrowser
  -----------------

  The ServiceBrowser is responsible for managing pending
  connections, connecting zmq pub-sub when the remote
  service is found on the network.

--]]------------------------------------------------------
local lib = {}
lib.__index = lib
rk.ServiceBrowser  = lib


local srv = {}
srv.__index = srv

local browsers = {}

--- Methods for service
function srv:request(...)
  return self.req:request(...)
end

-- Return the subscribe url for the service
function srv:sub_url()
  local url = self.cached_sub_url
  if url then
    return url
  end
  self.cached_sub_url = string.format('tcp://%s:%i', self.host, self:request(rubyk.sub_port_url))
  return self.cached_sub_url
end

--- Helper function triggered whenever a new remote service is found on the network.
-- The service is added to the list of services for the current service type.
--
-- If there is any pending connections matching the service name, create connections.
local function browser_add_remote_service(self, remote_service)
  self.services[remote_service.name] = remote_service
  remote_service.req = zmq.Req()
  remote_service.req:connect(string.format('tcp://%s:%i', remote_service.host, remote_service.port))
  setmetatable(remote_service, srv)

  local pending = self.pending[remote_service.name]
  if pending then
    for _, subscription in ipairs(pending) do
      -- service found, connect
      subscription.subscriber:connect(remote_service:sub_url())
      subscription.connected = true
    end
    self.pending[remote_service.name] = nil
  end
end

--- Creates an instance of the ServiceBrowser
-- Keep a hash of created browser by service_type
-- Always return the same browser for the given service_type
--
setmetatable(lib, {
  -- new method
 __call = function(table, service_type)
  local instance = browsers[service_type]
  if instance then
    return instance
  end
  instance = {service_type = service_type, pending = {}, services = {}}
  instance.browser = mdns.Browser(service_type, function(remote_service)
    if remote_service.add then
      browser_add_remote_service(instance, remote_service)
    end
  end)
  browsers[service_type] = instance
  setmetatable(instance, lib)
  return instance
end})

--- Connect a Subscriber to another remote service.
--
function lib:connect(remote_name, subscription)
  local remote_service = self.services[remote_name]
  if remote_service then
    -- already found service on network
    -- connect
    subscription.subscriber:connect(remote_service:sub_url())
    subscription.connected = true
  else
    local pending = self.pending[remote_name]
    if not pending then
      pending = {}
      self.pending[remote_name] = pending
    end
    table.insert(pending, subscription)
  end
end
