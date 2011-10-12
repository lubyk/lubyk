--[[------------------------------------------------------

  Basic tests for lubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.VBoxLayout')
local withUSer = should:testWithUser()

function should.autoload()
  assertTrue(mimas.VBoxLayout)
end

function should.createVBoxLayout()
  local win = mimas.Window()
  local lay = mimas.VBoxLayout(win)
  assertMatch('mimas.VBoxLayout', lay:__tostring())
end

function withUSer.should.displayWidgets(t)
  local win = mimas.Window()
  local lay = mimas.VBoxLayout(win)
  local btn1 = mimas.Button("Click here")
  local btn2 = mimas.Button("Ignore")
  lay:addWidget(btn1)
  lay:addWidget(btn2)

  win:move(100, 100)
  win:resize(100,100)
  local callback = mimas.Callback(function()
    t.continue = true
  end)

  callback:connect(btn1, 'clicked')
  callback:connect(btn2, 'clicked')

  win:show()
  t:timeout(function(done)
    return done or t.continue
  end)
  assertTrue(t.continue)
end

function withUSer.should.nestLayouts(t)
  local win = mimas.Window()
  local lay = mimas.VBoxLayout(win)
  local sub = mimas.HBoxLayout()
  local labl = mimas.Label("Title")
  lay:addWidget(labl)
  lay:addWidget(sub)

  local btn1 = mimas.Button("Click here")
  local btn2 = mimas.Button("Ignore")
  sub:addWidget(btn1)
  sub:addWidget(btn2)

  win:move(100, 100)
  win:resize(100,100)
  local callback = mimas.Callback(function()
    t.continue = true
  end)

  callback:connect(btn1, 'clicked')
  callback:connect(btn2, 'clicked')

  win:show()
  t:timeout(function(done)
    return done or t.continue
  end)
  assertTrue(t.continue)
end

test.all()
