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
    local first
    local s = '{'
    for k,v in pairs(o) do
      if not first then
        s = s .. '\n'
      end
      if type(k) == 'number' then
        s = s .. indent .. '[' .. k .. ']'
      elseif k:match('^[a-zA-Z_]+$') then
        s = s .. indent .. k 
      else
        s = s .. indent .. '["'.. k .. '"]'
      end
      s = s .. ' = ' .. dump(v, indent .. '  ') .. ',\n'
    end
    return s .. '}'
  elseif type(o) == 'number' then
    return tostring(o)
  elseif type(o) == 'string' then
    return '"' .. o:gsub('"', '\\"') .. '"'
  else
    -- IGNORE
  end
end

private.dump = dump

