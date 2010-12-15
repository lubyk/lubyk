local lib  = {}
local lib2 = {}
Autoload   = lib

setmetatable(lib, {
  -- new method
 __call = function(table, prefix)
  assert(prefix, 'Autoload needs a name')
  local instance = {prefix = prefix}
  setmetatable(instance, lib)
  return instance
end})


--- Autoload method
function lib.__index(table, name)
  -- Trying to load new lib like rk.Foobar
  local ok, new_lib_or_error = require(table.prefix..'.'..name)
  if ok then
    return new_lib_or_error or table[name]
  else
    print(new_lib_or_error)
    return nil
  end
end

--- Autoload.global()
function lib.global()
  setmetatable(_G, lib2)
end

--- Autoload for _G
function lib2.__index(table, name)
  -- Trying to load new lib with rk.XXXX
  local ok, new_lib_or_error = require(name)
  if ok then
    return new_lib_or_error or table[name]
  else
    print(new_lib_or_error)
    return nil
  end
end
