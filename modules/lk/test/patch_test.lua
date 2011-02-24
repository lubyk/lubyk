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