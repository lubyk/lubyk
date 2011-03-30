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
    name           = name,
    nodes          = {},
    pending_inlets = {}
  }
  setmetatable(instance, editor.Process)
  return instance
end

function should.setName()
  local process = patch('dummy')
  process:set {
    name = 'hello'
  }
  assertEqual('hello', process.name)
  assertEqual('editor.Process', process.type)
end

function should.createNodes()
  local process = patch('dummy')
  process:set {
    nodes = {
      foo = {},
      bar = {},
    }
  }
  assertEqual('editor.Node', process.nodes.foo.type)
  assertEqual('editor.Node', process.nodes.bar.type)
end

function should.createSlots()
  local process = patch('dummy')
  process:set {
    nodes = {
      foo = {
        inlets = {
          a = {},
        },
        outlets = {
          z = {},
        },
      },
    }
  }

  local a = process.nodes.foo.inlets.a
  local z = process.nodes.foo.outlets.z
  assertEqual('editor.Inlet', a.type)
  assertEqual(process:get('foo/in/a'), a)
  assertEqual('editor.Outlet', z.type)
  assertEqual(process:get('foo/out/z'), z)
end

function should.resolveLinksOnMissingTarget()
  local process = patch('dummy')
  process:set {
    nodes = {
      foo = {
        outlets = {
          a = {
            links = {'bar/in/x'},
          },
        },
      },
    }
  }

  -- link target created later
  process:set {
    nodes = {
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

function should.resolveLinksOnExistingTarget()
  local process = patch('dummy')
  process:set {
    nodes = {
      bar = {
        inlets = {
          x = {},
        },
      },
    }
  }

  -- link target does not exist on creation
  process:set {
    nodes = {
      foo = {
        outlets = {
          a = {
            links = {'bar/in/x'},
          },
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
