--[[------------------------------------------------------

  lk.Service
  ----------

  The Service is a zmq.REP (reply) servier announced with
  mDNS and a zmq.PUB whose port can be queried with the
  lubyk.sub_port_url query.

--]]------------------------------------------------------

local lib = {type='lk.Service'}
lib.__index = lib
lk.Service  = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, name, opts)
  if type(opts) == 'function' then
    opts = {callback = opts}
  elseif not opts then
    opts = {}
  end
  local service_type, callback, type_name = opts.service_type, opts.callback, opts.type
  type_name = type_name or lib.type
  service_type = service_type or lubyk.service_type
  if not callback then
    -- dummy
    callback = function()
      return nil
    end
  end
  local self = {name = name, callback = callback, info = {type = type_name}}

  --======================================= PUB server
  self.pub = zmq.Pub()

  --======================================= REP server (sync)
  self.rep = zmq.SimpleRep(function(...)
    print(name, "RECEIVED REQUEST", ...)
    if ... == lubyk.info_url then
      -- get other ports from service
      if not self.info.pub then
        -- publish and pull ports
        self.info.pub  = self.pub:port()
        self.info.pull = self.pull:port()
      end
      return self.info
    else
      -- handle requests here. Maybe we need two different callbacks
      -- for reply and pull...
      return self.callback(...)
    end
  end)

  --======================================= PULL server (async)
  self.pull = zmq.SimplePull(function(...)
    -- handle requests here
    self.callback(...)
  end)

  --======================================= announce REP server
  print('Service', service_type, name, self.pull:port())
  self.registration = mdns.Registration(service_type, name, self.rep:port())

  setmetatable(self, lib)
  return self
end})

function lib:notify(...)
  self.pub:send(...)
end

function lib:join(...)
  self.rep:join(...)
  self.pull:join(...)
end

function lib:kill(...)
  self.registration:__gc()
  self.registration = nil
  self.rep:kill(...)
  self.pull:kill(...)
end

function lib:quit(...)
  self.rep:quit(...)
  self.pull:quit(...)
end

