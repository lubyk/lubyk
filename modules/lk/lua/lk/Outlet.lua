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
 __call = function(lib, node, name, ...)
  local instance = node.outlets[name]
  if not instance then
    instance = {node = node, connections = {}}
    setmetatable(instance, lib)
    node.outlets[name] = instance
    table.insert(node.sorted_outlets, instance)
    function instance.send(...)
      lib.send(instance, ...)
    end
  end
  instance:set(name, ...)
  return instance.send
end})

function lib:set(name, info)
  self.name = name
  self.info = info
end

function lib:send(...)
  for _,slot in ipairs(self.connections) do
    slot.receive(...)
  end
end

-- TODO: performance compile new 'instance.send' method
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