--[[------------------------------------------------------

  lk.Outlet test
  --------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Outlet')

local function mock_node()
  return {outlets = {}, inlets = {}, pending_inlets={}}
end

function should.createOutlet()
  local node = mock_node()
  local bang = lk.Outlet(node, 'bang', 'Bangs on every beat.')
  assertType('function', bang)
  assertEqual('bang', node.outlets.bang.name)
  assertEqual(bang, node.outlets.bang.send)
  assertEqual('Bangs on every beat.', node.outlets.bang.info)
end

function should.connectToInlets()
  local node = mock_node()
  local bang = lk.Outlet(node, 'bang', 'Send bang.')
  local beep = lk.Inlet(node, 'beep', 'Receives bangs.')
  assertPass(function()
    node.outlets.bang:connect(node.inlets.beep)
  end)
  local t = 0
  function beep.receive(value)
    t = value
  end
  bang(120)
  assertEqual(120, t)
end


test.all()