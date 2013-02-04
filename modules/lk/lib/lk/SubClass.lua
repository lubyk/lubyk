--[[------------------------------------------------------

  lk.SubClass
  -----------

  Method to create subclasses of LuaObject based classes.
  MyWidget = lk.SubClass(mimas.Widget, mimas.SplashScreenFlag)

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

-- MyWidget = lk.SubClass(mimas.Widget, ...)
function lk.SubClass(class, ...)
  local defaults = {...}

  function class.__call(lib, ...)
    return lib.new(...)
  end

  if not class._new then
    -- Keep track of the topmost "new" function.
    -- We need this because we only want to call the bottom most "init"
    -- function to avoid calling methods on a version of self without the
    -- correct metatable.
    class._new = class.new
  end

  -- Create the new class
  local lib = {}
  lib.__index = lib

  -- Inherit methods from super class
  setmetatable(lib, class)

  -- Default new
  function lib.new(...)
    local self = class._new(unpack(defaults))
    setmetatable(self, lib)
    self:init(...)
    return self
  end

  -- Default init (only the last defined init function is called).
  function lib:init()
  end

  return lib
end
