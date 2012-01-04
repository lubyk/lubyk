--[[------------------------------------------------------

  editor.Morph
  ------------

  Used as proxy for lk.Morph.

--]]------------------------------------------------------

local lib      = {type='editor.Morph'}
lib.__index    = lib
editor.Morph   = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, zone)
  local self = {
    name    = '',
    title   = '~',
    zone    = zone,
  }

  setmetatable(self, lib)
  return self
end})


function lib:connect(service)
  self.ip      = service.ip
  self.req     = service.req
  self.push    = service.push
  self.machine = self.zone.machine_list:getMachine(service.ip)
  -- noop
  self.online = true
  self:mountDav()
  self:sync()
end

function lib:disconnect(remote)
  -- noop
  self.online = false
end

function lib:mountDav()
  -- mount morph DAV server
  self.dav_url = string.format('http://%s:8103', self.ip)
  -- option -S == do not prompt when server goes offline
  local cmd = string.format('mount_webdav -S %s %s', self.dav_url, self.zone:workPath())
  self.mount_fd = worker:execute(cmd)
end

function lib:sync()
  -- Get patch definition
  local patch = self.req:request(lubyk.dump_url)
  local processes = patch.processes
  if processes then
    -- create disconnected processes in machine view
    for name, info in pairs(processes) do
      local host
      if type(info) == 'string' then
        host = info
      else
        host = info.host
      end

      if host == '' then
        -- local to morph
        host = self.machine.name
      end
      -- Declare
      local process = self.zone:findProcess(name, host)
      process.keep_alive = true
    end
  end
end
