--[[------------------------------------------------------

  lk.Node test
  ------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Node')

local function makePatch()
  local code = yaml.load(lk.content(fixture.path('foo.yml')))
  local patch = lk.Patch('foo')
  patch:set(code)
  return patch
end

local function makeNode(patch, name)
  name = name or 'foo'
  return lk.Node(patch or makePatch(), name, [[
    local i, o, p = lubyk.i, lubyk.o, lubyk.p
    o.scaled = {}

    p {
      scale = 1,
      vect = {
        x = 1,
        y = 0,
      },
    }

    function i.raw(x)
      received = x
      o.scaled(x * scale)
    end

    function assertReceived(x)
      assertEqual(x, received)
    end
  ]])
end

function should.runCode()
  local node = makeNode()
  assertEqual(1, node.env.scale)
end

function should.mergeParamsInEnv()
  local node = makeNode()
  node:setParams {
    scale = 2,
    ['vect.y'] = 5
  }
  assertEqual(2, node.env.scale)
  assertEqual(1, node.env.vect.x)
  assertEqual(5, node.env.vect.y)
end

function should.set()
  local node = makeNode()
  node:set{_ ={scale=2.0},x=10,y=30}
  assertEqual(2.0, node.env.scale)
  assertEqual(10, node.x)
  assertEqual(30, node.y)
end

function should.declareInlets()
  local node = makeNode()
  assertEqual('raw', node.inlets.raw.name)
end

function should.declareOutlets()
  local node = makeNode()
  assertEqual('scaled', node.outlets.scaled.name)
end

function should.dump()
  local node = makeNode()
  local dump = node:dump()
  assertEqual('foo', dump.name)
  assertType('table', dump.inlets)
  assertType('table', dump.outlets)
end


function should.partialDump()
  local node = makeNode()
  node.foo = 5
  local dump = node:partialDump{foo = 1}
  assertValueEqual({foo = 5}, dump)
end

function should.readGlobalEnv()
  local node = lk.Node(makePatch(), 'foo', [[
    function test(x)
      assertEqual(x, node_test_global)
    end

    function set(x)
      node_test_global = x
    end
  ]])
  node_test_global = 3
  -- new global created on first read
  node.env.test(3)
  -- a copy exists in env, does not read this anymore
  -- (performance).
  env_test_global = 4
  node.env.test(3)
  -- change does not propagage outside
end

function should.notWriteGlobalEnv()
  local node = lk.Node(makePatch(), 'foo', [[
    function test(x)
      assertEqual(x, node_test_global)
    end

    function set(x)
      node_test_global = x
    end
  ]])
  node_test_global = 1
  node.env.set(3)
  node.env.test(3)
  -- change does not propagage outside
  assertEqual(1, node_test_global)
end

function should.respondToUrl()
  local a = makeNode(nil, 'a')
  local b = makeNode(nil, 'b')
  b.parent = a
  -- absolute path contains patch name
  assertEqual('/foo/a', a:url())
  assertEqual('/foo/a/b', b:url())
end

function should.respondToMakeAbsoluteUrl()
  local a = makeNode(nil, 'a')
  local b = makeNode(nil, 'b')
  b.parent = a
  -- absolute path contains patch name
  assertEqual('/foo/bar/baz', a:makeAbsoluteUrl('bar/baz'))
  assertEqual('/bar/baz', a:makeAbsoluteUrl('/bar/baz'))
  assertEqual('/foo/a/bar/baz', b:makeAbsoluteUrl('bar/baz'))
end

function should.connectSlots()
  local a = makeNode()
  local b = makeNode()
  a.outlets.scaled:connect(b.inlets.raw)
  a:setParams{scale=3}
  a.inlets.raw.receive(5)
  b.env.assertReceived(15)
end

function should.connectSlotsOnSet()
  local p = makePatch()
  local a = makeNode(p)
  -- pending links (relative url to parent/patch)
  a:set{_={scale=3}, links={scaled={['b/in/raw']=true}}}
  -- should resolve links now
  local b = makeNode(p, 'b')
  a.inlets.raw.receive(5)
  b.env.assertReceived(15)
end

test.all()
