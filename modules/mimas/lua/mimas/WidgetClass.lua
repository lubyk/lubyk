--[[------------------------------------------------------

  mimas.WidgetClass
  -----------------

  ...

--]]------------------------------------------------------
local super_mt = mimas_core.Widget_
local function noop(...)
  return true -- ignore event
end
-- helper to create sub-classes
-- MyWidget = mimas.WidgetClass('MyWidget')
function mimas.WidgetClass(class_name, flags)
  local lib         = {
    -- default methods
    init    = noop,
    paint   = noop,
    resized = noop,
    mouse   = noop,
    click   = noop,
    ['type']= class_name,
  }

  -- Methods not in class_name are mapped to calls to self.super
  function lib:__index(key)
    local m = rawget(lib, key)
    if m then
      return m
    else
      m = rawget(super_mt, key)
      if m then
        local function f(self, ...)
          return m(self.super, ...)
        end
        lib[key] = f
        return f
      end
    end
  end

  setmetatable(lib, {
    -- new method
   __call = function(lib, ...)
    local self = setmetatable({}, lib)
    self.super = mimas.Widget(flags or 0)
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
  return lib
end
