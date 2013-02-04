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
local private = {}
local lubyk_mt = {}

setmetatable(lib, { __call = function(lib, ...) return lib.new(...) end })

function lib.new(process, name, code_str)
  local env  = {}
  -- new node
  local self = {
    -- Find inlet by name. Inlets not in slots.inlets are removed on GC.
    -- Do not change this field name: it is used by editor.Node.
    inlets  = setmetatable({}, {__mode = 'v'}),
    -- Find outlet by name. Outlets not in slots.outlets are removed on GC.
    -- Do not change this field name: it is used by editor.Node.
    outlets = setmetatable({}, {__mode = 'v'}),
    -- GC protection of inlets and outlets. Here the slots are ordered.
    slots = {
      inlets  = {},
      outlets = {},
    },
    accessors = {},
    env       = env,
    errors    = {},
    name      = name,
    process   = process,
  }
  setmetatable(self, lib)

  -- Table to declare and access outlets
  self.outlet_method = lk.OutletMethod(self)

  -- env has read access to _G
  setmetatable(env, env_mt)
  env.lubyk = setmetatable({
    -- Table to declare inlets.
    i = lk.InletMethod(self),
    -- Table to declare and access outlets.
    o = self.outlet_method,
    -- Table to declare and access parameters.
    p = lk.ParamMethod(self),
    -- Print to remote GUI
    print = function(...) self:log('info', ...) end,

  }, lubyk_mt)


  -- set any param
  env.lubyk.setParam = function(url, ...) process:setParam(env.lio, url, ...) end

  process.nodes[name] = self
  -- pending connection resolution
  self.pending_inlets = process.pending_nodes[name] or {}
  process.pending_nodes[name] = nil
  if code_str then
    self:eval(code_str)
  end
  return self
end

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
  local code, err = loadstring(code_str, self:url())
  if not code then
    self:error(err)
    return
  end

  -- In case of error, we rollback
  local old_slots = self.slots

  -- Reset list of GC protected slots.
  self.slots = {
    inlets  = {},
    outlets = {},
  }

  -- Clear cached outlet functions
  self.outlet_method:clear()

  local old_accessors = self.accessors
  self.accessors = {}

  -- code will execute in node's environment
  setfenv(code, self.env)
  -- We use sched pcall to enable yield during code eval (to
  -- launch mimas for example).
  local ok, err = sched:pcall(code)
  if not ok then
    self.slots = old_slots
    self.accessors = old_accessors
    self:error(err)
  else

    self.code = code_str
    -- Unused inlets and outlets will be collected by the GC.
  end
  self.process.need_cleanup = true
end

function lib:set(definition, ignore_links)

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
    if k == '_' then
      -- load params after evaluating script code (so that
      -- allowed params and inlets are defined).
      local params = definition._
      if params then
        self:setParams(params)
      end
    elseif k == 'source' or
           k == 'links' then
      -- ignore (we want to run these in a specific order)
    else
      self[k] = v
    end
  end

  if not ignore_links then
    local links = definition.links
    if links then
      self:setLinks(links)
    end
  end
end


function lib:error(...)
  self:log('error', ...)
end

local fmt = string.format
function lib:log(typ, ...)
  local all = {...}
  local f = all[1]
  local msg = ''
  if type(f) == 'table' then
    if f.__tostring then
      msg = tostring(f)
    else
      msg = tostring(f)
    end
  elseif #all > 1 then
    for i, v in ipairs(all) do
      if i == 1 then
        msg = tostring(v)
      else
        msg = msg .. '\t'..tostring(v)
      end
    end
  else
    msg = tostring(f)
  end
  
  self.process:notify {
    log = {
      url = self:url(),
      msg = msg,
      typ = typ,
    }
  }
end

function lib:makeAbsoluteUrl(url)
  if string.match(url, '^/') then
    return url
  else
    return (self.parent or self.process):url() .. '/' .. url
  end
end

function private:setLink(out_name, target_url, process)
  local outlet = self.outlets[out_name]
  if not outlet then
    self:error("Outlet name '%s' does not exist.", out_name)
  else
    local slot, err = process:get(target_url, lk.Inlet)
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

function private:removeLink(out_name, target_url)
  local outlet = self.outlets[out_name]
  if outlet then
    outlet:disconnect(target_url)
  end
end

function lib:setLinks(all_links)
  local process = self.process
  for out_name, links in pairs(all_links) do
    if type(links) == 'string' then
      private.setLink(self, out_name, links, process)
    else
      for target_url, link_def in pairs(links) do
        if link_def then
          private.setLink(self, out_name, target_url, process)
        else
          private.removeLink(self, out_name, target_url)
        end
      end
    end
  end
end

local function dumpSlots(list, links)
  local res = {}
  for _,slot in ipairs(list) do
    table.insert(res, slot:dump(links))
  end
  return res
end

function lib:dump()
  return {
    name = self.name,
    hue  = self.hue,
    x    = self.x,
    y    = self.y,
    code = self.code,
    --- Params
    _    = private.dumpParams(self, true),
    inlets  = dumpSlots(self.slots.inlets),
    outlets = dumpSlots(self.slots.outlets),
  }
end

