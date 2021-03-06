--[[------------------------------------------------------

  lk.Outlet
  ---------

  Outgoing node slot.

--]]------------------------------------------------------
-- 'inlet' accessor metamethod
local lib   = {type = 'lk.Outlet'}
lib.__index = lib
lk.Outlet   = lib

local doSend
setmetatable(lib, {
  -- lk.Outlet(node)
  -- Create a new outlet and insert it into
  -- node.
 __call = function(lib, node, name, opts)
  local self = {
    name = name,
    node = node,
    opts = opts or {},
    -- Connection information
    links = {},
    -- Connection inlets
    inlets = {},
  }

  function self.send(...)
    doSend(self, ...)
  end

  return setmetatable(self, lib)
end})

function doSend(self, ...)
  for _,inlet in ipairs(self.inlets) do
    inlet.receive(...)
  end
end

function lib:set(opts)
  self.opts = opts
end

local default_receive = lk.Inlet.receive
-- TODO: performance compile new 'instance.send' method by
-- generating lua code and evaluating the code with upvalues.
function lib:connect(inlet)
  for _, inl in ipairs(self.inlets) do
    if inl == inlet then
      return
    end
  end
  local conn = {type = 'Basic'}
  self.links[inlet:url()] = conn
  table.insert(self.inlets, inlet)

  local msg = self.opts.connect
  if msg then
    local receive = inlet.receive
    if receive ~= default_receive then
      if type(msg) == 'function' then
        msg = msg()
      end
      receive(msg, conn)
    end
  end
end

function lib:disconnect(target_url)
  for i, inlet in ipairs(self.inlets) do
    if inlet:url() == target_url then
      table.remove(self.inlets, i)
      local link_def = self.links[target_url]
      if link_def then
        local msg = self.opts.disconnect
        if msg ~= nil then
          local receive = inlet.receive
          if receive ~= default_receive then
            if type(msg) == 'function' then
              msg = msg()
            end

            receive(msg)
          end
        end
        self.links[target_url] = nil
      end
    end
  end
end

-- This is called on object removal.
function lib:disconnectAll()
  for target_url, _ in pairs(self.links) do
    -- It is OK to set self.links[target_url] to nil during this
    -- traversal.
    self:disconnect(target_url)
  end
end

local function dumpLinks(self, changed_link)
  local links
  if changed_link then
    -- return a copy
    links = {}
    for k, v in pairs(self.links) do
      links[k] = v
    end

    for k, v in pairs(changed_link) do
      if v == false then
        links[k] = false
      end
    end
  else
    links = self.links
  end
  return links
end

function lib:dump(links)
  local res = {
    name = self.name,
    info = self.info,
    links = dumpLinks(self, links and links[self.name]),
  }
  return res
end
