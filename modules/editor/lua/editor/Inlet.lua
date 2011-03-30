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
 __call = function(table, node_or_list, name, definition)
  local instance, node, pending_inlets
  if definition then
    node = node_or_list
    pending_inlets = node.pending_inlets
  else
    pending_inlets = node_or_list
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

-- Create or update view.
function lib:updateView()
  if not self.view then
    self.view = editor.SlotView(self)
    self.node.view:addWidget(self.view)
  else
    self:updateLinkViews()
  end
end

-- Called when slot moved
function lib:updateLinkViews()
  for _,link in ipairs(self.links) do
    link:updateView()
  end
end

function lib:set(def)
end