--[[------------------------------------------------------

  mimas.WidgetClass
  -----------------

  ...

--]]------------------------------------------------------

local function noop(...)
  return true -- ignore event
end
-- helper to create sub-classes
-- MyWidget = mimas.WidgetClass()
function mimas.WidgetClass()
  local lib         = {
    -- default methods
    init    = noop,
    paint   = noop,
    resized = noop,
    mouse   = noop,
    click   = noop
  }
  lib.__index       = lib

  setmetatable(lib, {
    -- new method
   __call = function(table, ...)
    local instance = {}
    setmetatable(instance, lib)
    instance.super = mimas.Widget()
    function instance.super.paint(p, w, h)
      instance:paint(p, w, h)
    end
    function instance.super.resized(w, h)
      instance:resized(w, h)
    end
    function instance.super.mouse(x, y)
      return instance:mouse(x, y)
    end
    function instance.super.click(x, y, btn)
      return instance:click(x, y, btn)
    end
    instance:init(...)
    return instance
  end})

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