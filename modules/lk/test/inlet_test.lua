--[[------------------------------------------------------

  lk.Inlet test
  --------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Inlet')

local function mock_node()
  return {inlets = {}}
end

function should.create_inlet()
  local inlet = lk.Inlet(mock_node(), 'tempo', 'Set tempo [bpm].')
  assert_equal('tempo', inlet.name)
  assert_equal('Set tempo [bpm].', inlet.info)
end

function should.add_inlet_to_node_once()
  local node  = mock_node()
  local inlet = lk.Inlet(node, 'tempo', 'Set tempo [bpm].')
  local inlet2= lk.Inlet(node, 'tempo', 'Set tempo [bpm].')
  -- same table identity
  assert_true(inlet == inlet2)
  assert_true(inlet == node.inlets.tempo)
end

test.all()