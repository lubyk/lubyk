--[[------------------------------------------------------

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
local private      = {}


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
function lib:processConnected(process)
  local service_name = process.name
  if service_name == '' then
    -- morph
    -- found new zone
    -- private.connectMorph(self, process)
  else
    -- process
    -- we are not interested in processes in Main
  end
end

function lib:removeService(process)
  local service_name = process.name
  if service_name == '' then
    -- morph
    -- private.disconnectMorph(self, process)
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
  local zone = editor.Zone(self.process_watch)
  self.zones[zone_name] = zone
  if self.splash_view then
    self.splash_view:close()
    self.splash_view = nil
  end
end

function lib:startZone(path)
  -- Spawn Morph server
  self.morph_pid = worker:spawn([[
  require 'lubyk'
  print("Starting DAV Morph server...")
  morph = lk.DavMorph(%s)
  morph:join()
  ]], path)

  -- Maybe we should make sure it started ok before selecting the zone.
  self:selectZone(Lubyk.zone)
end

--=============================================== Receive an openFile event

function lib:openFile(path)
  if string.match(path, '.lkp$') then
    if self.morph_pid then
      -- kill it
      self.morph.req(lubyk.quit_url)
      worker:waitpid(self.morph_pid)
      self.morph_pid = nil
      self.morph     = nil
    end
    self:startZone(path)
  end
end


