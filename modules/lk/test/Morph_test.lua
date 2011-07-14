--[[------------------------------------------------------

  lk.Morph test
  -------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Morph')

function should.loadCode()
  assertTrue(lk.Morph)
end

-- TODO: write tests ?

test.all()
