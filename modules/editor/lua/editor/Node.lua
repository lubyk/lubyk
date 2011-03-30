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
 __call = function(table, process, name, def)
  if not name then
    def     = process
    process = def.process
    name    = def.name
  end

  local instance = {
    name    = name,
    x       = def.x,
    y       = def.y,
    inlets  = {},
    outlets = {},
    process = process,
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

function lib:set(def)
  self:setHue(def.hue or 0.2)

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
  else
    -- update needed views
  end
end

function lib:setHue(hue)
  self.hue      = hue
  self.color    = mimas.Color(self.hue, 0.3, 0.8, 0.8)
  self.bg_color = mimas.Color(self.hue, 0.2, 0.2)
end

--- Create inlets from a list of defined slots.
function lib:setInlets(list)
  local inlets = self.inlets

  for name, def in pairs(list) do
    if inlets[name] then
      -- update ?
    else
      inlets[name] = editor.Inlet(self, name, def)
    end
  end
end

--- Create outlets from a list of defined slots.
function lib:setOutlets(list)
  local outlets = self.outlets

  for name, def in pairs(list) do
    if outlets[name] then
      -- update ?
    else
      outlets[name] = editor.Outlet(self, name, def)
    end
  end
end

function lib:error(...)
  print(string.format(...))
--  table.insert(self.errors, string.format(...))
end

local function set_link(self, out_name, in_url, process)
  local outlet = self.outlets[out_name]
  if not outlet then
    self:error("Outlet name '%s' does not exist.", out_name)
  else
    local slot, err = process:get(in_url, lk.Inlet)
    if slot == false then
      -- error
      self:error(err)
    elseif not slot then
      -- slot not found
      -- If the slot does not exist yet, make a draft to be used
      -- when the node creates the real inlet.
      local err
      slot, err = process:pendingInlet(in_url)
      if not slot then
        self:error(err)
        return
      end
    end
    -- connect to real or pending slot
    outlet:connect(slot)
  end
end

function lib:set_links(links)
  local process = self.process
  for out_name, def in pairs(links) do
    if type(def) == 'table' then
      -- multiple links
      for _, ldef in ipairs(def) do
        set_link(self, out_name, ldef, process)
      end
    else
      set_link(self, out_name, def, process)
    end
  end
end
