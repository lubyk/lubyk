--[[------------------------------------------------------

  editor.Inlet test
  -----------------

  ...

--]]------------------------------------------------------
require 'lubyk'

-- editor needs a 'main' state with currently selected objects
-- and such things. FIXME: remove.
editor.main = {}

local should = test.Suite('editor.Inlet')

-- Mock a node
local function mockNode(name)
  local instance = {
    name    = name,
    process = {},
    pending_inlets = {},
  }
  return instance
end

function should.createInlet()
  local node   = mockNode('dummy')
  local inlet = editor.Inlet(node, 'tempo', {})

  assertEqual('tempo', inlet.name)
  assertEqual('editor.Inlet', inlet.type)
end

function should.createPendingInlet()
  local node    = mockNode('dummy')
  local pending = {}
  local inlet   = editor.Inlet(pending, 'tempo') -- no definition

  assertEqual('tempo', inlet.name)
  assertEqual('editor.Inlet', inlet.type)
  -- pending inlet properly registered
  assertEqual(inlet, pending.tempo)
end

function should.usePendingInlet()
  local node    = mockNode('dummy')
  local pending = node.pending_inlets
  local pinlet  = editor.Inlet(pending, 'tempo') -- no definition
  local inlet   = editor.Inlet(node, 'tempo', {})

  assertEqual(pinlet, inlet)
  assertEqual('tempo', inlet.name)
  assertEqual('editor.Inlet', inlet.type)
  -- removed from pending list
  assertNil(node.pending_inlets.tempo)
end

test.all()
