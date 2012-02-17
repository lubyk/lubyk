--[[------------------------------------------------------

  lk.Outlet test
  --------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Outlet')

local function mockNode()
  return {
    inlets         = {},
    sorted_inlets  = {},
    pending_inlets = {},
    outlets        = {},
    sorted_outlets = {},
    name           = 'foo',
    url            = function() return '/node/url' end,
  }
end

function should.createOutlet()
  local node = mockNode()
  local bang = lk.Outlet('bang', node)
  assertEqual('lk.Outlet', bang.type)
  assertEqual('bang', bang.name)
  assertType('function', bang.send)
end

function should.dump()
  local node = mockNode()
  local bang = lk.Outlet('bang', node)
  local beep = lk.Inlet('beep', node)
  bang:connect(beep)
  local dump = bang:dump()
  assertEqual('bang', dump.name)
  assertType('table', dump.links)
  assertTrue(dump.links['/node/url/in/beep'])
end

function should.connectToInlets()
  local node = mockNode()
  local bang = lk.Outlet(node, 'bang', 'Send bang.')
  local beep = lk.Inlet(node, 'beep', 'Receives bangs.')
  assertPass(function()
    bang:connect(beep)
  end)
  local t = 0
  function beep.receive(value)
    t = value
  end
  bang.send(120)
  assertEqual(120, t)
end


test.all()
