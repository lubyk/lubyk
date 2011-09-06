--[[------------------------------------------------------

  lk.ProcessWatch
  ---------------

  Watches for other lk.Process and creates new
  lk.RemoteProcess when one is found. This is used to
  handle inter process messages. The delegate should
  respond to:

    * processConnected(name)
    * processDisconnected(name)

--]]------------------------------------------------------

local lib = {}
lib.__index     = lib
lk.ProcessWatch = lib

setmetatable(lib, {
  -- new method
 __call = function(lib)
  local service_type = Lubyk.service_type
  local self = {
    browser   = lk.ServiceBrowser(service_type),
    processes = {},
    -- weak table
    delegates = setmetatable({}, {__mode = 'v'}),
  }
  setmetatable(self, lib)

  --======================================= SUB client
  self.sub = zmq.SimpleSub(function(...)
    -- receive message from local ServiceBrowser
    local url, service_name = ...
    local zone, name = string.match(service_name, '^([^:]+):(.*)$')
    if not zone then
      print('Error in ProcessWatch: found service without zone', service_name)
      return
    elseif zone ~= Lubyk.zone then
      -- ignore
      return
    end

    if url == lubyk.add_service_url then

      local process = self:process(name)
      if process.online then
        -- allready found
        return
      else
        local service = self.browser.services[service_name]
        service.zone = zone
        service.name = name

        process:connect(service)
        for _, delegate in ipairs(self.delegates) do
          delegate:processConnected(process)
        end
      end
    elseif url == lubyk.rem_service_url then
      local process = self.processes[name]
      if process then
        -- disconnect
        process:disconnect()
        for _, delegate in ipairs(self.delegates) do
          delegate:processDisconnected(process)
        end
      end
    end
  end)

  -- so we can get connection commands from the service browser
  self.sub:connect(string.format('inproc://%s', service_type))

  return self
end})

function lib:addDelegate(delegate)
  table.insert(self.delegates, delegate)
  for _, process in pairs(self.processes) do
    delegate:processConnected(process)
  end
  return self
end

--- Return the remote process or create a disconnected process.
function lib:process(name)
  local process = self.processes[name]
  if not process then
    process = lk.RemoteProcess(name)
    self.processes[name] = process
  end
  return process
end

