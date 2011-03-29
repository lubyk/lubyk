--[[------------------------------------------------------

  editor.ProcessTab test
  ----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.ProcessTab')

local function mockProcess()
  return {name = 'Dummy ProcessTab', hue = 0.9}
end

function should.drawProcessTab(t)
  t.tab = editor.ProcessTab(mockProcess())
  t.tab:move(100, 100)
  t.tab:show()

  t.thread = lk.Thread(function()
    sleep(800)
    t.tab:close()
    assertTrue(true)
  end)
end

test.gui()
