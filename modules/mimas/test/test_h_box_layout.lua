--[[------------------------------------------------------

  Basic tests for rubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('mimas.HBoxLayout')

function should.display_button()
  local app = mimas.Application()
  local win = mimas.Widget()
  local lay = mimas.HBoxLayout(win)
  local btn1 = mimas.PushButton("Hello")
  local btn2 = mimas.PushButton("Quit")
  lay:addWidget(btn1)
  lay:addWidget(btn2)

  win:resize(320, 240)
  local callback = mimas.Callback(function()
    app:quit()
  end)

  callback:connect(btn1, 'clicked')
  callback:connect(btn2, 'clicked')

  win:show()
  app:exec()
end

test.all()