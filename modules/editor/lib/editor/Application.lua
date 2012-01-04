--[[------------------------------------------------------
--

  editor.Application
  ------------------

  Singleton to start splash screen and open editor
  windows (editor.Zone). This class has no public methods.

--]]------------------------------------------------------
require 'mimas'
require 'mimas.Application'
local lib          = lk.SubClass(mimas, 'Application')
lib.type           = 'editor.Application'
lib.__index        = lib
editor.Application = lib
local app          = app
local private      = {}

-- We transform mimas.Application into an editor.Application.
function lib.new()
  local self = app
  setmetatable(self, lib)
  self:init()
  return self
end

function lib:init()
  -- editing windows' models
  self.zones     = {}
  -- list of found zones
  self.zone_list = {}
  -- hosts
  self.host_list = {'localhost'}

  self:selectZone(Lubyk.zone)
  
  if false then
    -- splash screen, etc: later
    -- Start listening for processes and zones on the network
    self.process_watch = lk.ProcessWatch():addDelegate(self)

    -- Data source for zones
    self.zone_data = mimas.DataSource(self.zone_list)

    -- Data source for hosts
    self.host_data = mimas.DataSource(self.host_list)

    self.splash_view = editor.SplashScreen(self)
    self.splash_view:show()
  end
end

--=============================================== ProcessWatch delegate
function lib:processConnected(service)
  local service_name = process.name
  if service_name == '' then
    -- morph
    -- found new zone
    self.morph = service
  else
    -- process
    -- we are not interested in processes in Main
  end
end

function lib:removeService(service)
  local service_name = service.name
  if service_name == '' then
    -- morph
    self.morph = nil
    private.disconnectMorph(self, process)
  else
    -- process
    -- we are not interested in processes in Main
  end
end

--============================================= ZoneChooser delegate

function lib:zonesDataSource()
  return self.zone_data
end

function lib:hostsDataSource()
  return self.host_data
end

function lib:selectZone(zone_name)
  if not self.zones[zone_name] then
    local zone = editor.Zone(self.process_watch, zone_name)
    self.zones[zone_name] = zone
    if self.splash_view then
      self.splash_view:close()
      self.splash_view = nil
    end
  end
end

function lib:removeZone(zone_name)
  -- Window was closed.
  self.zones[zone_name] = nil
end

function lib:startZone(opts)
  -- Spawn Morph server
  worker:spawn([[
  require 'lubyk'
  morph = lk.DavMorph(%s)
  run()
  ]], opts)

  -- Maybe we should make sure it started ok before selecting the zone.
  self:selectZone(Lubyk.zone)
end

--=============================================== Receive an openFile event

function lib:openFile(path)
  -- We receive spurious openFile events for the arguments passed to 
  -- Resources/app.lua. Check for correct file type.
  if string.match(path, '.lkp$') then
    if self.morph then
      -- kill it
      self.morph.push:send(lubyk.quit_url)
      self.morph     = nil
    end
    self:startZone {path = path, start_stem = true}
  end
end


