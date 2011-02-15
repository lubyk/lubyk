--[[------------------------------------------------------

  mimas.WidgetClass
  -----------------

  ...

--]]------------------------------------------------------

-- helper to create sub-classes
-- MyWidget = mimas.WidgetClass()
function mimas.WidgetClass()
  local lib         = {}
  lib.__index       = lib

  setmetatable(lib, {
    -- new method
   __call = function(table, ...)
    local instance = {}
    instance.super = mimas.Widget()
    function instance.super.paint(p, w, h)
      instance:paint(p, w, h)
    end
    setmetatable(instance, lib)
    instance:init(...)
    return instance
  end})

  function lib:init()
    -- default = noop
  end

  function lib:paint(p, w, h)
    -- default = noop
  end

  -- FIXME: We need a better way to sub-class !

  function lib:widget()
    return self.super:widget()
  end

  function lib:addWidget(...)
    self.super:addWidget(...)
  end

  function lib:move(...)
    self.super:move(...)
  end

  function lib:resize(...)
    self.super:resize(...)
  end

  function lib:setMinimumSize(...)
    self.super:setMinimumSize(...)
  end

  function lib:show()
    return self.super:show()
  end

  function lib:close()
    return self.super:close()
  end

  return lib
end