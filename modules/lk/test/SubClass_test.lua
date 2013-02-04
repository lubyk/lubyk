--[[------------------------------------------------------

  lk.SubClass test
  ----------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.SubClass')

local Foo = {a = 'foo', b = 'foo', c = 'foo'}
Foo.__index = Foo
setmetatable(Foo, {__call = function(lib, ...) return lib.new(...) end})

-- Base class should not call 'init'.
function Foo.new()
  return setmetatable({}, Foo)
end

local Bar = lk.SubClass(Foo)
Bar.b = 'bar'
function Bar:init()
  self.bar_init = true
end

local Baz = lk.SubClass(Bar)
Baz.c = 'baz'
function Bar:init()
  self.baz_init = true
end

function should.inherit()
  local foo = Foo()
  assertEqual(Foo, debug.getmetatable(foo))
  assertEqual(foo.a, 'foo')
  assertEqual(foo.b, 'foo')
  assertEqual(foo.c, 'foo')
  
  local bar = Bar()
  assertEqual(Bar, debug.getmetatable(bar))
  assertEqual(bar.a, 'foo')
  assertEqual(bar.b, 'bar')
  assertEqual(bar.c, 'foo')

  local baz = Baz()
  assertEqual(Baz, debug.getmetatable(baz))
  assertEqual(baz.a, 'foo')
  assertEqual(baz.b, 'bar')
  assertEqual(baz.c, 'baz')

  -- Make sure init is called only once.
  assertNil(  foo.bar_init )
  assertNil(  foo.baz_init )

  assertTrue( bar.bar_init )
  assertNil(  bar.baz_init )

  assertNil(  baz.bar_init )
  assertTrue( baz.baz_init )
end

test.all()
