--[[------------------------------------------------------

  Basic tests for lubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'lubyk'
Lubyk.mimas_quit_on_close = false

local should   = test.Suite('mimas.Widget')
local withUser = should:testWithUser()

--==================== Widget sub-class example =======

-- This is how you create a sub-class of mimas.Widget
local lib = lk.SubClass(mimas, 'Widget')
lib.type  = 'test.MyWidget'
MyWidget  = lib

function lib:init(name)
  self.name = name
end

-- custom paint
function lib:paint(p, w, h)
  local box_padding = 5
  local hpen_width = 2 -- half pen width
  local bp = hpen_width + box_padding -- full box padding
  local arc_radius = 15
  local text_padding = 20
  local pen = mimas.Pen(hpen_width * 2, mimas.Color(0.2, 0.8, 0.8, 0.8))
  pen:setStyle(mimas.DashLine)
  p:setBrush(mimas.Color(0.5, 0.5, 0.5))
  p:setPen(pen)
  p:drawRoundedRect(bp, bp, w + arc_radius + bp, h - 2 * bp, arc_radius)
  p:drawText(text_padding, text_padding, w - 2*text_padding, h - 2*text_padding, mimas.AlignCenter, self.name)
end

--=====================================================

function should.centerWidget(t)
  t.win = mimas.Window()
  t.win:setStyle 'background:transparent'
  t.lay = mimas.VBoxLayout(t.win)
  t.lbl = mimas.Label("This should be centered")
  t.win:resize(300,200)
  t.win:center()
  t.lbl:setStyle "color:#fee; text-align:center; background:orange"
  t.lay:addWidget(t.lbl)
  t.win:show()
  function t.win:click()
    t.continue = true
  end

  t:timeout(500, function(done)
    return done or t.continue
  end)
  t.win:close()
end

function should.addWidgetsToWindow(t)
  -- we use the test env to protect from gc
  t.win = mimas.Window()
  t.lay = mimas.VBoxLayout(t.win)
  t.wid1 = MyWidget('Hello')
  t.lay:addWidget(t.wid1)
  t.wid2 = MyWidget('Lubyk')
  t.lay:addWidget(t.wid2)
  t.win:move(100, 100)
  t.win:resize(400, 400)
  t.win:show()

  function t.win:click()
    t.continue = true
  end

  t:timeout(500, function(done)
    return done or t.continue
  end)
  t.win:close()
end

local function opName(op)
  if op == mimas.MousePress then
    return 'MousePress'
  elseif op == mimas.MouseRelease then
    return 'MouseRelease'
  elseif op == mimas.DoubleClick then
    return 'DoubleClick'
  else
    return '???'
  end
end

local function modName(mod)
  if mod == mimas.NoModifier then
    return 'NoModifier'
  elseif mod == mimas.ShiftModifier then
    return 'ShiftModifier'
  elseif mod == mimas.ControlModifier then
    return 'ControlModifier'
  elseif mod == mimas.AltModifier then
    return 'AltModifier'
  elseif mod == mimas.MetaModifier then
    return 'MetaModifier'
  elseif mod == mimas.KeypadModifier then
    return 'KeypadModifier'
  elseif mod == mimas.GroupSwitchModifier then
    return 'GroupSwitchModifier'
  else
    return '???'
  end
end

function withUser.should.respondToClick(t)
  -- we use the test env to protect from gc
  t.win = mimas.Window()
  function t.win:click(x, y, op, btn, mod)
    t.label:setText(
      string.format('x:%i y:%i op:%s btn:%i mod:%s',
        x,
        y,
        opName(op),
        btn,
        modName(mod)
      )
    )
  end

  t.lay = mimas.VBoxLayout(t.win)
  t.label = mimas.Label('Click on window (not on me, press ESC to close)')
  t.lay:addWidget(t.label)
  t.win:move(100, 300)
  t.win:resize(400, 400)
  t.win:show()
  t:timeout(2000, function(done)
    return done
  end)
  t.win:close()
end

--[[
function withUser.should.openFileDialog(t)
  local path = lk.file()
  local basedir, filename = lk.directory(path)
  t.win = mimas.Window()
  t.win:show()
  assertMatch(path..'$', t.win:getOpenFileName(string.format('Select "%s"', filename), basedir, 'Lua files (*.lua)'))
  t.win:close()
end

function withUser.should.getExistingDirectory(t)
  local path = lk.directory(lk.file())
  local basedir, dirname = lk.directory(path)
  t.win = mimas.Window()
  t.win:show()
  assertMatch(path..'$', t.win:getExistingDirectory(string.format('Select "%s"', dirname), basedir))
  t.win:close()
end
--]]

function withUser.should.callbackOnClose(t)
  t.win = mimas.Window()
  t.lay = mimas.VBoxLayout(t.win)
  t.lbl = mimas.Label('Close window.')
  t.lay:addWidget(t.lbl)
  function t.win:closed()
    t.continue = true
  end
  t.win:resize(100,100)
  t.win:move(10,10)
  t.win:show()
  t:timeout(function(done)
    return done or t.continue
  end)
  assertTrue(t.continue)
end

test.all()
