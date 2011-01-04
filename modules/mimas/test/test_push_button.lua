--[[------------------------------------------------------

  Basic tests for rubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('mimas.PushButton')

local app = mimas.Application()

function should.draw_button()
  local win = mimas.Widget()
  win:move(100, 100)

  local lbl = mimas.Label("Button without layout", win)

  local callback = mimas.Callback(function()
    app:quit()
  end)

  callback:connect(btn, 'clicked')
  win:show()
end

function should.insert_button_in_layout()
  local win = mimas.Widget()
  win:move(100, 100)
  local lay = mimas.HBoxLayout(win)
  local lbl = mimas.Label("Button with layout")
  lay:addWidget(lbl)
  local callback = mimas.Callback(function()
    app:quit()
  end)

  callback:connect(lbl, 'clicked')

  win:show()
end

app:post(function()
  test.all()
end)

app:exec()