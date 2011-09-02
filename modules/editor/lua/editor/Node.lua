--[[------------------------------------------------------

  editor.Node
  -----------

  The Node contains all the information to communicate with
  the remote and also contains a NodeView.

--]]------------------------------------------------------

local lib   = {type='editor.Node'}
lib.__index = lib
editor.Node = lib

-- Minimal width of LineEdit to create node
local MINW = 100

setmetatable(lib, {
  --- Create a new editor.Node reflecting the content of a remote
  -- node. If the process view is not shown, do not create views. If
  -- the view exists, this method must be called in the GUI thread.
 __call = function(lib, process, name, def)
  if not name then
    def     = process
    process = def.process
    name    = def.name
  end

  local instance = {
    name           = name,
    hue            = 0.2,
    x              = 100,
    y              = 100,
    inlets         = setmetatable({}, {__mode = 'v'}),
    sorted_inlets  = {},
    outlets        = setmetatable({}, {__mode = 'v'}),
    sorted_outlets = {},
    process        = process,
    parent         = process,
    delegate       = process.delegate,
  }

  -- List of inlet prototypes (already linked) to use
  -- on inlet creation.
  if process.pending_inlets[name] then
    instance.pending_inlets = process.pending_inlets[name]
    process.pending_inlets[name] = nil
  else
    instance.pending_inlets = {}
  end
  setmetatable(instance, lib)
  instance:set(def)
  return instance
end})

local function setCode(self, code)
  if self.code ~= code then
    self.code = code
    self.need_code_write = true
  end
  self.changed_code = nil
end

--- Called when we receive a change notification from the
-- remote. To actually change the remote Node, use "change".
function lib:set(def)
  for k, v in pairs(def) do
    if k == 'code' then
      setCode(self, v)
    elseif k == 'hue' or
           k == 'inlets' or
           k == 'has_all_slots' or
           k == 'outlets' then
      -- skip
    else
      self[k] = v
    end
  end

  self:setHue(def.hue or self.hue)

  if def.inlets then
    self:setInlets(def.inlets, def.has_all_slots)
  end

  if def.outlets then
    self:setOutlets(def.outlets, def.has_all_slots)
  end

  if self.process.view then
    self:updateView()
  end
end

function lib:updateView()
  if not self.view then
    self.view = editor.NodeView(self, self.process.view)
  end

  if self.ghost and not self.dragging then
    -- value updated, remove ghost
    self.ghost:delete()
    self.ghost = nil
  end
  -- update needed views
  self.view:updateView()
end

function lib:setHue(hue)
  self.hue      = hue
  self.color    = mimas.Color(self.hue, 0.3, 0.8, 0.8)
  self.bg_color = mimas.Color(self.hue, 0.2, 0.2)
end

--- Create inlets from a list of defined slots.
function lib:setInlets(list, has_all_slots)
  local sorted_inlets = self.sorted_inlets
  local inlets        = self.inlets
  if has_all_slots then
    self.sorted_inlets = {}
    sorted_inlets = self.sorted_inlets
  end

  for _, def in ipairs(list) do
    local name = def.name
    if inlets[name] then
      -- update ?
      inlets[name]:set(def)
      if has_all_slots then
        table.insert(sorted_inlets, inlet)
      end
    else
      local inlet = editor.Inlet(self, name, def)
      table.insert(sorted_inlets, inlet)
      inlets[name] = inlet
    end
  end
end

-- The process is going offline, we need to transform connected
-- inlets from other processes to pending_inlets.
function lib:disconnectProcess(process_name)
  for _, outlet in ipairs(self.sorted_outlets) do
    outlet:disconnectProcess(process_name)
  end
end

--- Create outlets from a list of defined slots.
function lib:setOutlets(list)
  local sorted_outlets = self.sorted_outlets
  local outlets        = self.outlets
  if has_all_slots then
    self.sorted_outlets = {}
    sorted_outlets = self.sorted_outlets
  end

  for _, def in ipairs(list) do
    local name = def.name
    if outlets[name] then
      -- update ?
      outlets[name]:set(def)
      if has_all_slots then
        table.insert(sorted_outlets, outlet)
      end
    else
      local outlet = editor.Outlet(self, name, def)
      table.insert(sorted_outlets, outlet)
      outlets[name] = outlet
    end
  end
end

function lib:error(...)
  print(string.format(...))
--  table.insert(self.errors, string.format(...))
end

--- Try to update the remote end with new data.
function lib:change(definition)
  self.process:change {
    nodes = {
      [self.name] = definition
    }
  }
end

-- edit code in external editor
function lib:edit()
  local filepath = self:filepath()
  if self.need_code_write then
    -- write to file
    lk.writeall(filepath, self.code)
    self.need_code_write = false
  end
  self.delegate:editFile(filepath, self)
end

function lib:filepath()
  return self.delegate:workPath() .. '/' .. self.process.name .. '/' .. self.name .. '.lua'
end

function lib:fileChanged(path)
  local code = lk.readall(path)
  if code ~= self.code and code ~= self.changed_code then
    -- Sometimes, the os gives us 2 notifications for the same change
    -- this is to avoid sending change notifications twice (reset on notification).
    self.changed_code = code
    -- send remote update
    self:change {code = code}
  end
end

function lib:deleteView()
  self.view = nil
  for _, slot in ipairs(self.sorted_outlets) do
    slot:deleteViews()
  end

  for _, slot in ipairs(self.sorted_inlets) do
    slot:deleteViews()
  end
end

function lib:url()
  return self.parent:url() .. '/' .. self.name
end


-- ========== HELPERS

-- Create a ghost node (before it is droped or name is set)
function lib.makeGhost(node_def, delegate)
  -- mock a node for NodeView
  local node = {
    name           = node_def.name,
    x              = 0,
    y              = 0,
    sorted_inlets  = {},
    sorted_outlets = {},
    delegate       = delegate,
  }
  editor.Node.setHue(node, node_def.hue or 0.2)
  local ghost = editor.NodeView(node, delegate.main_view)
  ghost.is_ghost = true
  ghost:updateView()

  -- this function will be called when the ghost is dropped
  -- or when it appears after double-click
  function ghost:openEditor(finish_func)
    -- add a LineEdit on top of self
    self.edit = editor.NodeLineEdit(delegate.main_view, node.name, delegate.library)
    self.edit:resize(math.max(self.width, MINW), self.height)
    self.edit:selectAll()
    delegate.main_view:addWidget(self.edit)
    self.edit:globalMove(self.super:globalPosition())
    self.edit:setFocus()
    self.edit.editingFinished = function()
      -- call cleanup
      self.edit:autoFinished()
      self.edit.editingFinished = nil
      app:post(finish_func)
    end
  end
  return ghost
end
