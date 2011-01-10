--[[------------------------------------------------------

  test the dummy module
  ---------------------

  The Dummy module does nothing and is just used as a way
  to make sure the build process is ok.

--]]------------------------------------------------------
require 'rubyk'
require 'dummy.Dummy'

local should = test.Suite('dummy')

function should.respond_to_plat()
  assert_equal('macosx', dummy.Dummy_plat())
end

function should.set_callback_function_on_userdatum()
  local x = dummy.Dummy()
  local y = 0
  function x.callback()
    y = 15
  end

  x:callback()
  assert_equal(15, y)
end

test.all()