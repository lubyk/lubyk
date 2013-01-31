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
 __call = function(lib, ...)
   return lib.new(...)
 end
})

-- Create a new inlet
function lib.new(node, name)
  local self
  if type(node) == 'string' then
    -- Pending inlet.
    self = setmetatable({
      name       = name,
      target_url = node,
    }, lib)
  else
    self = setmetatable({
      name = name,
    }, lib)
    self:setNode(node)
  end

  return self
end

function lib.receive(...)
  print('Inlet function not set.')
end

-- This is called when a pending inlet becomes real.
function lib:setNode(node)
  -- Pending becomes real.
  -- FIXME: receive "connect" message ?
  self.target_url = nil
  self.node = node
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
