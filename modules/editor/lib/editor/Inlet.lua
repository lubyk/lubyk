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
  -- started drag operation (view became ghost), copy
  -- slot into non-ghost NodeView.
  self.ghost = self.view
  self.ghost.is_ghost = true
  self.view  = editor.SlotView(self)
  self.node.view:addWidget(self.view)
  -- duplicate links
  self.ghost_links  = self.links
  self.links  = {}
  for _, link in ipairs(self.ghost_links) do
    -- current link becomes a ghost (position set in ghost NodeView)
    link.is_ghost = true
    -- automatically registers in self.links
    editor.Link(link.source, self, link.target_url)
  end
end

local function removeGhost(self)
  self.ghost = nil
  -- remove ghost links
  for _,link in ipairs(self.ghost_links) do
    link:delete()
  end
  self.ghost_links = nil
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
    removeGhost(self)
  end
  self:updateLinkViews()
end

-- Called when slot moved
function lib:updateLinkViews()
  for _,link in ipairs(self.links) do
    link:updateView()
  end
  if self.ghost_links then
    for _,link in ipairs(self.ghost_links) do
      link:updateView()
    end
  end
end


function lib:set(def)
end

-- Delete all links and views related to this slot.
function lib:deleteViews()
  for _,link in ipairs(self.links) do
    link:deleteView()
  end

  if self.ghost_links then
    for _,link in ipairs(self.ghost_links) do
      link:deleteView()
    end
  end

  if self.view then
    self.view = nil
  end
end

function lib:url()
  local url = self.node:url() .. '/in/' .. self.name
  return url
end