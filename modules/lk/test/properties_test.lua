--[[------------------------------------------------------

  lk.Properties
  -------------

  Storage with notification on change.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Properties')

function should.create_prop_on_new_properties_accessor()
  local node = {}
  local prop = lk.Properties(node)
  assert_true(node.prop)
end

function should.declare_properties_with_node_property()
  local node = {}
  local prop = lk.Properties(node)
  node:property('foo', 'this is foo')
  assert_equal(node.prop.properties.foo.info, 'this is foo')
end

function should.trigger_callback_if_exists()
  local node = {}
  local prop = lk.Properties(node)
  node:property('foo', 'this is foo')
  function node:foo(value) return value + 1; end
  prop.foo = 3
  assert_equal(4, prop.foo)
end

function should.not_raise_an_error_on_missing_callback()
  local node = {}
  local prop = lk.Properties(node)
  node:property('foo', 'this is foo')
  prop.foo = 3
  assert_equal(3, prop.foo)
end

function should.raise_an_error_on_invalid_property()
  local node = {}
  local prop = lk.Properties(node)
  assert_error('Unknown property bar', function()
    prop.bar = 4
  end)
end

function should.connect_properties_on_connect()
  local node_a = {}
  local prop_a = lk.Properties(node_a)
  node_a:property('foo', 'this is foo')
  local node_b = {}
  local prop_b = lk.Properties(node_b)
  node_b:property('bar', 'this is bar')
  node_a:connect('foo', node_b, 'bar')
  -- node_a.foo ---> node_b.bar
  prop_a.foo = 4
  assert_equal(4, prop_b.bar)
end

function should.not_connect_properties_on_multiple_connect()
  local call_count = 0
  local node_a = {}
  local prop_a = lk.Properties(node_a)
  node_a:property('foo', 'this is foo')
  local node_b = {}
  local prop_b = lk.Properties(node_b)
  node_b:property('bar', 'this is bar')
  function node_b:bar(value)
    call_count = call_count + 1
    return value
  end
  node_a:connect('foo', node_b, 'bar')
  node_a:connect('foo', node_b, 'bar')
  -- node_a.foo ---> node_b.bar
  prop_a.foo = 4
  assert_equal(4, prop_b.bar)
  assert_equal(1, call_count)
end

function should.disconnect_prop_on_disconnect()
  local node_a = {}
  local prop_a = lk.Properties(node_a)
  node_a:property('foo', 'this is foo')
  local node_b = {}
  local prop_b = lk.Properties(node_b)
  node_b:property('bar', 'this is bar')
  node_b:property('baz', 'this is baz')
  node_a:connect('foo', node_b, 'bar')
  node_a:connect('foo', node_b, 'baz')
  node_a:disconnect('foo', node_b, 'bar')
  -- node_a.foo ---> node_b.bar
  prop_a.foo = 4
  assert_equal(nil, prop_b.bar)
  assert_equal(4, prop_b.baz)
end

function should.disconnect_all_on_disconnect_without_prop_name()
  local node_a = {}
  local prop_a = lk.Properties(node_a)
  node_a:property('foo', 'this is foo')
  local node_b = {}
  local prop_b = lk.Properties(node_b)
  node_b:property('bar', 'this is bar')
  node_b:property('baz', 'this is baz')
  node_a:connect('foo', node_b, 'bar')
  node_a:connect('foo', node_b, 'baz')
  node_a:disconnect('foo', node_b)
  -- node_a.foo ---> node_b.bar
  prop_a.foo = 4
  assert_equal(nil, prop_b.bar)
  assert_equal(nil, prop_b.baz)
end

function should.disconnect_all_on_disconnect_without_src_prop_name()
  local node_a = {}
  local prop_a = lk.Properties(node_a)
  node_a:property('foo', 'this is foo')
  local node_b = {}
  local prop_b = lk.Properties(node_b)
  node_b:property('bar', 'this is bar')
  node_b:property('baz', 'this is baz')
  node_a:connect('foo', node_b, 'bar')
  node_a:connect('foo', node_b, 'baz')
  node_a:disconnect(node_b)
  -- node_a.foo ---> node_b.bar
  prop_a.foo = 4
  assert_equal(nil, prop_b.bar)
  assert_equal(nil, prop_b.baz)
end

function should.trigger_callback_before_sending()
  local node_a = {}
  local prop_a = lk.Properties(node_a)
  node_a:property('foo', 'this is foo')
  local node_b = {}
  local prop_b = lk.Properties(node_b)
  node_b:property('bar', 'this is bar')
  node_a:connect('foo', node_b, 'bar')
  -- node_a.foo ---> node_b.bar
  function node_a:foo(value)
    assert_equal(nil, prop_b.bar)
    return value + 1
  end
  prop_a.foo = 4
  assert_equal(5, prop_b.bar)
end

function should.connect_to_any_method()
  local node = {}
  local prop = lk.Properties(node)
  node:property('foo', 'this is foo')
  local data = {}
  local function change_data(data, idx, value)
    data[idx] = value
  end
  node:connect('foo', change_data, data, 'tada')
  -- node_a.foo ---> change_data(data, 'tada', value)

  assert_equal(nil, data['tada'])
  prop.foo = 4
  assert_equal(4, data['tada'])
end

test.all()