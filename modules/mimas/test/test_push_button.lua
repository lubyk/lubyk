--[[------------------------------------------------------

  Basic tests for rubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('mimas.PushButton')

function should.display_button()
  local app = mimas.Application()
  local win = mimas.Widget()
  local btn = mimas.PushButton("Quit", win)
  win:resize(320, 240)
  local callback = mimas.Callback(function()
    app:quit()
  end)

  callback:connect(btn, 'clicked')

  win:show()
  app:exec()
end

test.all()