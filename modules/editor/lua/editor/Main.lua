--[[------------------------------------------------------

  editor.Main
  -----------

  Singleton to start splash screen and open editor
  windows (editor.Zone). This class has no public methods.

--]]------------------------------------------------------

local lib   = {type='editor.Main'}
lib.__index = lib
editor.Main  = lib

-- PUBLIC
setmetatable(lib, {
  -- new method
 __call = function(lib)
  local self = {
    -- editing windows' models
    zones     = {},
    -- found zones
    zone_list = {},
    -- hosts
    host_list = {'localhost'},
  }
  setmetatable(self, lib)

  -- Start listening for processes and zones on the network
  self.process_watch = lk.ProcessWatch(self)
  -- Data source for zones
  self.zone_data = mimas.DataSource(self.zone_list)
  -- Data source for hosts
  self.host_data = mimas.DataSource(self.host_list)

  self.splash_view = editor.SplashScreen(self)
  self.splash_view:show()
  return self
end})


--=============================================== ProcessWatch delegate
local function addZone(self, remote_service)
  table.insert(self.zone_list, remote_service)
  -- update zone list
  self.zone_data:reset()
end

local function removeZone(self, remote_service)
  -- update zone list
  for i, srv in ipairs(self.zone_list) do
    if srv.zone == remote_service.zone then
      table.remove(self.zone_list, i)
      break
    end
  end
  self.zone_data:reset()
end

function lib:addService(remote_service)
  local service_name = remote_service.name
  if service_name then
    -- process
    -- we are not interested in processes in Main
  else
    -- morph
    -- found new zone
    addZone(self, remote_service)
  end
end

function lib:removeService(remote_service)
  local service_name = remote_service.name
  if service_name then
    -- process
    -- we are not interested in processes in Main
  else
    -- morph
    removeZone(self, remote_service)
  end
end

--============================================= ZoneChooser delegate

function lib:zonesDataSource()
  return self.zone_data
end

function lib:hostsDataSource()
  return self.host_data
end

function lib:selectZone(network_name)
  print('selected', network_name)
end

function lib:startZone(zone_name, host_name, path)
  local zone = editor.Zone(zone_name, self.process_watch)
  self.zones[zone_name] = zone
  zone.main_view = editor.ZoneView(zone)
  self.splash_view:close()
  self.splash_view = nil
  zone.main_view:show()
  zone.main_view:move(50, 50)
end