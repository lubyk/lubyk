--[[------------------------------------------------------

  lk.OutletMethod test
  -------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.OutletMethod')

local function mock_node()
  return {outlets = {}}
end

function should.create_outlet_method()
  local node = mock_node()
  local outlet = lk.OutletMethod(node)
  assert_type('table', outlet)
end

function should.create_new_outlets_on_call()
  local node = mock_node()
  local outlet = lk.OutletMethod(node)
  assert_pass(function()
    outlet('bang', 'Bang.')
    local outl = node.outlets.bang
    outlet('bang', 'Send bang.')
    -- multiple calls do not create new outlets
    assert_equal(outl, node.outlets.bang)
  end)
  local bang = node.outlets.bang
  assert_equal('bang', bang.name)
  assert_equal('Send bang.', bang.info)
end

test.all()