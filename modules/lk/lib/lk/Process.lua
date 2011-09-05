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
 __call = function(lib, opts)
  opts.zone = opts.zone or 'default'
  local self = lk.Patch(opts)
  local opts = {
    callback = function(...)
      return self:callback(...)
    end,

  }

  self.service = lk.Service(self.zone .. ':' .. self.name, opts)

  return self
end})

--- All code is in lk.Patch (Patch.lua)
