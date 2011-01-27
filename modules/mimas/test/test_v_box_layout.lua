--[[------------------------------------------------------

  Basic tests for lubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.VBoxLayout')

local app = mimas.Application()

function should.display_widgets()
  local win = mimas.Window()
  local lay = mimas.VBoxLayout(win)
  local btn1 = mimas.PushButton("Hello")
  local btn2 = mimas.PushButton("Quit")
  lay:addWidget(btn1)
  lay:addWidget(btn2)

  win:move(100, 100)
  local callback = mimas.Callback(function()
    app:quit()
  end)

  callback:connect(btn1, 'clicked')
  callback:connect(btn2, 'clicked')

  win:show()
end

function should.nest_layouts()
  local win = mimas.Window()
  local lay = mimas.VBoxLayout(win)
  local sub = mimas.HBoxLayout()
  local labl = mimas.Label("Title")
  lay:addWidget(labl)
  lay:addLayout(sub)

  local btn1 = mimas.PushButton("Hello")
  local btn2 = mimas.PushButton("Quit")
  sub:addWidget(btn1)
  sub:addWidget(btn2)

  win:move(100, 100)
  local callback = mimas.Callback(function()
    app:quit()
  end)

  callback:connect(btn1, 'clicked')
  callback:connect(btn2, 'clicked')

  win:show()
end


app:post(function()
  test.all()
end)

app:exec()