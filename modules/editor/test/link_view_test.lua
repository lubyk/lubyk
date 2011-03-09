--[[------------------------------------------------------

  editor.SlotView test
  --------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.SlotView')

local function mock_slot(hue)
  return {node = {color = mimas.Color(hue, 0.3, 0.8, 0.8)}}
end

-- editor needs a 'main' state with currently selected objects
-- and such things
editor.main = {sdragging = {}}

local sz = 600
function should.draw_slot(t)
  t.win = mimas.Window()
  t.win:move(100, 100)
  t.win:resize(sz, sz)

  -- outlet
  t.oview = editor.SlotView(mock_slot(0.2))
  t.win:addWidget(t.oview)
  t.oview:move(2*sz/3, 2*sz/3)

  -- inlet
  t.iview = editor.SlotView(mock_slot(0.9))
  t.win:addWidget(t.iview)
  t.iview:move(sz/2, sz/2)

  t.link = editor.LinkView(t.oview, t.iview)
  t.win:addWidget(t.link)
  t.link:slotMoved()
  t.win:show()
  function t.win.mouse(x,y)
    t.oview:move(x-30, y-30)
    t.link:slotMoved()
  end
  --local i = 0
  --local r = sz/10
  --t.timer = lk.Timer(30, function()
  --  i = i + 0.05
  --  t.oview:move(sz/2 + r * math.cos(1.5*i), sz/2 + r * math.sin(i))
  --  t.link:slotMoved()
  --
  --  app:post(function()
  --    t.win:update()
  --  end)
  --end)
  --t.timer:start()

  t.thread = lk.Thread(function()
    sleep(20000)
    --r = sz/5
    sleep(5000)
    t.win:close()
    assert_true(true)
  end)
end

test.gui()
