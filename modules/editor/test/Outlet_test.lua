--[[------------------------------------------------------

  editor.Outlet test
  ------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

-- editor needs a 'main' state with currently selected objects
-- and such things. FIXME: remove.
editor.main = {}

local should = test.Suite('editor.Outlet')

-- Mock a node
local function mockNode(name)
  local instance = {
    name    = name,
    process = {
      -- links are not resolved
      get          = function() return nil end,
      pendingInlet = function() return {}  end,
    },
  }
  return instance
end

function should.createOutlet()
  local node   = mockNode('dummy')
  local outlet = editor.Outlet(node, 'bang', {})

  assertEqual('bang', outlet.name)
  assertEqual('editor.Outlet', outlet.type)
end

function should.buildLinks()
  local node   = mockNode('dummy')
  local outlet = editor.Outlet(node, 'bang', {links = {'counter/in/trigger'}})

  assertEqual('editor.Link', outlet.links[1].type)
end

test.all()
