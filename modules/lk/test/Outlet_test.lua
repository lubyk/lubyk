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

function should.callChildConnected(t)
  local parent = mockNode()
  local data = {}
  function parent.env.childConnected(func)
    -- Returns 'true' if the connection succeeds. This is used to
    -- indicate a bi-directional link.
    return func('foo.Bar', data)
  end

  local bang = lk.Outlet(parent, 'bang', 'Send bang.')

  local child = mockNode()
  function child.env.connected(type, data)
    if type == 'foo.Bar' then
      -- Do something with provided arguments.
      table.insert(data, 'child1')
      -- Return true to inform parent that connection is OK.
      return true
    end
  end

  local beep = lk.Inlet(child, 'beep', 'Receives bangs.')
  bang:connect(beep)

  assertValueEqual({
    'child1'
  }, data)

  assertEqual('Bidirectional', bang.links[beep:url()].type)
end

function should.callChildDisconnected(t)
  local parent = mockNode()
  local data = {}
  function parent.env.childConnected(func)
    return func('foo.Bar', data)
  end

  function parent.env.childDisconnected(func)
    -- Returns 'true' if the connection succeeds. This is used to
    -- indicate a bi-directional link.
    return func('foo.Bar', data)
  end

  local bang = lk.Outlet(parent, 'bang', 'Send bang.')

  local child = mockNode()
  function child.env.connected(type, d)
    table.insert(d, 'child1')
    return true
  end

  function child.env.disconnected(type, d)
    if type == 'foo.Bar' then
      -- Do something with provided arguments.
      for i, v in ipairs(d) do
        if v == 'child1' then
          table.remove(d, i)
        end
      end
    end
  end

  local beep = lk.Inlet(child, 'beep', 'Receives bangs.')
  bang:connect(beep)

  assertValueEqual({
    'child1'
  }, data)

  -- disconnect
  bang:disconnect(beep:url())
  assertValueEqual({
  }, data)
end


test.all()
