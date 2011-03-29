--[[------------------------------------------------------

  lk.Process
  ----------

  A Process is just a lk.Patch with network connectivity
  through the use of an lk.Service.

--]]------------------------------------------------------
local lib   = {type='lk.Process'}
lib.__index = lib
lk.Process  = lib
-- lk.Process inherits from lk.Patch
setmetatable(lib, lk.Patch)

setmetatable(lib, {
  -- new method
 __call = function(table, filepath_or_code)
  local instance = lk.Patch(filepath_or_code)
  instance.name = string.match(instance.filepath, '([^%./]+)%.[a-z]+')
  instance.service = lk.Service(instance.name)
  setmetatable(instance, lib)
  return instance
end})

-- TODO
