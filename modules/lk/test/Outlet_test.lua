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
    env            = {},
  }
end

function should.createOutlet()
  local node = mockNode()
  local bang = lk.Outlet(node, 'bang')
  assertEqual('lk.Outlet', bang.type)
  assertEqual('bang', bang.name)
  assertType('function', bang.send)
end

function should.dump()
  local node = mockNode()
  local bang = lk.Outlet(node, 'bang')
  local beep = lk.Inlet(node, 'beep')
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

function should.sendDefaultOnConnection(t)
  local parent = mockNode()
  local child = mockNode()
  local data = {}
  local conn_val1 = 'Connect value1'
  local conn_val2 = 'Connect value2'

  local bang = lk.Outlet(parent, 'bang', {connect = conn_val1})
  local bang2 = lk.Outlet(parent, 'bang2', {connect = conn_val2})
  local beep = lk.Inlet(child, 'beep')

  function beep.receive(x, conn)
    t.received = x
    -- Returns 'true' if the connection succeeds. This is used to
    -- indicate a bi-directional link.
    if x == conn_val1 then
      conn.type = 'Bidirectional'
    end
  end

  bang:connect(beep)

  assertEqual(conn_val1, t.received)
  assertEqual('Bidirectional', bang.links[beep:url()].type)

  bang2:connect(beep)
  assertEqual(conn_val2, t.received)
  assertEqual('Basic', bang2.links[beep:url()].type)
end

function should.sendDisconnectValue(t)
  local parent = mockNode()
  local child = mockNode()
  local data = {}
  local conn_val1 = 'Connect value1'
  local conn_val2 = 'Connect value2'
  local disc_val1 = 'Disconnect value1'
  local disc_val2 = 'Disconnect value2'

  local bang  = lk.Outlet(parent, 'bang',  {
    connect    = conn_val1,
    disconnect = disc_val1,
  })
  local bang2 = lk.Outlet(parent, 'bang2', {
    connect    = conn_val2,
    disconnect = disc_val2,
  })
  local beep  = lk.Inlet(child, 'beep')

  function beep.receive(x)
    t.received = x
  end

  bang:connect(beep)
  assertEqual(conn_val1, t.received)
  bang2:connect(beep)
  assertEqual(conn_val2, t.received)

  bang:disconnect(beep:url())
  assertEqual(disc_val1, t.received)

  bang2:disconnect(beep:url())
  assertEqual(disc_val2, t.received)
end

test.all()
