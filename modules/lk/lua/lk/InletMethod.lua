--[[------------------------------------------------------

  lk.InletMethod
  --------------

  Creates the inlet declaration method 'inlet' used in
  nodes.

--]]------------------------------------------------------
-- 'inlet' accessor metamethod
local lib = {type='lk.InletMethod'}
lib.__index    = lib
lk.InletMethod = lib

setmetatable(lib, {
  -- new method
 __call = function(table, node)
  -- Create inlet() method/accessor for a given node
  local instance = {node = node}
  setmetatable(instance, lib)
  return instance
end})

function lib.__call(self, ...)
  -- inlet(name, info)
  -- Declare a new inlet
  return lk.Inlet(self.node, ...)
end

function lib.__newindex(self, name, func)
  -- inlet.foo(xxx)
  -- Set an inlet callback
  local inlet = self.node.inlets[name]
  if not inlet then
    error(string.format("Inlet '%s' not declared.", name))
  end
  inlet.receive = func
end
