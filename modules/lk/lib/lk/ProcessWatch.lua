--[[------------------------------------------------------

  lk.ProcessWatch
  ---------------

  Watches for other lk.Process and creates new
  lk.RemoteProcess when one is found. This is used to
  handle inter process messages. The delegate should
  respond to:

    * processConnected(name)
    * processDisconnected(name)

  FIXME: Merge with lk.ServiceBrowser
--]]------------------------------------------------------

local lib = {}
lib.__index     = lib
lk.ProcessWatch = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, mock_browser)
  local service_type = Lubyk.service_type
  local self = {
    processes = {},
    -- weak table
    delegates = setmetatable({}, {__mode = 'v'}),
  }
  setmetatable(self, lib)

  self.browser = lk.ServiceBrowser(service_type):addDelegate(self)
  return self
end})

function lib:addDelegate(delegate)
  table.insert(self.delegates, delegate)
  for _, process in pairs(self.processes) do
    if process.online then
      delegate:processConnected(process)
    end
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

function lib:addDevice(service)
  local zone, name = string.match(service.name, '^([^:]+):(.*)$')
  if not zone then
    print('Error in ProcessWatch: found service without zone', service.name)
    return
  elseif zone ~= Lubyk.zone then
    -- ignore
    return
  end

  if service.op == 'add' then
    local process = self:process(name)
    if process.online then
      -- allready found
      return
    else
      service.zone = zone
      service.name = name

      process:connect(service)
      for _, delegate in ipairs(self.delegates) do
        delegate:processConnected(process)
      end
    end
  else
    local process = self.processes[name]
    if process then
      -- disconnect
      process:disconnect()
      for _, delegate in ipairs(self.delegates) do
        delegate:processDisconnected(process)
      end
    end
  end
end

lib.removeDevice = lib.addDevice
