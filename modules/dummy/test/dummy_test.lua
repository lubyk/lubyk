--[[------------------------------------------------------

  test the dummy module
  ---------------------

  The Dummy module does nothing and is just used as a way
  to make sure the build process is ok.

--]]------------------------------------------------------
require 'lubyk'
require 'dummy.Dummy'

local should = test.Suite('dummy')

function should.haveStaticFunction()
  assertEqual('macosx', dummy.Dummy_plat())
end

function should.returnTableOnNew()
  local self = dummy.Dummy()
  assertType('table', self)
end

function should.containSuper()
  local obj = dummy.Dummy()
  assertType('userdata', obj.super)
end

function should.setAnyKeyOnObj()
  local obj = dummy.Dummy()
  obj.foo = 'foo'
  obj.bar = 5
  assertEqual('foo', obj.foo)
  assertEqual(5, obj.bar)
end

function should.readSelfToGetCallback()
  local obj = dummy.Dummy()
  local y = 0

  function obj:hello(nb)
    y = nb
  end

  -- the 'callback' method is used to simulate a C callback.
  obj:callback('hello', 15)
  assertEqual(15, y)
end

function should.passSelfInCallback()
  local obj = dummy.Dummy()
  local s
  function obj:tada()
    s = self
  end

  -- the 'callback' method is used to simulate a C callback.
  obj:callback('tada', 15)
  assertEqual(obj, s)
end

function should.getCFunctionInCallback()
  local obj = dummy.Dummy()
  -- C can also access C methods.
  assertEqual(138, obj:callback('addSomething', 15))
end

function should.overloadCallback()
  local obj = dummy.Dummy()
  assertEqual(138, obj:addSomething(15))
  -- we overload 'addSomething'
  function obj:addSomething(nb)
    -- we have access to the original method here
    return self.super:addSomething(nb + 10)
  end
  assertEqual(148, obj:addSomething(15))
end

function should.initializeWithTable()
  local obj = dummy.Dummy {
    name = "Muad'Dib"
  }

  assertEqual("Muad'Dib", obj.name)
end

test.all()
