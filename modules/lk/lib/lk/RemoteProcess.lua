--[[------------------------------------------------------

  lk.RemoteProcess
  ----------------

  Proxy for lk.Process used for inter process messaging.

--]]------------------------------------------------------

local lib = {type='lk.RemoteProcess'}
lib.__index      = lib
lk.RemoteProcess = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, name, remote_service)
  local instance = {
    name = name,
    inlets = {},
  }
  setmetatable(instance, lib)

  if remote_service then
    instance:connect(remote_service)
  end

  return instance
end})

function lib:connect(remote_service)
  self.ip = remote_service.ip
  self.info = remote_service.info
  self.sub_url = remote_service.sub_url
  self.push = remote_service.push
  self.req  = remote_service.req
  -- replace dummy 'send'
  self.send = function(...)
    remote_service.push:send(...)
  end
  self.online = true
end

function lib:disconnect()
  self.info = nil
  self.sub_url = nil
  self.push = nil
  self.req  = nil
  -- dummy
  self.send = lib.send
  self.online = false
end

function lib.send(url, ...)
  --print('Not connected', url)
  -- not connected
  -- This method is replaced on connection
end


-- Return a dummy inlet
function lib:findByPath(path)
  local inlet = self.inlets[path]
  if not inlet then
    inlet = {target_url = '/' .. self.name .. '/' .. path}
    -- just to fool lk.Outlet in thinking this is an lk.Inlet
    -- FIXME: remove metatable test in Patch:get() ?
    -- FIXME: just check for a 'receive' function ?
    setmetatable(inlet, lk.Inlet)
    function inlet.receive(...)
      self.send(path, ...)
    end
    self.inlets[path] = inlet
  end
  return inlet
end
