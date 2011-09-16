--[[------------------------------------------------------

  remdebug test
  -------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('remdebug')

function should.autoLoad()
  assertTrue(remdebug)
end

test.all()
