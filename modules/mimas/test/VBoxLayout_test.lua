--[[------------------------------------------------------

  mimas.VBoxLayout test
  ---------------------

  ...

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
  t:timeout(function()
    return t.continue
  end)
  assertTrue(t.continue)
end

function withUSer.should.changeAlignment(t)
  local win = mimas.Window()
  local lay = mimas.VBoxLayout(win)
  local btn1 = mimas.Button("Click to change alignment", function(self)
    if self.right then
      lay:setAlignment(self, mimas.AlignLeft)
      self.right = false
    else
      lay:setAlignment(self, mimas.AlignRight)
      self.right = true
    end
  end)
  local btn2 = mimas.Button("Click when done")
  lay:addWidget(btn1, 0, mimas.AlignLeft)
  lay:addWidget(btn2, 0, mimas.AlignLeft)

  win:move(100, 100)
  win:resize(100,100)
  local callback = mimas.Callback(function()
    t.continue = true
  end)

  callback:connect(btn2, 'clicked')

  win:show()
  t:timeout(function()
    return t.continue
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
  t:timeout(function()
    return t.continue
  end)
  assertTrue(t.continue)
end

test.all()
