--[[------------------------------------------------------

  lk.Inlet
  --------

  Incoming node slot.

--]]------------------------------------------------------
local lib   = {type='lk.Inlet'}
lib.__index = lib
lk.Inlet    = lib

setmetatable(lib, {
  -- lk.Inlet(node)
  -- Create a new inlet
 __call = function(lib, name, node)
  local self
  if type(node) == 'string' then
    -- Pending inlet.
    self = {
      name       = name,
      target_url = node,
    }
  else
    self = {
      name = name,
    }
  end

  return setmetatable(self, lib)
end})

function lib.receive(...)
  print('Inlet function not set.')
end

function lib:dump()
  return {
    name = self.name,
  }
end

function lib:url()
  if self.target_url then
    -- remote or pending
    return self.target_url
  else
    -- local
    return self.node:url() .. '/in/' .. self.name
  end
end
