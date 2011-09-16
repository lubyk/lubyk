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
  assertFalse(continue)
  collectgarbage('collect')
  assertFalse(continue)
  fin = nil
  collectgarbage('collect')
  assertTrue(continue)
end

test.all()
