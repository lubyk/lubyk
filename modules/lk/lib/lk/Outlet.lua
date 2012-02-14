--[[------------------------------------------------------

  lk.Outlet
  ---------

  Outgoing node slot.

--]]------------------------------------------------------
-- 'inlet' accessor metamethod
local lib   = {type = 'lk.Outlet'}
lib.__index = lib
lk.Outlet   = lib

setmetatable(lib, {
  -- lk.Outlet(node)
  -- Create a new outlet and insert it into
  -- node.
 __call = function(lib, name, node)
  local self = {
    name = name,
    node = node,
    connections = {},
  }

  function self.send(...)
    self.send(self, ...)
  end

  return setmetatable(self, lib)
end})

function lib:send(...)
  for _,slot in ipairs(self.connections) do
    slot.receive(...)
  end
end

function lib:set(opts)
  -- noop for the moment. We could change the type of zmq
  -- operation (work distribution).
end

-- TODO: performance compile new 'instance.send' method by
-- generating lua code and evaluating the code with upvalues.
function lib:connect(inlet)
  for _,slot in ipairs(self.connections) do
    if slot == inlet then
      return
    end
  end
  table.insert(self.connections, inlet)
end

function lib:disconnect(target_url)
  for i, slot in ipairs(self.connections) do
    if slot:url() == target_url then
      table.remove(self.connections, i)
    end
  end
end

local function dumpLinks(self)
  local res = {}
  for _,slot in ipairs(self.connections) do
    res[slot:url()] = true
  end
  return res
end

function lib:dump(links)
  local res = {
    name = self.name,
    info = self.info,
    links = dumpLinks(self)
  }
  if links and links[self.name] then
    for k, v in pairs(links[self.name]) do
      if not res.links[k] then
        -- notify removed link
        res.links[k] = false
      end
    end
  end
  return res
end
