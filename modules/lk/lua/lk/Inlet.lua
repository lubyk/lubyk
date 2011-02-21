--[[------------------------------------------------------

  lk.Inlet
  --------

  Incoming node slot.

--]]------------------------------------------------------
local lib   = {}
lib.__index = lib
lk.Inlet    = lib

setmetatable(lib, {
  -- lk.Inlet(node)
  -- Create a new inlet
 __call = function(table, node, name, ...)
  local instance = node.inlets[name]
  if not instance then
    instance = {node = node}
    setmetatable(instance, lib)
    node.inlets[name] = instance
  end
  instance:set(name, ...)
  return instance
end})

function lib:set(name, info)
  self.name = name
  self.info = info
end
