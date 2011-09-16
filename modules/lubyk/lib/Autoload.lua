local lib  = {}
local lib2 = {}
Autoload   = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, prefix)
  assert(prefix, 'Autoload needs a name')
  local self = {prefix = prefix}
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

--- Autoload for _G
function lib2.__index(table, name)
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
