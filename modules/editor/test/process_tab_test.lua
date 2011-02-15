--[[------------------------------------------------------

  editor.ProcessTab test
  ----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.ProcessTab')

function mock_process()
  return {name = 'Dummy ProcessTab', hue = 0.9}
end

function should.draw_process_tab(t)
  t.tab = editor.ProcessTab(mock_process())
  t.tab:move(100, 100)
  t.tab:show()

  t.thread = lk.Thread(function()
    sleep(1000)
    t.tab:close()
    assert_true(true)
  end)
end

test.gui()
