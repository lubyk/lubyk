--[[------------------------------------------------------

  mimas.WidgetClass
  -----------------

  ...

--]]------------------------------------------------------

local function noop(...)
  return true -- ignore event
end
-- helper to create sub-classes
-- MyWidget = mimas.WidgetClass('MyWidget')
function mimas.WidgetClass(class_name)
  local lib         = {
    -- default methods
    init    = noop,
    paint   = noop,
    resized = noop,
    mouse   = noop,
    click   = noop,
    ['type']= class_name,
  }
  lib.__index       = lib

  setmetatable(lib, {
    -- new method
   __call = function(lib, ...)
    local self = {}
    setmetatable(self, lib)
    self.super = mimas.Widget()
    function self.super.paint(...)
      self:paint(...)
    end
    function self.super.resized(...)
      self:resized(...)
    end
    function self.super.mouse(...)
      return self:mouse(...)
    end
    function self.super.click(...)
      return self:click(...)
    end
    self:init(...)
    return self
  end})

  -- FIXME: We need a better way to sub-class !

  function lib:widget()
    return self.super:widget()
  end

  function lib:addWidget(...)
    self.super:addWidget(...)
  end

  function lib:setParent(...)
    self.super:setParent(...)
  end

  function lib:move(...)
    self.super:move(...)
  end

  function lib:resize(...)
    self.super:resize(...)
  end

  function lib:lower(...)
    self.super:lower(...)
  end

  function lib:raise(...)
    self.super:raise(...)
  end

  function lib:setSizeHint(...)
    self.super:setSizeHint(...)
  end

  function lib:setSizePolicy(...)
    self.super:setSizePolicy(...)
  end

  function lib:setMinimumSize(...)
    self.super:setMinimumSize(...)
  end

  function lib:update()
    self.super:update()
  end

  function lib:show()
    return self.super:show()
  end

  function lib:hide()
    return self.super:hide()
  end

  function lib:close()
    return self.super:close()
  end

  function lib:globalPosition()
    return self.super:globalPosition()
  end

  function lib:globalMove(...)
    return self.super:globalMove(...)
  end
  return lib
end