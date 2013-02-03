--[[------------------------------------------------------

  lk.OutletMethod test
  -------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.OutletMethod')

local function mockNode()
  return {
    outlets = {},
    slots = {
      outlets = {},
    },
    env = {},
  }
end

function should.createOutletMethod()
  local node = mockNode()
  local o = lk.OutletMethod(node)
  assertType('table', o)
  assertEqual('lk.OutletMethod', o.type)
end

function should.createNewOutletOnCall()
  local node = mockNode()
  local outlet = lk.OutletMethod(node)
  assertPass(function()
    outlet.bang = {connect = 'Connect', disconnect = 'Disconnect'}
  end)
  assertEqual('Connect', node.outlets.bang.opts.connect)
  assertEqual('Disconnect', node.outlets.bang.opts.disconnect)
  local bang = node.outlets.bang
  assertEqual('bang', bang.name)
end

function should.createOutletFunction()
  local node = mockNode()
  local outlet = lk.OutletMethod(node)
  outlet.draw = {info = 'Connect drawing methods.'}
  local func = outlet.draw
  assertType('function', func)
end

test.all()
