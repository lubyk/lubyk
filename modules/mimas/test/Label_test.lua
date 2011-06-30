--[[------------------------------------------------------

  Basic tests for lubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.Label')


function should.accept_destroy_from_gui()
  local win = mimas.Window()
  win:resize(50, 50)
  win:show()
  local label = mimas.Label("Hop", win)

  thread = lk.Thread(function()
    win = nil
    collectgarbage('collect')
    -- not deleted by Lua, but marked as deleted in C++
    assertTrue(label:deleted())
  end)
end

function should.accept_destroy_from_Lua(t)
  t.win = mimas.Window()
  t.win:move(100,50)
  t.layout = mimas.HBoxLayout(t.win)
  local label = mimas.Label("Label destroyed by Lua in 1s")
  t.layout:addWidget(label)
  t.win:show()
  t.thread = lk.Thread(function()
    sleep(1000)
    label = nil
    collectgarbage('collect')
    -- Label destroyed by Lua
    sleep(1000)
    assertTrue(t.win:close()) -- visual feedback needed..
  end)
end


function should.style_labels(t)
  t.win = mimas.Window()
  t.win:move(100, 120)
  t.lb = mimas.Label("Label not in layout", t.win)
  t.lb:move(180, 0)
  t.lb:resize(180, 20)

  t.lay = mimas.VBoxLayout(t.win)
  local lay = t.lay
  t.lbl1 = mimas.Label("Label in layout")
  t.lay:addWidget(t.lbl1)

  -- can use rgb(), rgba(), hsv(), hsva() or #00FA88 (caps)
  local tests = {
    'color:hsva(180, 255, 255, 20%)',
    'border: 2px solid hsv(80, 255, 255)',
    'background: hsva(80, 255, 255, 40%)',
    'padding:15px; background: hsva(70, 255, 255, 30%); border: 1px solid blue'
  }

  for _, style_test in ipairs(tests) do
    local lbl = mimas.Label(string.format("setStyle: %s", style_test))
    lay:addWidget(lbl)
    lbl:setStyle(style_test)
    -- avoid GC
    t[style_test] = lbl
  end

  t.lbl2 = mimas.Label("setHue")
  lay:addWidget(t.lbl2)
  t.lbl2:setHue(210 / 360)

  t.lbl3 = mimas.Label("name = test_name")
  t.lbl3:setName("test_name")
  lay:addWidget(t.lbl3)

  -- visual check
  assertTrue(true)
  t.win:show()
  t.thread = lk.Thread(function()
    sleep(2000)
    t.win:close()
  end)
end

test.all()