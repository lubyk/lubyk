--[[------------------------------------------------------

  lk.Node
  -------

  Encapsulate lua code (protect global space, enable reload,
  provide links and introspection).

--]]------------------------------------------------------
-- node metatable
local lib   = {type='lk.Node'}
lib.__index = lib
lk.Node     = lib

-- node's environment metatable
local env_mt= {}

setmetatable(lib, {
  -- new method
 __call = function(lib, process, name, code_str)
  local env  = {}
  -- new node
  local self = {
    -- inlets not in sorted_inlets are removed on gc
    inlets         = setmetatable({}, {__mode = 'v'}),
    sorted_inlets  = {},
    -- outlets not in sorted_outlets are removed on gc
    outlets        = setmetatable({}, {__mode = 'v'}),
    sorted_outlets = {},
    env            = env,
    errors         = {},
    name           = name,
    process        = process,
  }
  setmetatable(self, lib)
  -- env has read access to _G
  setmetatable(env, env_mt)
  -- method to declare inlets
  env.inlet  = lk.InletMethod(self)
  -- method to declare outlets
  env.outlet = lk.OutletMethod(self)

  process.nodes[name] = self
  -- pending connection resolution
  self.pending_inlets = process.pending_nodes[name] or {}
  process.pending_nodes[name] = nil
  if code_str then
    self:eval(code_str)
  end
  return self
end})

-- metatable for the new global env in each
-- node
function env_mt.__index(env, name)
  -- if the variable exists in original _G,
  -- cache locally
  local gvar = _G[name]
  rawset(env, name, gvar)
  return gvar
end

function lib:url()
  if self.parent then
    return self.parent:url() .. '/' .. self.name
  else
    return self.process:url() .. '/' .. self.name
  end
end

-- function to reload code
function lib:eval(code_str)
  -- hold to all current outlets
  local old_outlets = self.sorted_outlets
  local old_inlets  = self.sorted_inlets
  self.sorted_outlets = {}
  self.sorted_inlets  = {}

  local code, err = loadstring(code_str)
  if not code then
    self.sorted_outlets = old_outlets
    self.sorted_inlets  = old_inlets
    self:error(err)
    return
  end
  -- code will execute in node's environment
  setfenv(code, self.env)
  local ok, err = pcall(code)
  if not ok then
    self.sorted_outlets = old_outlets
    self.sorted_inlets  = old_inlets
    self:error(err)
  else
    self.code = code_str
    -- Unused inlets and outlets will be collected by the GC.
  end
end

function lib:set(definition)
  -- load params before evaluating script code
  local params = definition.params
  if params then
    self:setParams(params)
  end

  if definition.code and definition.code ~= self.code then
    self:eval(definition.code)
  elseif definition.class then
    local code = self.process:findClass(definition.class)

    if code then
      -- Once the prototype is resolved, we remove 'class' information.
      definition.class = nil
      definition.code  = code
      self:eval(code)
    else
      -- FIXME: set error on Node
      error(string.format("Could not find source code for '%s'.", self:url()))
    end
  elseif not self.code then
    -- Try to find code
    local code = self.process:findCode(self:url() .. '.lua' )
    if code then
      self:eval(code)
    else
      -- FIXME: set error on Node
      error(string.format("Could not find source code for '%s'.", self:url()))
    end
  end

  for k, v in pairs(definition) do
    if k == 'source' or
       k == 'params' or
       k == 'links' then
      -- ignore (we want to run these in a specific order)
    else
      self[k] = v
    end
  end

  local links = definition.links
  if links then
    self:setLinks(links)
  end
end

function lib:setParams(params)
  local env = self.env
  for k, v in pairs(params) do
    env[k] = v
  end
end

function lib:error(...)
  print(string.format(...))
  print(debug.traceback())
  print('----------------')
--  table.insert(self.errors, string.format(...))
end

function lib:makeAbsoluteUrl(url)
  if string.match(url, '^/') then
    return url
  else
    return (self.parent or self.process):url() .. '/' .. url
  end
end

local function setLink(self, out_name, target_url, process)
  local outlet = self.outlets[out_name]
  if not outlet then
    self:error("Outlet name '%s' does not exist.", out_name)
  else
    -- change relative url to absolute url
    local slot, err = process:get(self:makeAbsoluteUrl(target_url), lk.Inlet)
    if slot == false then
      -- error
      self:error(err)
    elseif not slot then
      -- slot not found
      -- If the slot does not exist yet, make a draft to be used
      -- when the node creates the real inlet.
      slot, err = process:pendingInlet(target_url)
      if not slot then
        -- FIXME: store absolute path for 'target_url' in process pending list
        -- and resolve this list on node creation
        self:error(err)
        return
      end
    end
    -- connect to real or pending slot
    outlet:connect(slot)
  end
end

local function removeLink(self, out_name, target_url)
  local outlet = self.outlets[out_name]
  if outlet then
    outlet:disconnect(target_url)
  end
end

function lib:setLinks(all_links)
  local process = self.process
  for out_name, links in pairs(all_links) do
    if type(links) ~= 'table' then
      setLink(self, out_name, links, process)
    else
      for target_url, link_def in pairs(links) do
        if link_def then
          setLink(self, out_name, target_url, process)
        else
          removeLink(self, out_name, target_url)
        end
      end
    end
  end
end

local function dumpSlots(list, links)
  local res = {}
  local has_slots = false
  for _,slot in ipairs(list) do
    has_slots = true
    table.insert(res, slot:dump(links))
  end

  if has_slots then
    return res
  else
    return nil
  end
end

function lib:dump()
  return {
    name = self.name,
    hue  = self.hue,
    x    = self.x,
    y    = self.y,
    code = self.code,
    inlets  = dumpSlots(self.sorted_inlets),
    outlets = dumpSlots(self.sorted_outlets),
  }
end

function lib:partialDump(data)
  local res = {}
  for k, v in pairs(data) do
    -- 'inlets' and 'outlets' should never be in the data
    if k ~= 'inlets' and k ~= 'outlets' and k ~= 'name' then
      res[k] = self[k]
    end
  end

  if data.code or data.links then
    -- code changes can alter slots: dump them
    res.has_all_slots = true
    res.inlets  = dumpSlots(self.sorted_inlets)
    res.outlets = dumpSlots(self.sorted_outlets, data.links)
  end

  return res
end
