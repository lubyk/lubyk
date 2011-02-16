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
 __call = function(table)
  local service_type = lubyk.service_type
  local instance = {browser = lk.ServiceBrowser(service_type), list = {}, view = editor.ProcessList()}

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
      app:post(function()
        local process = editor.Process(remote_service)
        instance.list[service_name] = process
        -- Adding widgets must be done in the GUI thread
        instance.view:addProcess(process)
      end)
    elseif url == lubyk.rem_service_url then
      -- remove connection
      if instance.list[service_name] then
        instance.list[service_name] = nil
        app:post(function()
          instance.view:removeProcess(service_name)
        end)
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
