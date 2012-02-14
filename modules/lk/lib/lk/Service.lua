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
  local info = opts.info or {}
  info.type = type_name
  local self = {name = name, callback = callback, info = (opts.info or {})}

  --======================================= PUB server
  self.pub = zmq.Pub()
  self.info.pub = self.pub:port()

  function self.do_callback(...)
    local url = ...
    if url == lubyk.info_url then
      -- get other ports from service
      return self.info
    elseif url == lubyk.quit_url then
      -- quit
      if opts.quit then
        return opts.quit()
      else
        return sched:quit()
      end
    else
      -- handle requests here. Maybe we need two different callbacks
      -- for reply and pull...
      return self.callback(...)
    end
  end

  --======================================= REP server (sync)
  -- FIXME: REMOVE (risk of hanging)
  self.rep = zmq.SimpleRep(function(...)
    return self.do_callback(...)
  end)

  --======================================= PULL server (async)
  self.pull = zmq.SimplePull(function(...)
    -- handle requests here
    self.do_callback(...)
  end)

  self.info.pull = self.pull:port()

  --======================================= announce REP server
  self.registration = mdns.Registration(service_type, name, self.rep:port(), self.info, opts.registration_callback)
  -- TODO: act on name change ....

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
  self.registration:kill()
  self.registration = nil
  self.rep:kill(...)
  self.pull:kill(...)
end

function lib:quit(...)
  self.rep:quit(...)
  self.pull:quit(...)
end

