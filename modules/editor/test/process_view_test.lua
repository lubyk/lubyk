--[[------------------------------------------------------

  editor.ProcessView test
  -----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.ProcessView')

function mock_process()
  return {name = 'Dummy ProcessView', hue = 0.3}
end

function should.draw_process_view(t)
  t.view = editor.ProcessView(mock_process())
  t.view:move(100, 100)
  t.view:show()

  t.thread = lk.Thread(function()
    sleep(5000)
    t.view:setName('Home')
    sleep(4000)
    t.view:close()
    assert_true(true)
  end)
end

function should.draw_process_view(t)
  t.win = mimas.Window()
  t.view = editor.ProcessView(mock_process())
  t.win:addWidget(t.view)
  t.view:move(30, 40)
  t.view:resize(260, 240)
  -- overlay layout
  t.lay = mimas.HBoxLayout(t.win)
  t.lay:addStretch(2)
  t.lay:setContentsMargins(0,0,0,0)
  t.list = editor.ProcessList()
  t.list:addProcess{name = 'Dune', hue = 0.5}
  t.list:addProcess{name = 'Home Run', hue = 0.2}
  t.lay:addWidget(t.list, 0, mimas.AlignRight + mimas.AlignTop)

  t.win:move(100, 100)
  t.win:resize(320, 300)
  t.win:show()

  function t.win.mouse(x, y)
    t.view:move(x - 10, y - 10)
    t.win:update()
  end

  t.thread = lk.Thread(function()
    sleep(5000)
    t.view:setName('Home Run')
    sleep(14000)
    t.win:close()
    assert_true(true)
  end)
end


test.gui()
