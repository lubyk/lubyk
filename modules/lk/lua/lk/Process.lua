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
  local self = lk.Patch(filepath_or_code, true)
  local opts = {
    callback = function(...)
      return self:callback(...)
    end,
    
  }
  -- TODO: can we avoid this extra step by passing
  -- self in callback ?
  self.service = lk.Service(self.name, opts)

  return self
end})

--- All code is in lk.Patch (Patch.lua)