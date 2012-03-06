--[[------------------------------------------------------

  lk.Environment test
  -------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Environment')

-- global env
Environment_test_a = 1

function should.protectGlobalEnv()
  local env = lk.Environment[[
    assertEqual(1, Environment_test_a)
    -- new value in protected env
    Environment_test_a = 2
    assertEqual(2, Environment_test_a)
  ]]
  env()
  assertEqual(1, Environment_test_a)
end

function should.readGlobalEnv()
  local env = lk.Environment[[
    assertEqual(3, Environment_test_b)
  ]]
  -- new global created after env compilation
  Environment_test_b = 3
  env()
  -- a copy exists in env, does not read this anymore
  -- (performance).
  Environment_test_b = 4
  env()
end

test.all()
