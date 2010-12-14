--[[------------------------------------------------------

  test the dummy module
  ---------------------

  The Dummy module does nothing and is just used as a way
  to make sure the build process is ok.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('dummy')

function should.respond_to_plat()
  assert_equal('macosx', dummy.plat())
end

test.all()