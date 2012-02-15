--[[------------------------------------------------------

  editor.Morph
  ------------

  Used as proxy for lk.Morph.

--]]------------------------------------------------------

local lib      = {type='editor.Morph'}
lib.__index    = lib
editor.Morph   = lib

local private  = {
  -- Set GUI state from lk.Morph notifications.
  set = {},
}

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
  self.davport = service.info.davport
  self.req     = service.req
  self.push    = service.push
  -- Receive changes from morph
  self.sub = zmq.SimpleSub(function(...)
    -- we receive notifications, update content
    private.changed(self, ...)
  end)
  self.sub:connect(service.sub_url)
  self.online = true
  if self.tab then
    self.tab:setHue()
  end
  self.zone.control_tabs:addPlusView()
  self.zone.view:hideDialog()

  private.mountDav(self)
  private.sync(self)
end

function lib:disconnect()
  -- noop
  self.zone.control_tabs:removePlusView()
  self.online = false

  if self.tab then
    self.tab:setHue(self.hue)
  end

  -- Remove all views
  local views = self.zone.views
  for name, view in pairs(views) do
    view:remove()
    views[name] = nil
  end

  private.unmountDav(self)

  if self.zone.view.link_editor then
    self.zone.view.link_editor:hide()
    self.zone.view.link_editor = nil
  end

  -- Show splash
  self.zone.view:showSplash()
end

function lib:quit()
  -- Morph will quit all running processes except the stem cells.
  self.push:send(lubyk.quit_url)
end

function lib:createProcess(def)
  self.push:send(lubyk.update_url, {
    processes = { [def.name] = def },
  })
end

function lib:removeProcess(process)
  process.removed = true
  self.push:send(lubyk.update_url, {
    processes = { [process.name] = false },
  })
end

function lib:restartProcess(name)
  self.push:send(lubyk.update_url, {
    processes = { [name] = 'restart'},
  })
end

function lib:change(def)
  self.push:send(lubyk.update_url, def)
end

--=============================================== PRIVATE

function private:mountDav()
  local work_path = self.zone:workPath()
  -- mount morph DAV server
  self.dav_url = string.format('http://%s:%i', self.ip, self.davport)
  -- option -S == do not prompt when server goes offline
  local cmd = string.format('mount_webdav -S %s %s', self.dav_url, work_path)
  self.mount_fd = worker:execute(cmd)
end

function private:unmountDav()
  if self.mount_fd then
    local work_path = self.zone:workPath()
    -- Automatic disconnection. If we do not do this, we have lots
    -- of dangling server problems.
    self.mount_fd = nil
    worker:execute(string.format('umount %s', work_path))
  end
end

function private:sync()
  -- Get patch definition
  private.changed(self, self.req:request(lubyk.dump_url))
end

--=============================================== CHANGED
--- We receive change information from lk.Morph.
function private:changed(changes)
  local set = private.set
  -- start with host
  if changes.host then
    self.host = changes.host
  end
  for k, v in pairs(changes) do
    local func = set[k]
    if func then
      func(self, v)
    end
  end
end


--=============================================== SET

function private.set:name(name)
  self.fullname = name
  local view = self.zone.view
  if view then
    view:setWindowTitle(string.format('%s: %s', Lubyk.zone, name))
  end
end

function private.set:processes(data)
  local processes = self.zone.found_processes
  for name, info in pairs(data) do
    local process = processes[name]
    if info == false then
      -- removed process
      if process then
        process:remove(true)
      end
    elseif not process then
      -- create disconnected processes in machine view
      local host
      if type(info) == 'string' then
        host = info
      else
        host = info.host
      end

      if host == '' then
        -- local to morph
        host = self.host
      end
      -- Declare
      process = self.zone:findProcess(name, host)
      process.known_to_morph = true
    else
      -- Mark as known_to_morph
      process.known_to_morph = true
    end
  end
end

function private.set:_views(data)
  local views = self.zone.views
  for name, info in pairs(data) do
    local view = views[name]
    if info == false then
      -- removed view
      if view then
        view:remove()
        views[name] = nil
      end
    elseif view then
      -- update
      view:update(info)
    else
      -- create new ControlView
      views[name] = self.zone.control_tabs:addView(name, info)
    end
  end
  if self.zone.view.link_editor then
    self.zone.view.link_editor:rebuildPaths()
  end
end
