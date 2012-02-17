--[[------------------------------------------------------

  lk.Inlet test
  --------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Inlet')

local function mockNode()
  return {
    name = 'foo',
    inlets         = {},
    slots = {
      inlets = {},
    },
    pending_inlets = {},
    url = function() return '/node/url' end
  }

end

function should.createInlet()
  local inlet = lk.Inlet('tempo', mockNode())
  assertEqual('lk.Inlet', inlet.type)
  assertEqual('tempo', inlet.name)
end

function should.dump()
  local inlet = lk.Inlet('tempo', mockNode())
  local dump  = inlet:dump()
  assertEqual('tempo', dump.name)
end

function should.respondToUrl()
  local inlet = lk.Inlet('tempo', mockNode())
  assertEqual('/node/url/in/tempo', inlet:url())
end

function should.createPendingInlet()
  local inlet = lk.Inlet('tempo')
  assertEqual('tempo', inlet.name)
  assertEqual(nil, inlet.node)
end

test.all()
