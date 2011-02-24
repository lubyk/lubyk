--[[------------------------------------------------------

  lk.Inlet test
  --------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Inlet')

local function mock_node()
  return {inlets = {}, inlets_pending = {}}
end

function should.create_inlet()
  local inlet = lk.Inlet(mock_node(), 'tempo', 'Set tempo [bpm].')
  assert_equal('tempo', inlet.name)
  assert_equal('Set tempo [bpm].', inlet.info)
end

function should.create_pending_inlet()
  local inlet = lk.Inlet('tempo')
  assert_equal('tempo', inlet.name)
  assert_equal(nil, inlet.node)
end

function should.use_pending_inlet_on_create()
  local node = mock_node()
  -- pending inlet
  local inlet = lk.Inlet('tempo')
  node.inlets_pending = {tempo=inlet}

  assert_equal(inlet, lk.Inlet(node, 'tempo', 'Set tempo [bpm].'))
  assert_equal('Set tempo [bpm].', inlet.info)
end

function should.add_inlet_to_node_once()
  local node  = mock_node()
  local inlet = lk.Inlet(node, 'tempo', 'Set tempo [bpm].')
  local inlet2= lk.Inlet(node, 'tempo', 'Set tempo [bpm].')
  assert_equal(inlet, inlet2)
  assert_equal(inlet, node.inlets.tempo)
end

test.all()