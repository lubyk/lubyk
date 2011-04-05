--[[------------------------------------------------------

  editor.ProcessWatch
  -------------------

  Watches for remote Processes and creates new Processes
  when they are found. When a process disappears: remove
  the given Process.

--]]------------------------------------------------------

local lib = {}
lib.__index         = lib
editor.ProcessWatch = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, delegate)
  local service_type = lubyk.service_type
  local instance = {
    browser = lk.ServiceBrowser(service_type),
    list = {},
    delegate = delegate,
  }

  --======================================= SUB client

  instance.sub = zmq.SimpleSub(function(...)
    -- receive message from remote server or local ServiceBrowser
    local url, service_name = ...
    if url == lubyk.add_service_url then
      -- resolve pending
      if instance.list[service_name] then
        -- allready found
        return
      end

      local remote_service = instance.browser.services[service_name]
      local process = editor.Process(remote_service, delegate)
      instance.list[service_name] = process
      instance.delegate:addProcess(process)
    elseif url == lubyk.rem_service_url then
      local process = instance.list[service_name]
      -- remove connection
      if process then
        instance.list[service_name] = nil
        instance.delegate:removeProcess(process)
      end
    else
      -- ???
    end
  end)
  -- so we can get connection commands from the service browser
  instance.sub:connect(string.format('inproc://%s', service_type))

  setmetatable(instance, lib)
  instance:updateView()
  return instance
end})

function lib:updateView()
  if not self.delegate.process_list_view then
    self.delegate.process_list_view = editor.ProcessList()
  end
end
