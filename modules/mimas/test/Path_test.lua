--[[------------------------------------------------------

  Basic tests for lubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.Painter')

function should.load()
  assertEqual(1, mimas.Path)
end

local function makePath(t, s, x, y)
  local path = mimas.Path()
  local d = s * 0.5
  local x1, y1 = s * t, s * (1-t)
  local dx, dy = 1/(0.01+math.abs(x1-x)), 1/(0.01+math.abs(y1-y))
  print(dx, dy)
  local x2, y2 = x1 + dx, y1 + dy
  path:moveTo(x1, y1)
  path:cubicTo(
    x1-dx, y1+dy,
    x2+dx, y2-dy,
    x2, y2
  )
  return path
end

function should.drawPath(t)
  t.win = mimas.Window()
  t.win:move(10, 10)
  t.win:resize(400, 400)
  t.win:show()
  t.x = 0
  t.y = 0

  function t.win.paint(p, w, h)
    p:setPen(0.5, mimas.Color(0.3, 1, 1))
    for i=0,1,0.01 do
      local path = makePath(i, 200, t.x, t.y)
      p:drawPath(path)
    end
  end

  function t.win.mouse(x, y)
    t.x = x
    t.y = y
    t.win:update()
  end

  function t.win.click(x, y)
    if t.outline:contains(x, y) then
      t.win:close()
    end
  end

  t.win:update()
  t.thread = lk.Thread(function()
    sleep(22000)
    t.win:close()
    assertTrue(true)
  end)
end

test.all()