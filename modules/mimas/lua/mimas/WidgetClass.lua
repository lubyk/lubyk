--[[------------------------------------------------------

  mimas.WidgetClass
  -----------------

  ...

--]]------------------------------------------------------
local super_mt = mimas_core.Widget_

function super_mt.__call(lib, ...)
  return lib.new(...)
end

-- helper to create sub-classes
-- MyWidget = mimas.WidgetClass('MyWidget')
function mimas.WidgetClass(class_name, flags)
  local lib = {
    -- default methods
    ['type']= class_name,
  }
  lib.__index = lib
  setmetatable(lib, super_mt)

  -- new method
  function lib.new(...)
    local self = mimas.Widget(flags or 0)
    setmetatable(self, lib)
    self:init(...)
    return self
  end
  return lib
end
