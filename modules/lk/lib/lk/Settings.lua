--[[------------------------------------------------------

  lk.Settings
  -----------

  Per module preferences handling system. This is used by
  the module's settings (see editor.Setting for an example)
  The preferences/settings for lk are accessible with the
  global 'Lubyk' table.

--]]------------------------------------------------------
local lib     = {}
lib.__index   = lib
lk.Settings   = lib
local private = {}

setmetatable(lib, {
  __call = function(lib, mod_name, defaults)
    local self, path = private.loadSettings(mod_name)
    defaults.save = lib.save
    defaults.module = {name = mod_name, path = path}
    defaults.__index = defaults
    private.prepareTables(self, defaults)
    return setmetatable(self, defaults)
  end
})

--- Dump settings as a lua file in the settings path.
function lib:save()
  local path = self.module.path
  lk.writeall(path, lib.dump(self))
end

function lib:dump()
  return string.format('return %s\n', private.dump(self, '  '))
end

--=============================================== PRIVATE
function private.loadSettings(mod_name)
  local path = string.format('%s/.lubyk/%s.lua',
                 os.getenv('HOME'), mod_name)
  local code = loadfile(path)
  local self 
  if code then
    self = code()
  else
    self = {}
  end
  return self, path
end

local function dump(o, indent)
  if type(o) == 'table' then
    if o._placeholder then
      -- ignore
      return nil
    end
    local first = true
    local s = '{'
    for k,v in pairs(o) do
      local v = dump(v, indent .. '  ')
      if v then
        if first then
          s = s .. '\n'
          first = false
        end
        if type(k) == 'number' then
          s = s .. indent .. '[' .. k .. ']'
        elseif k:match('^[a-zA-Z_]+$') then
          s = s .. indent .. k 
        else
          s = s .. indent .. '["'.. k .. '"]'
        end
        s = s .. ' = ' .. v .. ',\n'
      end
    end
    return s .. '}'
  elseif type(o) == 'number' or
         type(o) == 'boolean' then
    return tostring(o)
  elseif type(o) == 'string' then
    return '"' .. o:gsub('"', '\\"') .. '"'
  else
    -- IGNORE
  end
end

private.dump = dump

-- Copy on write
local cow = {}

function cow.__index(tbl, key)
  return rawget(tbl._placeholder, key)
end

function cow.__newindex(tbl, key, value)
  -- make a copy of defaults
  local copy = tbl._self[tbl._key]
  if copy == tbl then
    copy = {}
    for k, v in pairs(tbl._placeholder) do
      copy[k] = v
    end
    copy[key] = value
    -- remove placeholder and work on the copy alone from now
    tbl._self[tbl._key] = copy
  else
    copy[key] = value
  end
end

function private:prepareTables(defaults)
  -- This is to enable natural value setting for tables
  -- editor.Setting.foo.bar = 'baz' -- <-- this should copy table defaults
  for k, v in pairs(defaults) do
    if type(v) == 'table' and not rawget(self, k) then
      self[k] = setmetatable({_placeholder = v, _self = self, _key = k}, cow)
    end
  end
end

