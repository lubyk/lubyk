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
-- Create a single link from an absolute target url of the form:
--   /process/parent_node/node/in/slot
-- or
--   node/in/slot
-- In the latter form, the "node" is searched in the same parent
-- as the outlet's node.
local function createLink(self, target_url)
  local process = self.node.process
  local target, err  = process:get(target_url, editor.Inlet)

  if target == false then
    error(err)
  elseif not target then
    local process_name, sub_url = string.match(target_url, '^/([^/]+)/(.+)$')
    if process_name then
      -- Resolve process
      process = process:findProcess(process_name)
      target, err = process:pendingInlet(sub_url)
    else
      target, err = process:pendingInlet(target_url)
    end
    if not target then
      error(err)
    end
  end
  -- automatically registers in self.links and self.links_by_target
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
    links_by_target = {},
  }
  setmetatable(instance, lib)

  instance:set(def)

  if node.view then
    instance:updateView()
  end
  return instance
end})

function lib:set(def)
  local links_by_target = self.links_by_target
  if def.links then
    for target_url, link_def in pairs(def.links) do
      local link = links_by_target[target_url]
      if link then
        -- update/remove
        if not link_def then
          link:delete()
        else
          -- FIXME: update link
        end
      elseif link_def then
        -- create
        createLink(self, target_url)
      end
    end
  end
end

-- Used when moving a node from one process to another.
local function dumpLinks(self)
  local links_by_target = self.links_by_target
  local list = {}
  local purl = self.node.process:url()
  for target_url, link_def in pairs(links_by_target) do
    list[lk.absolutizePath(target_url, purl)] = true
  end
  return list
end

-- Used when moving a node from one process to another.
function lib:dump()
  return dumpLinks(self)
end

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

-- Delete all links and views related to this slot.
function lib:deleteViews()
  for _,link in ipairs(self.links) do
    link:deleteView()
  end

  self.view = nil
  self.ghost = nil
end

-- The process is going offline, we need to transform connected
-- inlets from other processes to pending_inlets.
function lib:disconnectProcess(process)
  local target_urls = {}
  for i, link in ipairs(self.links) do
    if link.target_url:match('^/' .. process.name) then
      -- becomes a pending link (changing an existing key while traversing table is ok).
      table.insert(target_urls, link.target_url)
      self.links[i] = nil
    end
  end
  for _, target_url in ipairs(target_urls) do
    -- this creates a pending link
    createLink(self, target_url)
  end
end

function lib:url()
  local url = self.node:url() .. '/out/' .. self.name
  return url
end
