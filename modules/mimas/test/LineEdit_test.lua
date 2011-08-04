--[[------------------------------------------------------

  Basic tests for LineEdit
  ------------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.LineEdit')

local pos = 10
local function stack(win)
  win:move(10, pos)
  win:resize(200, 50)
  pos = pos + 100
end

function should.acceptDestroyFromGui()
  local win = mimas.Window()
  stack(win)
  win:show()
  local edit = mimas.LineEdit("Hop", win)
  thread = lk.Thread(function()
    win = nil
    collectgarbage('collect')
    -- not deleted by Lua, but marked as deleted in C++
    assertTrue(edit:deleted())
  end)
end

function should.acceptDestroyFromLua(t)
  t.win = mimas.Window()
  stack(t.win)
  t.layout = mimas.HBoxLayout(t.win)
  local edit = mimas.LineEdit("LineEdit destroyed by Lua in 1s")
  t.layout:addWidget(edit)
  t.win:show()
  t.thread = lk.Thread(function()
    sleep(1000)
    edit = nil
    collectgarbage('collect')
    -- LineEdit destroyed by Lua
    sleep(1000)
    t.win:close()
    assertTrue(true) -- visual feedback needed..
  end)
end

function should.callback(t)
  t.win = mimas.Window()
  stack(t.win)
  t.lb = mimas.LineEdit("Change this to close")
  t.lb:selectAll()
  t.lay = mimas.VBoxLayout(t.win)
  t.lay:addWidget(t.lb)
  function t.lb.editingFinished(text)
    t.lb:setText(string.format("You wrote '%s'.", text))
    t.win:close()
  end

  -- visual check
  assertTrue(true)
  t.win:show()
  t.thread = lk.Thread(function()
    sleep(2000)
    t.win:close()
  end)
end

function should.respondToText()
  lb = mimas.LineEdit("Hello Mimas")
  assertEqual("Hello Mimas", lb:text())
end

function should.respondToSetText()
  lb = mimas.LineEdit("Hello Mimas")
  lb:setText("Goodbye Mimas")
  assertEqual("Goodbye Mimas", lb:text())
end

function should.haveKeyboardCallback(t)
  t.win = mimas.Window()
  stack(t.win)
  t.lb = mimas.LineEdit("type up key to close")
  t.lb:selectAll()
  t.lay = mimas.VBoxLayout(t.win)
  t.lay:addWidget(t.lb)
  function t.lb.keyboard(key, on)
    if key == mimas.Key_Up then
      t.lb:setText('UP')
      t.win:close()
    else
      return false
    end
  end
  t.win:show()
  t.thread = lk.Thread(function()
    sleep(10000)
    t.win:close()
  end)
end

function should.haveClickCallback(t)
  t.win = mimas.Window()
  stack(t.win)
  t.lb = mimas.LineEdit("click to close")
  t.lb:selectAll()
  t.lay = mimas.VBoxLayout(t.win)
  t.lay:addWidget(t.lb)
  function t.lb.click(x, y, type, btn, mod)
    t.win:close()
  end
  t.win:show()
  t.thread = lk.Thread(function()
    sleep(10000)
    t.win:close()
  end)
end

function should.styleLineEdits(t)
  t.win = mimas.Window()
  stack(t.win)
  t.lb = mimas.LineEdit("LineEdit not in layout", t.win)
  t.lb:move(180, 0)
  t.lb:resize(180, 20)

  t.lay = mimas.VBoxLayout(t.win)
  local lay = t.lay
  t.lbl1 = mimas.LineEdit("LineEdit in layout")
  t.lay:addWidget(t.lbl1)

  -- can use rgb(), rgba(), hsv(), hsva() or #00FA88 (caps)
  local tests = {
    'color:hsva(180, 255, 255, 20%)',
    'border: 2px solid hsv(80, 255, 255)',
    'background: hsva(80, 255, 255, 40%)',
    'padding:15px; background: hsva(70, 255, 255, 30%); border: 1px solid blue'
  }

  for _, style_test in ipairs(tests) do
    local lbl = mimas.LineEdit(string.format("setStyle: %s", style_test))
    lay:addWidget(lbl)
    lbl:setStyle(style_test)
    -- avoid GC
    t[style_test] = lbl
  end

  t.lbl2 = mimas.LineEdit("setHue")
  lay:addWidget(t.lbl2)
  t.lbl2:setHue(210 / 360)

  t.lbl3 = mimas.LineEdit("name = test_name")
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

print('done')