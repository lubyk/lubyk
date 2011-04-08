--[[------------------------------------------------------

  lk.InletMethod test
  -------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.InletMethod')

local function mockNode()
  return {
    inlets = {},
    sorted_inlets = {},
    pending_inlets = {},
    name = 'foo'
  }
end

function should.createInletMethod()
  local node = mockNode()
  local inlet = lk.InletMethod(node)
  assertType('table', inlet)
end

function should.createNewInletsOnCall()
  local node = mockNode()
  local inlet = lk.InletMethod(node)
  assertPass(function()
    inlet('tempo', 'Set tempo.')
    local inl = node.inlets.tempo
    inlet('tempo', 'Set tempo [bpm].')
    -- multiple calls do not create new inlets
    assertEqual(inl, node.inlets.tempo)
  end)
  local tempo = node.inlets.tempo
  assertEqual('tempo', tempo.name)
  assertEqual('Set tempo [bpm].', tempo.info)
end

function should.usePendingInletsOnCall()
  local node = mockNode()
  local inl = lk.Inlet('tempo')
  -- this happens when the patch has pending connections to
  -- be resolved
  node.pending_inlets.tempo = inl
  local inlet = lk.InletMethod(node)
  assertPass(function()
    inlet('tempo', 'Set tempo.')
    assertEqual(inl, node.inlets.tempo)
    inlet('tempo', 'Set tempo [bpm].')
    -- multiple calls do not create new inlets
    assertEqual(inl, node.inlets.tempo)
  end)
  local tempo = node.inlets.tempo
  assertEqual('tempo', tempo.name)
  assertEqual('Set tempo [bpm].', tempo.info)
end

function should.setInletCallbackOnIndex()
  local node  = mockNode()
  local inlet = lk.InletMethod(node)
  local tempo = inlet('tempo', 'Set tempo [bpm].')
  local t = 0
  function inlet.tempo(x)
    t = x
  end
  tempo.receive(120)
  assertEqual(120, t)
end

function should.raiseErrorOnIndex_without_declaration()
  local node  = mockNode()
  local inlet = lk.InletMethod(node)
  assertError("Inlet 'tempo' not declared", function()
    function inlet.tempo(x)
    end
  end)
end


test.all()