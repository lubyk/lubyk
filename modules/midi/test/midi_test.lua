--[[------------------------------------------------------

  midi test
  ---------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('midi')

function should.auto_load()
  assertTrue(midi)
end

test.all()
