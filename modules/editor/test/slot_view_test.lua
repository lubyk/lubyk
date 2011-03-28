--[[------------------------------------------------------

  editor.SlotView test
  --------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.SlotView')

local function mock_slot()
  return {node = {color = mimas.Color(0.9, 0.3, 0.8, 0.8)}}
end

-- editor needs a 'main' state with currently selected objects
-- and such things
editor.main = editor.main or {}

function should.draw_slot(t)
  t.view = editor.SlotView(mock_slot())
  t.view:move(100, 100)
  t.view:resize(100, 100)
  t.view:show()

  t.thread = lk.Thread(function()
    sleep(800)
    t.view:close()
    assert_true(true)
  end)
end

test.gui()
