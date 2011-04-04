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
  }
  setmetatable(instance, lib)

  function instance.file_observer.pathChanged(path)
    instance:pathChanged(path)
  end
  return instance
end})

editor.main = editor.Main() -- singleton

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
    end)
  end
end

--- When a process goes offline, this method is called.
function lib:removeProcess(process)
  if self.process_list_view then
    app:post(function()
      self.process_list_view:removeProcess(process.name)
    end)
  end
end

local work_path = _lubyk_settings.editor.work_path or string.format('%s/.lk_editor', os.getenv('HOME'))

function lib:workPath()
  return work_path
end

local editor_cmd = _lubyk_settings.editor.editor_cmd

function lib:editFile(filepath, node)
  -- FIXME: holds node reference (make it weak ?)
  -- FIXME: when do we remove paths ?
  self.file_observer:addPath(filepath)
  self.observed_files[filepath] = node
  if editor_cmd then
    print(editor_cmd, filepath)
    os.execute(string.format("%s '%s'", editor_cmd, filepath))
  else
    -- use internal editor
  end
end

function lib:pathChanged(path)
  local node = self.observed_files[path]
  if node then
    node:fileChanged(path)
  end
end