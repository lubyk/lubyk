--[[------------------------------------------------------

  lk.Environment test
  -------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Environment')

-- global env
env_test_global = 1

function should.protect_global_env()
  local env = lk.Environment[[
    assert_equal(1, env_test_global)
    -- new value in protected env
    env_test_global = 2
    assert_equal(2, env_test_global)
  ]]
  env()
  assert_equal(1, env_test_global)
end

function should.read_global_env()
  local env = lk.Environment[[
    assert_equal(3, env_test_global2)
  ]]
  -- new global created after env compilation
  env_test_global2 = 3
  env()
  -- a copy exists in env, does not read this anymore
  -- (performance).
  env_test_global2 = 4
  env()
end

test.all()