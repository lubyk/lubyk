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

function should.update_name_from_definition()
  local process = patch('dummy')
  process:update {
    name = 'hello'
  }
  assert_equal('hello', process.name)
  assert_equal('editor.Process', process.type)
end

function should.create_nodes_from_definition()
  local process = patch('dummy')
  process:update {
    nodes = {
      foo = {},
      bar = {},
    }
  }
  assert_equal('editor.Node', process.nodes.foo.type)
  assert_equal('editor.Node', process.nodes.bar.type)
end

function should.create_and_link_links_from_definition()
  local process = patch('dummy')
  process:update {
    nodes = {
      foo = {
        outlets = {
          a = {'bar/in/x'}
        }
      },
      bar = {
        inlets = {
          x = {}
        }
      },
    }
  }
  -- source.node.process:get(target_name)
  local link = process.nodes.foo.outlets.a.links[1]
  assert_equal('editor.Link', link.type)
  assert_equal(process.nodes.foo.outlets.a, link.source)
  assert_equal(process.nodes.bar.inlets.x, link.target)
end


test.all()
