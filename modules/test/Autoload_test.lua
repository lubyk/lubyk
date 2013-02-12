--[[------------------------------------------------------

  h1. Autoload test

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('Autoload')

function should.raiseErrorOnStrict(t)
  t.a = 'something'

  Autoload.strict()
  assertError("Trying to access undefined global variable 'not_defined_global'.", function()
    t.a = not_defined_global
  end)

  -- Should allow autoload through require
  -- xml is not loaded by the scheduler
  assertType('table', xml)
  
  Autoload.strict(false)
  assertPass(function()
    t.a = not_defined_global
  end)

  assertNil(t.a)
end

test.all()
