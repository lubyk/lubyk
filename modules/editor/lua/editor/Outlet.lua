--[[------------------------------------------------------

  editor.Outlet
  -------------

  The editor.Outlet stores basic information on the remote
  outlet and manages links.

--]]------------------------------------------------------

local lib     = {type='editor.Outlet'}
lib.__index   = lib
editor.Outlet = lib

-- PRIVATE
-- Create a single link
local function createLink(self, target_url)
  for i,link in ipairs(self.links) do
    if link.target_url == target_url then
      return
    end
  end

  local process = self.node.process
  local target, err  = process:get(target_url, editor.Inlet)
  if target == false then
    error(err)
  elseif not target then
    target, err = process:pendingInlet(target_url)
    if not target then
      error(err)
    end
  end
  -- automatically registers in self.links
  local link = editor.Link(self, target, target_url)
  if self.view then
    link = link:updateView()
  end
end

-- PUBLIC
setmetatable(lib, {
  -- new method
 __call = function(lib, node, name, def)
  local instance = {
    node  = node,
    name  = name,
    -- array contains all links including ghost links
    -- dictionary contains keys of created targets
    links = {},
  }
  setmetatable(instance, lib)

  instance:set(def)

  if node.view then
    instance:updateView()
  end
  return instance
end})

function lib:set(def)
  if def.links then
    for _, target_url in ipairs(def.links) do
      createLink(self, target_url)
    end
  end
end

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
    -- current link becomes a ghost
    link.is_ghost = true
    -- automatically registers in self.links
    editor.Link(self, link.target, link.target_url)
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
    if not self.ghost then
      makeGhost(self)
    end
  elseif self.ghost then
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
