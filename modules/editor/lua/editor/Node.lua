--[[------------------------------------------------------

  editor.Node
  -----------

  The Node contains all the information to communicate with
  the remote and also contains a NodeView.

--]]------------------------------------------------------

local lib   = {type='editor.Node'}
lib.__index = lib
editor.Node = lib

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
    inlets         = {},
    sorted_inlets  = {},
    outlets        = {},
    sorted_outlets = {},
    process        = process,
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
  self.code = code
  self.need_code_write = true
end

--- Called when we receive a change notification from the
-- remote. To actually change the remote Node, use "change".
function lib:set(def)
  for k, v in pairs(def) do
    if k == 'code' then
      setCode(self, v)
    elseif k == 'hue' or
           k == 'inlets' or
           k == 'outlets' then
      -- skip
    else
      self[k] = v
    end
  end

  self:setHue(def.hue or self.hue)

  if def.inlets then
    self:setInlets(def.inlets)
  end

  if def.outlets then
    self:setOutlets(def.outlets)
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
function lib:setInlets(list)
  local sorted_inlets = self.sorted_inlets
  local inlets        = self.inlets

  for _, def in ipairs(list) do
    local name = def.name
    if inlets[name] then
      -- update ?
      inlets[name]:set(def)
    else
      local inlet = editor.Inlet(self, name, def)
      table.insert(sorted_inlets, inlet)
      inlets[name] = inlet
    end
  end
end

--- Create outlets from a list of defined slots.
function lib:setOutlets(list)
  local sorted_outlets = self.sorted_outlets
  local outlets        = self.outlets

  for _, def in ipairs(list) do
    local name = def.name
    if outlets[name] then
      -- update ?
      outlets[name]:set(def)
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
  if code ~= self.code then
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