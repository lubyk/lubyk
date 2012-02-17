--[[------------------------------------------------------

  lk.InletMethod test
  -------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.InletMethod')

local function mockNode()
  return {
    name = 'foo',
    inlets = {},
    slots = {
      inlets = {},
    },
    pending_inlets = {},
  }
end

function should.createInletMethod()
  local node = mockNode()
  local inlet = lk.InletMethod(node)
  assertType('table', inlet)
end

function should.createNewInletsOnIndex()
  local node = mockNode()
  local inlet = lk.InletMethod(node)
  assertPass(function()
    function inlet.tempo()
    end
    local inl = node.inlets.tempo
    -- multiple calls do not create new inlets
    function inlet.tempo()
    end
    assertEqual(inl, node.inlets.tempo)
  end)
  local tempo = node.inlets.tempo
  assertEqual('tempo', tempo.name)
end

function should.usePendingInletsOnCall()
  local node = mockNode()
  local inl = lk.Inlet('tempo')
  -- this happens when the patch has pending connections to
  -- be resolved
  node.pending_inlets.tempo = inl
  local inlet = lk.InletMethod(node)
  function inlet.tempo()
  end
  assertEqual(inl, node.inlets.tempo)
  -- multiple calls do not create new inlets
  function inlet.tempo()
  end
  assertEqual(inl, node.inlets.tempo)
end

function should.setInletCallbackOnIndex()
  local node  = mockNode()
  local inlet = lk.InletMethod(node)
  local t = 0
  function inlet.tempo(x)
    t = x
  end
  node.inlets.tempo.receive(120)
  assertEqual(120, t)
end

function should.catchErrors()
  local node  = mockNode()
  local inlet = lk.InletMethod(node)
  local err
  function node:error(...)
    err = ...
  end
  function inlet.tempo(x)
    error('bad message')
  end
  assertPass(function()
    node.inlets.tempo.receive('foo')
  end)
  assertMatch('InletMethod_test.lua:[0-9]+: bad message', err)
end


test.all()
