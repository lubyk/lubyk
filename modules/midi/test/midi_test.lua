--[[------------------------------------------------------

  midi test
  ---------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('midi')

function should.auto_load()
  assert_true(midi)
end

test.all()
