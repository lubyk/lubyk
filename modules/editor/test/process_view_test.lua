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

function should.draw_process_view(t)
  t.win = mimas.Window()
  t.tab = editor.ProcessView(mock_process())
  t.win:addWidget(t.tab)
  t.tab:move(30, 40)
  t.tab:resize(260, 240)
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

  t.thread = lk.Thread(function()
    sleep(5000)
    t.tab:setName('Home Run')
    sleep(14000)
    t.win:close()
    assert_true(true)
  end)
end


test.gui()
