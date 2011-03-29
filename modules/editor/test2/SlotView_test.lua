--[[------------------------------------------------------

  editor.SlotView test
  --------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

-- editor needs a 'main' state with currently selected objects
-- and such things
editor.main = editor.main or {}

local should = test.Suite('editor.SlotView')

local function mockSlot()
  return {node = {color = mimas.Color(0.9, 0.3, 0.8, 0.8)}}
end


function should.drawSlot(t)
  t.view = editor.SlotView(mockSlot())
  t.view:move(100, 100)
  t.view:resize(100, 100)
  t.view:show()

  t.thread = lk.Thread(function()
    sleep(800)
    t.view:close()
    assertTrue(true)
  end)
end

test.gui()
