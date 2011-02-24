--[[------------------------------------------------------

  lk.InletMethod test
  -------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.InletMethod')

local function mock_node()
  return {inlets = {}, inlets_pending={}}
end

function should.create_inlet_method()
  local node = mock_node()
  local inlet = lk.InletMethod(node)
  assert_type('table', inlet)
end

function should.create_new_inlets_on_call()
  local node = mock_node()
  local inlet = lk.InletMethod(node)
  assert_pass(function()
    inlet('tempo', 'Set tempo.')
    local inl = node.inlets.tempo
    inlet('tempo', 'Set tempo [bpm].')
    -- multiple calls do not create new inlets
    assert_equal(inl, node.inlets.tempo)
  end)
  local tempo = node.inlets.tempo
  assert_equal('tempo', tempo.name)
  assert_equal('Set tempo [bpm].', tempo.info)
end

function should.use_inlets_pending_on_call()
  local node = mock_node()
  local inl = lk.Inlet('tempo')
  -- this happens when the patch has pending connections to
  -- be resolved
  node.inlets_pending.tempo = inl
  local inlet = lk.InletMethod(node)
  assert_pass(function()
    inlet('tempo', 'Set tempo.')
    assert_equal(inl, node.inlets.tempo)
    inlet('tempo', 'Set tempo [bpm].')
    -- multiple calls do not create new inlets
    assert_equal(inl, node.inlets.tempo)
  end)
  local tempo = node.inlets.tempo
  assert_equal('tempo', tempo.name)
  assert_equal('Set tempo [bpm].', tempo.info)
end

function should.set_inlet_callback_on_index()
  local node  = mock_node()
  local inlet = lk.InletMethod(node)
  local tempo = inlet('tempo', 'Set tempo [bpm].')
  local t = 0
  function inlet.tempo(x)
    t = x
  end
  tempo.receive(120)
  assert_equal(120, t)
end

function should.raise_error_on_index_without_declaration()
  local node  = mock_node()
  local inlet = lk.InletMethod(node)
  assert_error("Inlet 'tempo' not declared", function()
    function inlet.tempo(x)
    end
  end)
end


test.all()