--[[------------------------------------------------------

  class test
  ----------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('class')

function should.autoload()
  assertType('function', class)
end

function should.createClass()
  assertPass(function()
    lib = class 'Foo'
  end)
end

function should.setType()
  local lib = class 'Foo'
  assertEqual('Foo', lib.type)
end

function should.callNew(t)
  local lib = class 'Foo'
  function lib.new(name)
    local self = { name = name }
    return setmetatable(self, lib)
  end

  local obj = lib('hello')

  assertEqual('hello', obj.name)
  assertEqual('Foo', obj.type)
end

function should.notDeclareGlobal(t)
  local lib = class 'Foo'
  assertNil(Foo)
end

function should.addToNamespace(t)
  -- global 'boom' namespac3
  boom = {}
  local lib = class 'boom.Bastic'
  assertEqual(lib, boom.Bastic)

  function lib.new(name)
    t.name = name
  end

  boom.Bastic('yo')
  assertEqual('yo', t.name)

  boom = nil
end

test.all()

