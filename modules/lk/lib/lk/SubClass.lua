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
    return self
  end

--]]------------------------------------------------------

-- MyWidget = lk.SubClass(mimas, 'Widget', ...)
function lk.SubClass(mod, class_name, ...)
  local constr   = assert(mod[class_name])
  local super_mt = assert(mod[class_name .. '_'])
  local defaults = {...}

  function super_mt.__call(lib, ...)
    return lib.new(...)
  end

  -- Create the new class
  local lib = {}
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
