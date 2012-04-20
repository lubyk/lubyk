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
 __call = function(lib, name)
  local self = lk.Patch(name)
  local opts = {
    callback = function(...)
      return self:callback(...)
    end,

    registration_callback = function(reg, service)
      if Lubyk.zone .. ':' .. name ~= service.name then
        printf("Existing process '%s'. Quit.", name)
        sched:quit()
      else
        self:start()
      end
    end,
  }

  self.service = lk.Service(Lubyk.zone .. ':' .. self.name, opts)

  return self
end})

--- All code is in lk.Patch (Patch.lua)