function lib:partialDump(data)
  local res = {}
  for k, v in pairs(data) do
    if k == '_' then
      res[k] = private.dumpParams(self, v)
    elseif k ~= 'inlets' and k ~= 'outlets' and k ~= 'name' then
      -- 'inlets' and 'outlets' should never be in the data
      res[k] = self[k]
    end
  end

  if data.code or data.links then
    -- code changes can alter slots: dump them

    res.has_all_slots = true
    res.inlets  = dumpSlots(self.slots.inlets)
    res.outlets = dumpSlots(self.slots.outlets, data.links)
    res._       = private.dumpParams(self, true)
  end
  return res
end

--- Node has been removed from patch, remove links and gc.
function lib:remove()
  -- self.fin = lk.Finalizer(function()
  --   print('******************* DELETED', self.name)
  -- end)
  -- Disconnect all links
  for _, outlet in ipairs(self.slots.outlets) do
    outlet:disconnectAll()
  end
  -- Remove from notification center
  self.process:onNotify(self, nil)
  self.env = nil
  self.process.need_cleanup = true
end

function lib:declareParams(hash)
  local defaults = {}
  self.defaults = defaults
  local env = self.env
  local accessors = self.accessors
  for k, v in pairs(hash) do
    if type(k) ~= 'string' then
      self:error("Default keys must be strings.")
    end
    if type(v) == 'table' then
      -- Transform foo = {x = 3} to
      -- 'foo.x' => accessor
      local base = env[k]
      if not base then
        base = {}
        env[k] = base
      end
      -- Only one level deep.
      for sk, sv in pairs(v) do
        if type(sk) ~= 'string' then
          self:error(string.format("Default in '%s' must be a string.", k))
        end
        local pname = k .. '.' .. sk
        if not accessors[pname] then
          accessors[pname] = function(value)
            -- This is executed during setParams.
            local rbase = env[k]
            if rbase then
              rbase[sk] = value
              -- So that param dump sees this value.
              env._pdump[pname] = rbase[sk]
            end
            local recv = accessors[k]
            if recv then
              recv(rbase)
            end
          end
        end
        -- This is done just once to set default values.
        defaults[pname] = sv
        env[pname] = sv
        if base[sk] == nil then
          base[sk] = sv
        end
      end
    else
      defaults[k] = v
      if env[k] == nil then
        -- Do not overwrite current values on script reload.
        env[k] = v
        local recv = accessors[k]
        if recv then
          recv(v)
        end
      end
    end
  end
end

--- Receive control events: update setting.
function lib:setParams(params)
  -- Prepare for partial dump
  local pdump    = {}
  self.pdump     = pdump
  self.pdump_params = params
  local env      = self.env
  -- Special pos.x accessors need to register
  -- set value in pdump directly.
  env._pdump     = pdump
  local accessors= self.accessors
  local defaults = self.defaults or {}
  if params == true then
    -- Query asked for a dump of the params.
    pdump = defaults
  else
    for k, value in pairs(params) do
      if not defaults[k] then
        -- Error notification: Invalid param.
        self:error("Trying to set invalid parameter '%s'.", k)
      else
        env[k] = value
        local recv = accessors[k]
        if recv then
          recv(value)
          -- 'receive' might set _pdump directly
          pdump[k] = pdump[k] or env[k]
        else
          pdump[k] = value
        end
      end
    end
  end
  -- Call 'param.changed' function in env if
  -- it exists.
  local func = accessors.changed
  if func then
    func(params)
  end
end

--- Receive a single param change from a source inside the process so
-- we need to notify.
function lib:setParam(k, value)
  local defaults  = self.defaults or {}
  local accessors = self.accessors
  if not defaults[k] then
    -- Error notification: Invalid param.
    self:error("Trying to set invalid parameter '%s'.", k)
  else
    self.env[k] = value
    local recv = accessors[k]
    if recv then
      recv(value)
    end
    -- Call 'param.changed' function in env if
    -- it exists.
    local func = accessors.changed
    if func then
      func({k = value})
    end
  end
  self.process:notify {
    -- YUCK ! FIXME: Why don't we use an alternative messaging method with
    -- string urls: notify('/foo/bar/_/baz', value) ?
    nodes = {
      [self.name] = {
        _ = {[k] = value},
      },
    },
  }
end

function private:dumpParams(params)
  if params == self.pdump_params then
    -- Optimization, send partial dump back.
    return self.pdump
  else
    local defaults = self.defaults
    if defaults then
      local env = self.env
      local res = {}
      if params == true then
        -- Dump all
        for k, _ in pairs(defaults) do
          res[k] = env[k]
        end
      else
        for k, _ in pairs(params) do
          -- Invalid param ?
          if not defaults[k] then
            -- Mark as invalid
            res[k] = {}
          else
            res[k] = env[k]
          end
        end
      end
      return res
    else 
      return nil
    end
  end
end

-- lubyk()
function lubyk_mt:__call()
  return self.i, self.o, self.p, self.print
end

function lubyk_mt:setParam(url, ...)
  self.process:setParam(self, url, ...)
end

function lubyk_mt:info()
  return {
    url = self.node:url()
  }
end

-- register a function that should be called if there is a
-- notification
function lubyk_mt:onNotify(callback)
  self.node.process:onNotify(self.node, callback)
end

lib.DEFAULT_CODE = [=[
--[[------------------------------------------------------

h1. TITLE

DESCRIPTION

--]]------------------------------------------------------
local i, o, p, print = lubyk()


]=]
