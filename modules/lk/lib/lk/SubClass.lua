--[[------------------------------------------------------

  lk.SubClass
  -----------

  Method to create subclasses of LuaObject based classes.
  MyWidget = lk.SubClass(mimas, 'Widget', mimas.SplashScreenFlag)

  function MyWidget:init(...)
    -- initialize self
  end

  For more control, you can write your own 'new' method:

  function MyWidget.new(...)
    local self = mimas.Widget()
    setmetatable(self, MyWidget)
    self:init(...)
    return self
  end

--]]------------------------------------------------------

-- MyWidget = lk.SubClass(mimas, 'Widget', ...)
function lk.SubClass(mod, class_name, ...)
  local constr   = assert(mod[class_name])
  local super_mt = rawget(mod,class_name .. '_') or mod[class_name]
  local defaults = {...}

  function super_mt.__call(lib, ...)
    return lib.new(...)
  end

  -- Create the new class
  local lib = {type = class_name}
  lib.__index = lib
  setmetatable(lib, super_mt)

  -- default new
  function lib.new(...)
    local self = constr(unpack(defaults))
    setmetatable(self, lib)
    self:init(...)
    return self
  end

  return lib
end
