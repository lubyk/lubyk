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
local LogWindow = lk.SubClass(mimas, 'Window')

function LogWindow:init(hue)
  self.hue = hue
end

function LogWindow:paint(p, w, h)
  p:fillRect(0, 0, w, h, mimas.Color(self.hue))
end

function should.displayWindow(t)
  t.win = LogWindow(0.7)
  t.layout = mimas.HBoxLayout(t.win)
  t.win:move(100, 100)
  t.win:resize(100, 100)
  t.win:show()
  function t.win:click()
    t.continue = true
  end

  t.label = mimas.Label("Super window. Click to close.")
  t.layout:addWidget(t.label)

  t:timeout(500, function(done)
    return done or t.continue
  end)
  t.win:close()
  assertTrue(true)
end

test.all()
