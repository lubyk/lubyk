--[[------------------------------------------------------

  lk.Patch test
  -------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Patch')

function should.load_code()
  assert_true(lk.Patch)
end

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

function should.add_nodes()
  local patch = lk.Patch()
  print('lk.Patch:addNode...')
  --local a = patch:addNode('a', make_node{value=111})
  --local b = patch:addNode('b', make_node{value=222})
  --patch:connect('a.output', 'b.input')
  --assert_equal(111, a.value)
  --assert_equal(222, b.value)
  --a.input(333)
  --assert_equal(333, a.value)
  --assert_equal(333, b.value)
end

test.all()