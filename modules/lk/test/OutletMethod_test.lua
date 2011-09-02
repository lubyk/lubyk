--[[------------------------------------------------------

  lk.OutletMethod test
  -------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.OutletMethod')

local function mockNode()
  return {
    outlets        = {},
    sorted_outlets = {},
    env            = {},
  }
end

function should.createOutletMethod()
  local node = mockNode()
  local outlet = lk.OutletMethod(node)
  assertType('table', outlet)
end

function should.createNewOutletOnCall()
  local node = mockNode()
  local outlet = lk.OutletMethod(node)
  assertPass(function()
    outlet('bang', 'Bang.')
    local outl = node.outlets.bang
    outlet('bang', 'Send bang.')
    -- multiple calls do not create new outlets
    assertEqual(outl, node.outlets.bang)
  end)
  local bang = node.outlets.bang
  assertEqual('bang', bang.name)
  assertEqual('Send bang.', bang.info)
end

function should.createOutletFunction()
  local node = mockNode()
  local outlet = lk.OutletMethod(node)
  outlet('draw', 'Connect drawing methods.')
  -- TODO:
  -- outlet.draw:setLocalOnly()
  assertType('function', node.env.draw)
end

function should.accessOutletOnIndex()
  local node = mockNode()
  local outlet = lk.OutletMethod(node)
  outlet('doom')
  assertEqual('lk.Outlet', outlet.doom.type)
end
test.all()
