--[[------------------------------------------------------

  editor.Inlet
  ------------

  The editor.Inlet stores basic information on the remote
  inlet.

--]]------------------------------------------------------

local lib     = {type='editor.Inlet'}
lib.__index   = lib
editor.Inlet  = lib

-- PUBLIC
setmetatable(lib, {
  -- new method
 __call = function(lib, node_or_pending, name, definition)
  local instance, node, pending_inlets
  if definition then
    node = node_or_pending
    pending_inlets = node.pending_inlets
  else
    pending_inlets = node_or_pending
  end

  instance = pending_inlets[name]

  if not instance then
    instance = {
      node  = node,
      name  = name,
      links = {},
    }
    setmetatable(instance, lib)
  else
    -- resolving node relation for pending inlet
    instance.node = node
  end

  if not definition then
    -- pending inlet
    pending_inlets[name] = instance
    return instance
  else
    -- real
    pending_inlets[name] = nil
  end

  instance:set(definition)

  -- only executed if real
  if node and node.view then
    instance:updateView()
  end

  return instance
end})

local function makeGhost(self)
  -- Started drag operation (view has a ghost), create ghost slots.
  self.ghost = editor.SlotView(self)
  self.ghost.is_ghost = true
  self.node.ghost:addWidget(self.ghost)
end

-- Create or update view.
function lib:updateView()
  if not self.view then
    self.view = editor.SlotView(self)
    self.node.view:addWidget(self.view)
  elseif self.node.ghost then
    -- not has a ghost but we don't, create one
    if not self.ghost then
      makeGhost(self)
    end
  elseif self.ghost then
    -- we have a ghost but node hasn't, remove
    self.ghost = nil
  end
  self:updateLinkViews()
end

-- Called when slot moved
function lib:updateLinkViews()
  for _,link in ipairs(self.links) do
    link:updateView()
  end
end


function lib:set(def)
end

-- Delete all links and views related to this slot.
function lib:deleteViews()
  for _,link in ipairs(self.links) do
    link:deleteView()
  end

  self.view = nil
  self.ghost = nil
end

function lib:url()
  local url = self.node:url() .. '/in/' .. self.name
  return url
end
