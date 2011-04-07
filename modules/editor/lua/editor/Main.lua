--[[------------------------------------------------------

  editor.main
  -----------

  Singleton to hold editor state information (such as
  current selection, etc).

--]]------------------------------------------------------

local lib    = {type='editor.Main'}
lib.__index  = lib
editor.Main  = lib

-- PUBLIC
setmetatable(lib, {
  -- new method
 __call = function(lib)
  local instance = {
    selected_node_views = {},
    -- files edited in external editor
    observed_files      = {},
    file_observer       = mimas.FileObserver(),
    library             = editor.Library(),
  }
  setmetatable(instance, lib)

  function instance.file_observer.pathChanged(path)
    instance:pathChanged(path)
  end
  return instance
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

--- When a new process is discovered on the network, this method
-- is called by ProcessWatch.
function lib:addProcess(process)
  if self.process_list_view then
    app:post(function()
      -- Adding widgets must be done in the GUI thread
      -- FIXME: use updateView()
      self.process_list_view:addProcess(process)
      self.main_view:placeElements()
    end)
  end
end

--- When a process goes offline, this method is called.
function lib:removeProcess(process)
  if self.process_list_view then
    app:post(function()
      self.process_list_view:removeProcess(process.name)
      process:deleteView()
      self.main_view:placeElements()
    end)
  end
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

function lib:setView(main_view)
  self.main_view = main_view
  self.process_list_view = main_view.process_list_view
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