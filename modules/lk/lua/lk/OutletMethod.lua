--[[------------------------------------------------------

  lk.OutletMethod
  ---------------

  Creates the outlet declaration method 'outlet' used in
  nodes.

--]]------------------------------------------------------
-- 'outlet' accessor metamethod
local lib = {type='lk.OutletMethod'}
lk.OutletMethod = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, node)
  -- Create outlet() method/accessor for a given node
  local self = {_node = node, _outlets = {}}
  setmetatable(self, lib)
  return self
end})

function lib:__index(key)
  return self._node.outlets[key]
end

function lib:__call(...)
  local name = ...
  -- outlet(name, info)
  -- Declare a new outlet
  local outlet = lk.Outlet(self._node, ...)
  self._node.env[name] = outlet
  return outlet
end
