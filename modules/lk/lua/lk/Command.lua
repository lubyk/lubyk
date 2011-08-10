--[[------------------------------------------------------

  lk.Command
  ----------

  The Command executes single commands or opens a console
  to interact with running lubyk morph servers and process.
  This is like a simplified (or scriptable) GUI.

--]]------------------------------------------------------

local lib = {type='lk.Command'}
lib.__index = lib
lk.Command  = lib

--private
local getTarget

setmetatable(lib, {
  -- new method
 __call = function(lib, url, ...)
  local zone = 'default' --lk.Setting('zone', 'default')

  local self = {
    zone  = zone,
    zones = {},
    -- private flag to find more processes
    find_more = true,
  }
  if url then
    local target, cmd = getTarget(url)
    if target ~= '*' then
      -- just find this target, execute command and quit
      self.target = target
      self.cmd = {url, ...}
    elseif cmd then
      -- star based url (run on many processes)
      self.on_load = {url, ...}
    end
  end
  setmetatable(self, lib)
  self.zones[zone] = {}
  self.current = self.zones[zone]

  self.process_watch = lk.ProcessWatch():addDelegate(self)
  while(self.find_more) do
    self.find_more = nil
    if self.abort then
      break
    else
      sleep(1000) -- let the processes resolve
    end
  end

  if self.on_load then
    self:call(unpack(self.on_load))
    self.on_load = nil
  end

  return self
end})

function getTarget(url)
  return string.match(url, '^/([^/]*)(.*)$')
end

function lib:call(url, ...)
  local target_name, cmd = getTarget(url)
  if target_name == '*' then
    for _, service in pairs(self.current) do
      print(service.req:send(cmd, ...))
    end
  elseif target_name then
    local service = self.current[target_name]
    if service then
      print(service.req:send(cmd, ...))
    else
      print(string.format("Process '%s' not found.", target_name))
    end
  else
    print(string.format("Bad url '%s' (could not find process name)."))
  end
end

--================================================== lk.ProcessWatch delegate

function lib:addService(service)
  self.find_more = true
  local zone = self.zones[service.zone]
  if not zone then
    self.zones[service.zone] = {}
    zone = {}
  end
  zone[service.name] = service
  if self.target and service.zone == self.zone and service.name == self.target then
    self:call(unpack(self.cmd))
    self.abort = true
  end
end

function lib:removeService(service)
  local zone = self.zones[service.zone]
  if zone then
    zone[service.name] = nil
  end
end

