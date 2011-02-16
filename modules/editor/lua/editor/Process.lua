--[[------------------------------------------------------

  editor.Process
  --------------

  The editor.Process manages the connection with the
  remote Process. This is like lk.Client but with a single
  subscription.

--]]------------------------------------------------------

local lib      = {}
lib.__index    = lib
editor.Process = lib

setmetatable(lib, {
  -- new method
 __call = function(table, remote_service)
  local instance = {name = remote_service.name, push = remote_service.push}
  if remote_service.info.hue then
    instance.hue = remote_service.info.hue
  else
    instance.hue = 0.5
  end
  -- the little view on the side
  instance.tab = editor.ProcessTab(instance)

  --======================================= SUB client
  instance.sub = zmq.SimpleSub(function(...)
    -- we receive notifications
    -- ...
  end)
  instance.sub:connect(remote_service.sub_url)

  setmetatable(instance, lib)
  return instance
end})

function lib:send(...)
  self.push:send(...)
end
