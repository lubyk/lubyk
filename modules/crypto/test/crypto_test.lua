--[[------------------------------------------------------

  crypto test
  -----------

  ...

--]]------------------------------------------------------
require 'lubyk'
local digest = crypto.digest

local should = test.Suite('crypto')

function should.autoLoad()
  assertTrue(crypto)
end

function should.hashString()
  local sha1 = crypto.digest.new('sha1')
  assertEqual('e8909fbe698c2a8e5059d1a49887e72a3929c7ac', sha1:final('This is just'))
  assertEqual('531d5289be744a01ff4736dfce072061b5087a59', sha1:final(' some random string'))
end
test.all()
