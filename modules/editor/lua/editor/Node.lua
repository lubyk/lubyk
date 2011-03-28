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
 __call = function(table, def, process)
  local instance = {
    name    = def.name,
    x       = def.x,
    y       = def.y,
    inlets  = def.inlets  or {},
    outlets = def.outlets or {},
    links   = {},
  }
  setmetatable(instance, lib)
  instance:setHue(def.hue or 0.2)
  instance:set_inlets(def.inlets)
  instance:set_inlets(def.outlets)
  if process.view then
    instance.process = process
    instance.view = editor.NodeView(instance, process.view)
  end
  return instance
end})

function lib:setHue(hue)
  self.hue      = hue
  self.color    = mimas.Color(self.hue, 0.3, 0.8, 0.8)
  self.bg_color = mimas.Color(self.hue, 0.2, 0.2)
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
      slot, err = process:inlet_pending(in_url)
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