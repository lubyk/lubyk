--[[------------------------------------------------------

  Basic tests for lubyk GUI
  -------------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas')

function should.parseNumbers()
  -- When Qt is loaded, ensure we stay in "C" locale.
  local w = mimas.Window()
  local code = loadstring [[
  assertEqual(1.9, 1.4 + 0.5)
  ]]
  assertTrue(code)
  assertPass(function()
    code()
  end)
end

test.all()

