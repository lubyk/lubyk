--[[------------------------------------------------------

  Basic tests for lubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.PushButton')

function should.draw_button()
  local win = mimas.Window()
  win:move(100, 100)

  local btn = mimas.PushButton("Button without layout", win)

  local callback = mimas.Callback(function()
    app:quit()
  end)

  callback:connect(btn, 'clicked')
  win:show()
end

function should.create_with_function(t)
  t.win = mimas.Window()
  t.win:move(100, 100)
  t.lay = mimas.HBoxLayout(t.win)
  t.btn = mimas.PushButton("Button with layout", function()
    app:quit()
  end)
  collectgarbage('collect')
  -- should not remove the callback because it is saved with
  -- the button's env
  t.lay:addWidget(t.btn)
  t.win:show()
  t.timeout = rk.Thread(function()
    sleep(3000)
    t.win:close()
  end)
end

test.all()