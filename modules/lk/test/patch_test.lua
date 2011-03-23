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

local function make_patch()
  return lk.Patch(fixture.path('simple.yml'))
end

function should.create_empty_patch()
  local patch = lk.Patch()
  assert_equal('lk.Patch', patch.type)
end

function should.create_patch_with_new_filename()
  local patch = lk.Patch(fixture.path('foo.yml'))
  assert_equal('lk.Patch', patch.type)
  assert_false(patch.inline)
end

function should.create_literal_patch()
  local patch = lk.Patch [[
add:
  script: |
    inlet('val1', 'First value [number].')
    inlet('val2', 'Second value [number].')
    sum = outlet('sum', 'Sends sum of first and second values [number].')

    val1 = val1 or 0
    val2 = val2 or 0

    function inlet.val1(v)
      val1 = v
      sum(val1 + val2)
    end

    function inlet.val2(v)
      val2 = v
      sum(val1 + val2)
    end
  x: 70
  y: 95
  links:
    sum: store/in/value
  params:
    val1: 0
    val2: 5

store:
  class: fixtures.store
  x: 70
  y: 135
]]
  assert_true(patch.inline)
end

function should.create_patch_with_inline_code()
  local patch = lk.Patch(fixture.path('inline.yml'))
  assert_equal('lk.Patch', patch.type)
  assert_equal(patch.nodes.store,               patch:get('store'))
  assert_equal(patch.nodes.add,                 patch:get('add'))
end

function should.add_nodes()
  local patch = make_patch()
  assert_type('table', patch.nodes.add)
  assert_type('table', patch.nodes.store)
end

function should.set_default_values()
  local nodes = make_patch().nodes
  assert_equal(5, nodes.add.env.val2)
end

function should.get_elements_from_url()
  local patch = make_patch()
  assert_equal(patch.nodes.store,               patch:get('store'))
  assert_equal(patch.nodes.add,                 patch:get('add'))
  assert_equal(patch.nodes.add.inlets.val1,     patch:get('add/in/val1'))
  assert_equal(patch.nodes.add.inlets.val2,     patch:get('add/in/val2'))
  assert_equal(patch.nodes.add.outlets.sum,     patch:get('add/out/sum'))
  assert_equal(patch.nodes.store.inlets.value,  patch:get('store/in/value'))
  assert_equal(patch.nodes.store.outlets.value, patch:get('store/out/value'))
end

function should.assert_element_type_from_url()
  local patch = make_patch()
  local obj, err = patch:get('store', lk.Node)
  assert_true(obj)
  obj, err = patch:get('store', lk.Inlet)
  assert_match('expected lk.Inlet, found lk.Node', err)
  obj, err = patch:get('add/in/val1', lk.Inlet)
  assert_true(obj)
  obj, err = patch:get('add/in/val1', lk.Outlet)
  assert_match('expected lk.Outlet, found lk.Inlet', err)
end

function should.connect_nodes()
  local nodes = make_patch().nodes
  nodes.add.inlets.val1.receive(15)
  assert_equal(20, nodes.store.env.value)
end

test.all()