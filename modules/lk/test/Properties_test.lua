--[[------------------------------------------------------

  lk.Properties
  -------------

  Storage with notification on change.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Properties')

function should.createPropOnNewPropertiesAccessor()
  local node = {}
  local prop = lk.Properties(node)
  assertTrue(node.prop)
end

function should.declarePropertiesWithNodeProperty()
  local node = {}
  local prop = lk.Properties(node)
  node:property('foo', 'this is foo')
  assertEqual(node.prop.properties.foo.info, 'this is foo')
end

function should.triggerCallbackIfExists()
  local node = {}
  local prop = lk.Properties(node)
  node:property('foo', 'this is foo')
  function node:foo(value) return value + 1; end
  prop.foo = 3
  assertEqual(4, prop.foo)
end

function should.notRaiseAnErrorOnMissingCallback()
  local node = {}
  local prop = lk.Properties(node)
  node:property('foo', 'this is foo')
  prop.foo = 3
  assertEqual(3, prop.foo)
end

function should.raiseAnErrorOnInvalidProperty()
  local node = {}
  local prop = lk.Properties(node)
  assertError('Unknown property bar', function()
    prop.bar = 4
  end)
end

function should.connectPropertiesOnConnect()
  local nodeA = {}
  local propA = lk.Properties(nodeA)
  nodeA:property('foo', 'this is foo')
  local nodeB = {}
  local propB = lk.Properties(nodeB)
  nodeB:property('bar', 'this is bar')
  nodeA:connect('foo', nodeB, 'bar')
  -- nodeA.foo ---> nodeB.bar
  propA.foo = 4
  assertEqual(4, propB.bar)
end

function should.notConnectPropertiesOnMultipleConnect()
  local callCount = 0
  local nodeA = {}
  local propA = lk.Properties(nodeA)
  nodeA:property('foo', 'this is foo')
  local nodeB = {}
  local propB = lk.Properties(nodeB)
  nodeB:property('bar', 'this is bar')
  function nodeB:bar(value)
    callCount = callCount + 1
    return value
  end
  nodeA:connect('foo', nodeB, 'bar')
  nodeA:connect('foo', nodeB, 'bar')
  -- nodeA.foo ---> nodeB.bar
  propA.foo = 4
  assertEqual(4, propB.bar)
  assertEqual(1, callCount)
end

function should.disconnectPropOnDisconnect()
  local nodeA = {}
  local propA = lk.Properties(nodeA)
  nodeA:property('foo', 'this is foo')
  local nodeB = {}
  local propB = lk.Properties(nodeB)
  nodeB:property('bar', 'this is bar')
  nodeB:property('baz', 'this is baz')
  nodeA:connect('foo', nodeB, 'bar')
  nodeA:connect('foo', nodeB, 'baz')
  nodeA:disconnect('foo', nodeB, 'bar')
  -- nodeA.foo ---> nodeB.bar
  propA.foo = 4
  assertEqual(nil, propB.bar)
  assertEqual(4, propB.baz)
end

function should.disconnectAllOnDisconnectWithoutPropName()
  local nodeA = {}
  local propA = lk.Properties(nodeA)
  nodeA:property('foo', 'this is foo')
  local nodeB = {}
  local propB = lk.Properties(nodeB)
  nodeB:property('bar', 'this is bar')
  nodeB:property('baz', 'this is baz')
  nodeA:connect('foo', nodeB, 'bar')
  nodeA:connect('foo', nodeB, 'baz')
  nodeA:disconnect('foo', nodeB)
  -- nodeA.foo ---> nodeB.bar
  propA.foo = 4
  assertEqual(nil, propB.bar)
  assertEqual(nil, propB.baz)
end

function should.disconnectAllOnDisconnectWithoutSrcPropName()
  local nodeA = {}
  local propA = lk.Properties(nodeA)
  nodeA:property('foo', 'this is foo')
  local nodeB = {}
  local propB = lk.Properties(nodeB)
  nodeB:property('bar', 'this is bar')
  nodeB:property('baz', 'this is baz')
  nodeA:connect('foo', nodeB, 'bar')
  nodeA:connect('foo', nodeB, 'baz')
  nodeA:disconnect(nodeB)
  -- nodeA.foo ---> nodeB.bar
  propA.foo = 4
  assertEqual(nil, propB.bar)
  assertEqual(nil, propB.baz)
end

function should.triggerCallbackBeforeSending()
  local nodeA = {}
  local propA = lk.Properties(nodeA)
  nodeA:property('foo', 'this is foo')
  local nodeB = {}
  local propB = lk.Properties(nodeB)
  nodeB:property('bar', 'this is bar')
  nodeA:connect('foo', nodeB, 'bar')
  -- nodeA.foo ---> nodeB.bar
  function nodeA:foo(value)
    assertEqual(nil, propB.bar)
    return value + 1
  end
  propA.foo = 4
  assertEqual(5, propB.bar)
end

function should.connectToAnyMethod()
  local node = {}
  local prop = lk.Properties(node)
  node:property('foo', 'this is foo')
  local data = {}
  local function changeData(data, idx, value)
    data[idx] = value
  end
  node:connect('foo', changeData, data, 'tada')
  -- nodeA.foo ---> changeData(data, 'tada', value)

  assertEqual(nil, data['tada'])
  prop.foo = 4
  assertEqual(4, data['tada'])
end

test.all()