--[[------------------------------------------------------

  lk.Finalizer test
  -----------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Finalizer')

function should.autoload()
  assertType('function', lk.Finalizer)
end

function should.triggerOnGc()
  local continue = false
  local fin = lk.Finalizer(function()
    continue = true
  end)
  -- We make sure that we can set different finalizers at the same time.
  local fin2 = lk.Finalizer(function()
    continue2 = true
  end)
  assertFalse(continue)
  assertFalse(continue2)
  collectgarbage('collect')
  assertFalse(continue)
  assertFalse(continue2)
  fin = nil
  collectgarbage('collect')
  assertTrue(continue)
  assertFalse(continue2)
  fin2 = nil
  collectgarbage('collect')
  assertTrue(continue)
  assertTrue(continue2)
end

test.all()
