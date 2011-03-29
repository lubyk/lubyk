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
  local inlet = editor.Inlet('tempo')

  assertEqual('tempo', inlet.name)
  assertEqual('editor.Inlet', inlet.type)
end


test.all()
