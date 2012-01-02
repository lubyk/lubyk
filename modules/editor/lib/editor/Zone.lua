--[[------------------------------------------------------

  editor.Zone
  -----------

  Enables zone editing. This is the main document view: it
  holds editor state information (such as
  current selection, etc).

--]]------------------------------------------------------
local lib    = {type='editor.Zone'}
lib.__index  = lib
editor.Zone  = lib
local private= {}

--=============================================== PUBLIC
setmetatable(lib, {
  -- new method
 __call = function(lib, process_watch, name)
  local self = {
    name                = name,
    selected_node_views = {},
    -- files edited in external editor
    -- OBSOLETE since WebDAV: remove
    observed_files      = {},
    file_observer       = mimas.FileObserver(),
    -- Manage processes found
    pending_processes   = {},
    found_processes     = {},
  }
  setmetatable(self, lib)

  private.setupLibrary(self)

  private.setupProcessWatch(self, process_watch or lk.ProcessWatch())

  private.setupView(self)

  function self.file_observer.pathChanged(obs, path)
    self:pathChanged(path)
  end
  return self
end})

function lib:selectNodeView(node_view, add_to_selection)
  if not add_to_selection then
    for _, view in pairs(self.selected_node_views) do
      view.selected = false
      view:update()
    end
    self.selected_node_views = {}
  end
  if node_view then
    self.selected_node_views[node_view] = node_view
    node_view.selected = true
    node_view:update()
  end
end

function lib:deselectNodeView(node_view)
  self.selected_node_views[node_view] = nil
end

function lib:toggleView(process)
  if not process.view then
    process.view = editor.ProcessView(process)
    self.main_view:addProcessView(process.view)
  else
    process:deleteView()
  end
end

local work_path = '/Volumes/Lubyk'
                  --(Lubyk.editor or {}).work_path or
                  --string.format('%s/tmp_lubyk/editor/tmp', os.getenv('HOME'))

lk.makePath(work_path)

function lib:workPath()
  return work_path
end

local editor_cmd = (Lubyk.editor or {}).editor_cmd

function lib:editFile(filepath, node)
  if editor_cmd then
    local cmd = string.format("%s '%s'", editor_cmd, filepath)
    worker:execute(cmd)
  else
    -- FIXME
    -- use internal editor ?
    worker:execute(string.format("open '%s'", filepath))
  end
end

function lib:pathChanged(path)
  local node = self.observed_files[path]
  if node then
    node:fileChanged(path)
  end
end

--- User clicked on a link: select it.
function lib:selectLinkView(link_view)
  local old_selection = self.selected_link_view
  -- select link
  self.selected_link_view = link_view
  if old_selection then
    old_selection:update()
  end

  if link_view then
    link_view:update()
  end
end

-- Find a process from global position gx, gy.
function lib:processViewAtGlobal(gx, gy)
  for _, process in pairs(self.found_processes) do
    local view = process.view
    if view then
      local vx, vy = view:globalPosition()
      if gx > vx and gx < vx + view.width and
         gy > vy and gy < vy + view.height then
        return view
      end
    end
  end
  return nil
end

--- Find the closest slotView in all processes from the global
-- x and global y coordinates.
-- @return closest slot and distance to center of slot.
function lib:closestSlotView(gx, gy, for_type, skip_node)
  local best_slot, best_dist
  for _, process in pairs(self.found_processes) do
    local view = process.view
    if view then
      local slot, dist = view:closestSlotView(gx, gy, for_type, skip_node)
      if slot and (not best_slot or dist < best_dist) then
        best_dist = dist
        best_slot = slot
      end
    end
  end
  return best_slot, best_dist
end

--=============================================== editor.Process delegate
function lib:findProcess(process_name)
  local process = self.found_processes[process_name] or
                  self.pending_processes[process_name]
  if not process then
    -- pending
    process = editor.Process(process_name)
    self.pending_processes[process_name] = process
  end
  return process
end

--=============================================== lk.ProcessWatch delegate
function lib:processConnected(remote_process)
  local name = remote_process.name
  if name == '' then
    -- found morph server
    -- TODO: create editor.Morph to proxy calls to morph server
    self.morph = remote_process
    -- TODO: create editor.MorphView to show morph server
    -- mount morph DAV server
    self.morph.dav_url = string.format('http://%s:8103', remote_process.ip)
    -- We could use option -S == do not prompt when server goes offline
    local cmd = string.format('mount_webdav -S %s %s', self.morph.dav_url, self:workPath())
    self.mount_fd = worker:execute(cmd)
  elseif string.match(name, '^@(.+)$') then
    -- machine (stem cell)
    if self.machine_list_view then
      self.machine_list_view:setStem(remote_process.ip, remote_process)
    end
  else
    -- process
    local process = self.pending_processes[name]

    if process then
      -- pending process will be connected
      self.pending_processes[name] = nil
    else
      -- create new
      process = editor.Process(name)
    end
    -- We do this before connecting so that node creation
    -- does not create a pending process.
    self.found_processes[process.name] = process
    process:connect(remote_process, self)

    --- Update views
    if self.machine_list_view then
      self.machine_list_view:addProcess(process, remote_process)
    end
    self:toggleView(process)
  end
end

function lib:processDisconnected(remote_process)
  local name = remote_process.name
  if name == '' then
    -- morph server going offline
    -- TODO: disconnect morph
    self.morph = nil
    self.morph_view = nil
  elseif string.match(name, '^@(.+)$') then
    -- machine (stem cell)
    if self.machine_list_view then
      self.machine_list_view:setStem(remote_process.ip, nil)
    end
  else
    -- process
    local process = self.found_processes[name]
    if process then
      process:disconnect()
      --- Update views
      if process.name then
        -- not morph
        if self.machine_list_view then
          self.machine_list_view:removeProcess(remote_process)
          process:deleteView()
        end
        self.found_processes[name] = nil
        -- this could run anywhere but it has to run after the process is removed
        -- from process_list
        for _, p in pairs(self.found_processes) do
          -- transform outgoing links to the dying process to pending links.
          p:disconnectProcess(process)
        end
      end
    end
  end
end

function lib:startStemCell()
  worker:spawn([[
  require 'lubyk'
  stem = lk.StemCell()
  run()
  ]])
end

--=============================================== PRIVATE
function private.setupLibrary(self)
  -- delegate in Library is used by LibraryView for drag&drop operations.
  self.library = editor.Library(nil, self)
  -- Update library
  self.library:sync()
end

function private.setupProcessWatch(self, process_watch)
  -- Start listening for processes and zones on the network
  self.process_watch = process_watch:addDelegate(self)
end

function private.setupView(self)
  local view = editor.ZoneView(self)
  self.main_view = view
  function view:closed()
    -- when window is closed, we remove this zone
    app:removeZone(self.name)
  end
  self.machine_list_view = view.machine_list_view
  view:show()
end

