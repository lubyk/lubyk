--[[------------------------------------------------------

  lk.ProcessWatch
  ---------------

  Watches for other lk.Process and creates new
  lk.RemoteProcess when one is found. This is used to
  handle inter process messages.

--]]------------------------------------------------------

local lib = {}
lib.__index     = lib
lk.ProcessWatch = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, delegate)
  local service_type = lubyk.service_type
  local instance = {
    browser = lk.ServiceBrowser(service_type),
    found_processes   = {},
    -- Dummy proxy needed to link.
    pending_processes = {},
    delegate = delegate,
  }

  --======================================= SUB client
  instance.sub = zmq.SimpleSub(function(...)
    -- receive message from local ServiceBrowser
    local url, service_name = ...
    if url == lubyk.add_service_url then
      -- resolve pending
      if instance.found_processes[service_name] then
        -- allready found
        return
      end

      local remote_service = instance.browser.services[service_name]
      local process = instance.pending_processes[service_name]

      if process then
        -- connect
        process:connect(remote_service)
        instance.pending_processes[service_name] = nil
      else
        -- create new
        process = lk.RemoteProcess(service_name, remote_service)
      end

      instance.found_processes[service_name] = process
      instance.delegate:addProcess(process)
    elseif url == lubyk.rem_service_url then
      local process = instance.found_processes[service_name]
      -- remove connection (make dummy)
      if process then
        instance.found_processes[service_name] = nil
        instance.pending_processes[service_name] = process
        process:disconnect()
        instance.delegate:removeProcess(process)
      end
    else
      -- ???
    end
  end)
  -- so we can get connection commands from the service browser
  instance.sub:connect(string.format('inproc://%s', service_type))

  setmetatable(instance, lib)
  return instance
end})

function lib:findProcess(process_name)
  local process = self.found_processes[process_name] or
                  self.pending_processes[process_name]
  if not process then
    process = lk.RemoteProcess(process_name)
    self.pending_processes[process_name] = process
  end
  return process
end
