--[[------------------------------------------------------

  editor.Process test
  -------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

-- editor needs a 'main' state with currently selected objects
-- and such things. FIXME: remove.
editor.main = {}

local should = test.Suite('editor.Process')

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

function should.updateName()
  local process = patch('dummy')
  process:update {
    name = 'hello'
  }
  assertEqual('hello', process.name)
  assertEqual('editor.Process', process.type)
end

function should.createNodes()
  local process = patch('dummy')
  process:update {
    nodes = {
      foo = {},
      bar = {},
    }
  }
  assertEqual('editor.Node', process.nodes.foo.type)
  assertEqual('editor.Node', process.nodes.bar.type)
end

function should.resolveLinks()
  local process = patch('dummy')
  process:update {
    nodes = {
      foo = {
        outlets = {
          a = {
            links = {'bar/in/x'},
          },
        },
      },
      bar = {
        inlets = {
          x = {},
        },
      },
    }
  }
  -- source.node.process:get(target_name)
  local link = process.nodes.foo.outlets.a.links[1]
  assertEqual('editor.Link', link.type)
  assertEqual(process.nodes.foo.outlets.a, link.source)
  assertEqual(process.nodes.bar.inlets.x, link.target)
end


test.all()
