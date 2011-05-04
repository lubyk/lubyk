--[[------------------------------------------------------

  lk.Inlet
  --------

  Incoming node slot.

--]]------------------------------------------------------
local lib   = {type='lk.Inlet'}
lib.__index = lib
lk.Inlet    = lib

---------------------------------------------------------- PRIVATE

---------------------------------------------------------- PUBLIC
setmetatable(lib, {
  -- lk.Inlet(node)
  -- Create a new inlet
 __call = function(lib, node, name, ...)
  local instance
  if type(node) == 'string' then
    instance = {name = node}
    -- pending inlet
    setmetatable(instance, lib)
  else
    instance = node.inlets[name]
    if not instance then
      instance = node.pending_inlets[name]
      if instance then
        node.pending_inlets[name] = nil
      else
        instance = {name = name}
        setmetatable(instance, lib)
      end
      -- set node
      instance.node = node
      node.inlets[name] = instance
      table.insert(node.sorted_inlets, instance)
    end
    instance:set(name, ...)
  end
  if not instance.receive then
    -- temporary dummy method
    function instance.receive(...)
      print(string.format("'function inlet.%s' not defined.", instance.name))
    end
  end
  return instance
end})

function lib:set(name, info)
  self.name = name
  self.info = info
end

function lib.receive(...)
  print('Inlet function not set.')
end

function lib:dump()
  return {
    name = self.name,
    info = self.info
  }
end

function lib:url()
  return self.node.name .. '/in/' .. self.name
end