--[[------------------------------------------------------

  Basic tests for lubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'lubyk'
require 'mimas.Window'

local should = test.Suite('mimas.Window')

-- FIXME: mimas.Window === metatable &&
--        use new instead of call ?
--        or use ___call in metatable ?
--
-- mock a subclass of mimas.Window
local LogWindow = {}
LogWindow.__index = LogWindow

function LogWindow:new(o)
  local o = o or {hue = 0.3}
  return setmetatable({super = mimas.Window()}, self)
end

function LogWindow:paint(p, w, h)
  --p:setBrush(self.hue)
  p:fillRect(self.hue, 0, 0, w, h)
end
setmetatable(LogWindow, mimas.Window_)

function should.displayWindow(t)
  -- we use the test env to protect from gc
  t.win = LogWindow:new{hue = 0.7}
  t.layout = mimas.HBoxLayout(t.win)
  t.win:move(10, 10)
  t.win:show()
  t.label = mimas.Label("Super window closes in 200ms")
  t.layout:addWidget(t.label)

  t.thread = lk.Thread(function()
    sleep(1200)
    t.win:close()
    assertTrue(true)
  end)
end

test.all()
