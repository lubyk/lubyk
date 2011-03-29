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
 __call = function(table, node, name, definition)
  if not name then
    node, name = nil, node
  end

  local instance = {
    node  = node,
    name  = name,
  }
  setmetatable(instance, lib)

  if node and node.process.view then
    instance:updateView()
  end

  return instance
end})

-- Create or update view.
function lib:updateView()
  if not self.view then
    self.view = editor.SlotView(self)
  end
end
