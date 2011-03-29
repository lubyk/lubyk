--[[------------------------------------------------------

  editor.Node test
  ----------------

  ...

--]]------------------------------------------------------
require 'lubyk'

-- editor needs a 'main' state with currently selected objects
-- and such things. FIXME: remove.
editor.main = {}

local should = test.Suite('editor.Node')

-- Mock a process without network connectivity
local function patch(name)
  local instance = {
    name          = name,
    nodes         = {},
    pending_nodes = {}
  }
  setmetatable(instance, editor.Process)
  return instance
end

function should.createNode()
  local process = patch('dummy')
  local node = editor.Node {
    process = process,
    hue     = 0.5,
  }
  assertEqual(0.5, node.hue)
  assertEqual('editor.Node', node.type)
end

function should.createOutlets()
  local process = patch('dummy')
  local node = editor.Node {
    process = process,
    outlets = {
      a = {}
    }
  }
  assertEqual('editor.Outlet', node.outlets.a.type)
end

function should.createInlets()
  local process = patch('dummy')
  local node = editor.Node {
    process = process,
    inlets = {
      a = {}
    }
  }
  assertEqual('editor.Inlet', node.inlets.a.type)
end

test.all()
