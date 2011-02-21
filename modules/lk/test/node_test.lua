--[[------------------------------------------------------

  lk.Node test
  ------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Node')

local function make_node()
  return lk.Node[[
    inlet('raw', 'Receive raw value [float].')
    scaled = outlet('scaled', 'Sends scaled value [float].')
    scale = scale or 1

    function inlet.raw(x)
      received = x
      scaled(x * scale)
    end

    function assert_received(x)
      assert_equal(x, received)
    end
  ]]
end

function should.return_run_code()
  local node = make_node()
  assert_equal(1, node.env.scale)
end

function should.merge_params_in_env()
  local node = make_node()
  node:set{scale=2.0}
  assert_equal(2.0, node.env.scale)
end

function should.declare_inlets()
  local node = make_node()
  assert_equal('raw', node.inlets.raw.name)
end

function should.declare_outlets()
  local node = make_node()
  assert_equal('scaled', node.outlets.scaled.name)
end

function should.read_global_env()
  local node = lk.Node[[
    function test(x)
      assert_equal(x, node_test_global)
    end

    function set(x)
      node_test_global = x
    end
  ]]
  node_test_global = 3
  -- new global created on first read
  node.env.test(3)
  -- a copy exists in env, does not read this anymore
  -- (performance).
  env_test_global = 4
  node.env.test(3)
  -- change does not propagage outside
end

function should.not_write_global_env()
  local node = lk.Node[[
    function test(x)
      assert_equal(x, node_test_global)
    end

    function set(x)
      node_test_global = x
    end
  ]]
  node_test_global = 1
  node.env.set(3)
  node.env.test(3)
  -- change does not propagage outside
  assert_equal(1, node_test_global)
end

function should.connect_slots()
  local a = make_node()
  local b = make_node()
  a.outlets.scaled:connect(b.inlets.raw)
  a:set{scale=3}
  a.inlets.raw.receive(5)
  b.env.assert_received(15)
end

test.all()