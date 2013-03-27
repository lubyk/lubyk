local lib  = {}
local lib2 = {}
local private = {}

lk.Autoload = lib
-- Compatibility
Autoload = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, prefix, tbl)
  assert(not tbl, 'Should not pass a table')
  assert(prefix, 'Autoload needs a name')
  local self = tbl or {}
  self.prefix = prefix
  return setmetatable(self, lib)
end})


--- Autoload method
function lib.__index(table, name)
  -- Trying to load new lib like lk.Foobar
  local ok, new_lib_or_error = pcall(function() require(table.prefix..'.'..name); end)
  if ok then
    return new_lib_or_error or rawget(table, name)
  else
    print(new_lib_or_error)
    return nil
  end
end

--- Autoload.global()
function lib.global()
  setmetatable(_G, lib2)
end

--- Autoload.strict()
-- Once this method is called, disable any global variable access or code loading.
function lib.strict(x)
  if x == false then
    -- not strict
    lib2.__index = private.__index
  else
    -- disable autoload and raise on nil
    lib2.__index = private.__index_strict
  end
end

--- Autoload for _G
function private.__index(table, name)
  if name == '_PROMPT' or name == '_PROMPT2' then
    -- special case for lua interactive
    return nil
  end
  local ok, new_lib_or_error = pcall(function() require(name); end)
  if ok then
    return new_lib_or_error or table[name]
  else
    -- nil value
    return nil
  end
end
lib2.__index = private.__index

--- Autoload for _G (strict mode = no nil allowed)
function private.__index_strict(table, name)
  if name == '_PROMPT' or name == '_PROMPT2' then
    -- special case for lua interactive
    return nil
  end
  local ok, new_lib_or_error = pcall(function() require(name); end)
  if ok then
    return new_lib_or_error or table[name]
  else
    -- nil value
    error(string.format("Trying to access undefined global variable '%s'.\n%s", name, debug.traceback()))
  end
end
