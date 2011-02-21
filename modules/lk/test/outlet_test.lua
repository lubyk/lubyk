--[[------------------------------------------------------

  lk.Outlet test
  --------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Outlet')

local function mock_node()
  return {outlets = {}, inlets = {}}
end

function should.create_outlet()
  local node = mock_node()
  local bang = lk.Outlet(node, 'bang', 'Bangs on every beat.')
  assert_type('function', bang)
  assert_equal('bang', node.outlets.bang.name)
  assert_equal(bang, node.outlets.bang.send)
  assert_equal('Bangs on every beat.', node.outlets.bang.info)
end

function should.connect_to_inlets()
  local node = mock_node()
  local bang = lk.Outlet(node, 'bang', 'Send bang.')
  local beep = lk.Inlet(node, 'beep', 'Receives bangs.')
  assert_pass(function()
    node.outlets.bang:connect(node.inlets.beep)
  end)
  local t = 0
  function beep.receive(value)
    t = value
  end
  bang(120)
  assert_equal(120, t)
end


test.all()