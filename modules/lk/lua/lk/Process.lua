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
--setmetatable(lib, lk.Patch)

setmetatable(lib, {
  -- new method
 __call = function(lib, filepath_or_code)
  local instance = lk.Patch(filepath_or_code, true)
  -- TODO: can we avoid this extra step by passing
  -- self in callback ?
  instance.service = lk.Service(instance.name, function(...)
    return instance:callback(...)
  end)
  return instance
end})
