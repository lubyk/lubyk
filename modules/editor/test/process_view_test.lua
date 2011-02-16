--[[------------------------------------------------------

  editor.ProcessView test
  -----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.ProcessView')

function mock_process()
  return {name = 'Dummy ProcessView', hue = 0.2}
end

function should.draw_process_view(t)
  t.tab = editor.ProcessView(mock_process())
  t.tab:move(100, 100)
  t.tab:show()

  t.thread = lk.Thread(function()
    sleep(5000)
    t.tab:setName('Home')
    sleep(4000)
    t.tab:close()
    assert_true(true)
  end)
end

test.gui()
