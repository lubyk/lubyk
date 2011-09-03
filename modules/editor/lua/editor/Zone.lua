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

--=============================================== PRIVATE
local function setupLibrary(self)
  -- delegate in Library is used by LibraryView for drag&drop operations.
  self.library = editor.Library(nil, self)
  -- Update library
  self.library:sync()
end

local function setupProcessWatch(self, process_watch)
  -- Start listening for processes and zones on the network
  self.process_watch = process_watch:addDelegate(self)
end

local function setupView(self)
  local view = editor.ZoneView(self)
  self.main_view = view
  self.process_list_view = view.process_list_view
  view:show()
end

--=============================================== PUBLIC
setmetatable(lib, {
  -- new method
 __call = function(lib, zone, process_watch)
  local self = {
    zone                = zone,
    selected_node_views = {},
    -- files edited in external editor
    observed_files      = {},
    file_observer       = mimas.FileObserver(),
    -- Manage processes found
    pending_processes   = {},
    found_processes     = {},
  }
  setmetatable(self, lib)

  setupLibrary(self)

  setupProcessWatch(self, process_watch or lk.ProcessWatch())

  setupView(self)

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

local work_path = _lubyk_settings.editor.work_path or
                  string.format('%s/.lubyk/editor/tmp', os.getenv('HOME'))

lk.makePath(work_path)

function lib:workPath()
  return work_path
end

local editor_cmd = _lubyk_settings.editor.editor_cmd

function lib:editFile(filepath, node)
  -- FIXME: holds node reference (make it weak ?)
  -- FIXME: when do we remove paths ?
  if not self.observed_files[filepath] then
    self.file_observer:addPath(filepath)
  end
  self.observed_files[filepath] = node
  if editor_cmd then
    os.execute(string.format("%s '%s'", editor_cmd, filepath))
  else
    -- FIXME
    -- use internal editor
    os.execute(string.format("open '%s'", filepath))
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
local function addZone(self, remote_service)
  if remote_service == self.zone then
    -- TODO: create editor.Morph to proxy calls to morph server
    self.morph = remote_service
    -- TODO: create editor.MorphView to show morph server
  end
end

local function removeZone(self, remote_service)
  if remote_service.zone == self.zone then
    -- TODO: disconnect morph
    self.morph = nil
    self.morph_view = nil
  end
end

function lib:addService(remote_service)
  local service_name = remote_service.name
  if service_name ~= '' then
    -- process
    if remote_service.zone ~= self.zone then
      -- not in our zone: ignore
      return
    end

    local process = self.pending_processes[service_name]

    if process then
      -- pending process will be connected
      self.pending_processes[service_name] = nil
    else
      -- create new
      process = editor.Process(service_name)
    end

    process:connect(remote_service, self)


    --- Update views
    if self.process_list_view then
      app:post(function()
        -- Adding widgets must be done in the GUI thread
        -- FIXME: use updateView()
        self.process_list_view:addProcess(process)
        self.main_view:placeElements()
        self.found_processes[service_name] = process
      end)
    else
      self.found_processes[service_name] = process
    end
    app:post(function()
      self:toggleView(process)
    end)
  else
    -- morph
    -- found new zone
    addZone(self, remote_service)
  end
end

function lib:removeService(remote_service)
  local service_name = remote_service.name
  if service_name ~= '' then
    -- process
    if remote_service.zone ~= self.zone then
      -- not in our zone: ignore
      return
    end

    local process = self.found_processes[service_name]
    if process then
      process:disconnect()
      --- Update views
      app:post(function()
        -- This should run in the GUI thread
        if self.process_list_view then
          self.process_list_view:removeProcess(process.name)
          process:deleteView()
        end
        self.main_view:placeElements()
        self.found_processes[service_name] = nil
        -- this could run anywhere but it has to run after the process is removed
        -- from process_list
        for _, p in pairs(self.found_processes) do
          -- transform outgoing links to the dying process to pending links.
          p:disconnectProcess(process.name)
        end
      end)
    end
  else
    -- morph
    removeZone(self, remote_service)
  end
end

