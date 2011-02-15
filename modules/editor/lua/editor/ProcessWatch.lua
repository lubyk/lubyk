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
  local instance = {browser = lk.ServiceBrowser(lubyk.service_type), list = {}, list_view = editor.ProcessList()}

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
      local process = editor.Process(remote_service)
      instance.list[service_name] = process
      instance.list_view:addTab(process.tab)
    elseif url == lubyk.rem_service_url then
      -- remove connection
      if instance.list[service_name] then
        instance.list[service_name] = nil
        instance.list_view:removeTab(service_name)
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


function lib:subscribe(remote_name)
  if self.subscriptions[remote_name] then
    -- allready subscribed, abort
    return
  end
  subscription = {subscriber = self.sub}
  self.subscriptions[remote_name] = subscription
  local service = self.browser.services[remote_name]
  if service then
    self.sub:connect(service.sub_url)
    subscription.connected = true
  end
end

function lib:request(service_name, ...)
  local service = self.browser.services[service_name]
  if not service then
    return false, string.format('Cannot request to %s (service not found)', service_name)
  end
  return service.req:request(...)
end

function lib:send(service_name, ...)
  local service = self.browser.services[service_name]
  if not service then
    return false, string.format('Cannot send to %s (service not found)', service_name)
  else
    service.push:send(...)
    return true
  end
end

function lib:connected()
  for _, connection in pairs(self.subscriptions) do
    if not connection.connected then
      return false
    end
  end
  return true
end
