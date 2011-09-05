--[[------------------------------------------------------

  editor.Application
  ------------------

  Singleton to start splash screen and open editor
  windows (editor.Zone). This class has no public methods.

--]]------------------------------------------------------

local lib          = lk.SubClass(mimas, 'Application')
lib.type           = 'editor.Application'
lib.__index        = lib
editor.Application = lib

local function testApp(self)
  self.win = mimas.MainWindow()
  self.win:testMenus(true)
  --[[
  win.menu_bar = mimas.MenuBar(win)
  win.menu_bar:addMenu('File')
  win.menu_bar.File:addAction('Quit', 'Ctrl+Q', function()
    app:quit()
  end)
  --]]

  function self.win:paint(p, w, h)
    local steps=5
    for i=1,steps do
      p:fillRect((i-1)*w/steps, 0, w/steps, h, mimas.Color(i/steps))
    end
  end
  self.win:show()
end  

function lib:init()
  -- editing windows' models
  self.zones     = {}
  -- list of found zones
  self.zone_list = {}
  -- found morph by zone name
  self.morphs     = {}
  -- hosts
  self.host_list = {'localhost'}

  if true then
    testApp(self)
    return
  end

  -- Start listening for processes and zones on the network
  self.process_watch = lk.ProcessWatch():addDelegate(self)

  -- Data source for zones
  self.zone_data = mimas.DataSource(self.zone_list)

  -- Data source for hosts
  self.host_data = mimas.DataSource(self.host_list)

  self.splash_view = editor.SplashScreen(self)
  self.splash_view:show()
end

--=============================================== ProcessWatch delegate
local function addZone(self, remote_service)
  table.insert(self.zone_list, remote_service.zone)
  -- TODO: use editor.Morph and share with editor.Zone ?
  self.morphs[remote_service.zone] = remote_service
  -- update zone list
  self.zone_data:reset()
end

local function removeZone(self, remote_service)
  self.morphs[remote_service.zone] = nil
  -- update zone list
  for i, zone in ipairs(self.zone_list) do
    if zone == remote_service.zone then
      table.remove(self.zone_list, i)
      break
    end
  end
  self.zone_data:reset()
end

function lib:addService(remote_service)
  local service_name = remote_service.name
  if service_name == '' then
    -- morph
    -- found new zone
    addZone(self, remote_service)
  else
    -- process
    -- we are not interested in processes in Main
  end
end

function lib:removeService(remote_service)
  local service_name = remote_service.name
  if service_name == '' then
    -- morph
    removeZone(self, remote_service)
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
  local zone = editor.Zone(zone_name, self.process_watch)
  self.zones[zone_name] = zone
  self.splash_view:close()
  self.splash_view = nil
end

function lib:startZone(zone_name, host_name, path)
  -- spawn Morph server
  -- TODO: Could we replace mimas.Application by something else worker:join() or worker:run() ?
  worker:spawn([[
  require 'lubyk'
  app = mimas.Application()
  morph = lk.Morph(%s)
  app:exec()
  ]], {
    zone = zone_name,
    path = path
  })

  -- Maybe we should make sure it started ok before selecting the zone.
  self:selectZone(zone_name)
end
