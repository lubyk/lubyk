--[[------------------------------------------------------

  lk.Patch test
  -------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Patch')

function should.loadCode()
  assertTrue(lk.Patch)
end

local function makePatch()
  return lk.Patch(fixture.path('simple.yml'))
end

function should.createEmptyPatch()
  local patch = lk.Patch()
  assertEqual('lk.Patch', patch.type)
end

function should.createPatchWithNewFilename()
  local patch = lk.Patch(fixture.path('foo.yml'))
  assertEqual('lk.Patch', patch.type)
  assertFalse(patch.inline)
end

function should.createLiteralPatch()
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
  assertTrue(patch.inline)
end

function should.createPatchWithInlineCode()
  local patch = lk.Patch(fixture.path('inline.yml'))
  assertEqual('lk.Patch', patch.type)
  assertEqual(patch.nodes.store,               patch:get('store'))
  assertEqual(patch.nodes.add,                 patch:get('add'))
end

function should.addNodes()
  local patch = makePatch()
  assertType('table', patch.nodes.add)
  assertType('table', patch.nodes.store)
end

function should.set_defaultValues()
  local nodes = makePatch().nodes
  assertEqual(5, nodes.add.env.val2)
end

function should.getElementsFromUrl()
  local patch = makePatch()
  assertEqual(patch.nodes.store,               patch:get('store'))
  assertEqual(patch.nodes.add,                 patch:get('add'))
  assertEqual(patch.nodes.add.inlets.val1,     patch:get('add/in/val1'))
  assertEqual(patch.nodes.add.inlets.val2,     patch:get('add/in/val2'))
  assertEqual(patch.nodes.add.outlets.sum,     patch:get('add/out/sum'))
  assertEqual(patch.nodes.store.inlets.value,  patch:get('store/in/value'))
  assertEqual(patch.nodes.store.outlets.value, patch:get('store/out/value'))
end

function should.assertElementTypeFromUrl()
  local patch = makePatch()
  local obj, err = patch:get('store', lk.Node)
  assertTrue(obj)
  obj, err = patch:get('store', lk.Inlet)
  assertMatch('expected lk.Inlet, found lk.Node', err)
  obj, err = patch:get('add/in/val1', lk.Inlet)
  assertTrue(obj)
  obj, err = patch:get('add/in/val1', lk.Outlet)
  assertMatch('expected lk.Outlet, found lk.Inlet', err)
end

function should.connectNodes()
  local nodes = makePatch().nodes
  nodes.add.inlets.val1.receive(15)
  assertEqual(20, nodes.store.env.value)
end

test.all()