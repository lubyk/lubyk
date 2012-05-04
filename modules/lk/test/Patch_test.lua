--[[------------------------------------------------------

  lk.Patch test
  -------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Patch')

--local should2 = test.Suite('lk.Patch')
--local should = {}

function should.loadCode()
  assertTrue(lk.Patch)
end

local function makePatch(name)
  name = name or 'simple'
  local p = lk.Patch(name)
  function p:findCode(url)
    return lk.readAll(fixture.path('project' .. url))
  end
  p:set(yaml.loadpath(fixture.path('project/'..name..'/_patch.yml')))
  return p
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

function should.createPatchWithInlineCode()
  local patch = makePatch('inline')
  patch:set(yaml.loadpath(fixture.path('project/inline/_patch.yml')))
  assertEqual('lk.Patch', patch.type)
  assertEqual(patch.nodes.store, patch:get('/inline/store'))
  assertEqual(patch.nodes.add,   patch:get('/inline/add'))
end

function should.addNodes()
  local patch = makePatch()
  assertType('table', patch.nodes.add)
  assertType('table', patch.nodes.store)
end

function should.dumpNodes()
  local patch = makePatch()
  local dump  = patch:dump()
  assertType('table', dump.nodes.add)
  assertType('table', dump.nodes.store)
end

function should.partialDump()
  local patch = makePatch()
  local dump  = patch:partialDump{nodes={add={x=true}}}
  assertValueEqual({nodes={add={x=70}}}, dump)
end

function should.setDefaultValues()
  local nodes = makePatch().nodes
  assertEqual(5, nodes.add.env.val2)
end

function should.getElementsFromUrl()
  local patch = makePatch()
  assertEqual(patch.nodes.store,               patch:get('/simple/store'))
  assertEqual(patch.nodes.add,                 patch:get('/simple/add'))
  assertEqual(patch.nodes.add.inlets.val1,     patch:get('/simple/add/in/val1'))
  assertEqual(patch.nodes.add.inlets.val2,     patch:get('/simple/add/in/val2'))
  assertEqual(patch.nodes.add.outlets.sum,     patch:get('/simple/add/out/sum'))
  assertEqual(patch.nodes.store.inlets.value,  patch:get('/simple/store/in/value'))
  assertEqual(patch.nodes.store.outlets.value, patch:get('/simple/store/out/value'))
end

function should.getElementsFromPath()
  local patch = makePatch()
  assertEqual(patch.nodes.store,               patch:findByPath('store'))
  assertEqual(patch.nodes.add,                 patch:findByPath('add'))
  assertEqual(patch.nodes.add.inlets.val1,     patch:findByPath('add/in/val1'))
  assertEqual(patch.nodes.add.inlets.val2,     patch:findByPath('add/in/val2'))
  assertEqual(patch.nodes.add.outlets.sum,     patch:findByPath('add/out/sum'))
  assertEqual(patch.nodes.store.inlets.value,  patch:findByPath('store/in/value'))
  assertEqual(patch.nodes.store.outlets.value, patch:findByPath('store/out/value'))
end

function should.assertElementTypeFromUrl()
  local patch = makePatch()
  local obj, err = patch:get('/simple/store', lk.Node)
  assertTrue(obj)
  obj, err = patch:get('/simple/store', lk.Inlet)
  assertMatch('expected lk.Inlet, found lk.Node', err)
  obj, err = patch:get('/simple/add/in/val1', lk.Inlet)
  assertTrue(obj)
  obj, err = patch:get('/simple/add/in/val1', lk.Outlet)
  assertMatch('expected lk.Outlet, found lk.Inlet', err)
end

function should.connectNodes()
  local nodes = makePatch().nodes
  nodes.add.inlets.val1.receive(15)
  assertEqual(20, nodes.store.env.value)
end

function should.findProcess()
  local patch = makePatch()
  patch.process_watch = {}
  -- mock process_watch
  function patch.process_watch:process(name)
    return lk.RemoteProcess(name)
  end
  local other = patch:findProcess('foobar')
  assertEqual('lk.RemoteProcess', other.type)
end

test.all()
