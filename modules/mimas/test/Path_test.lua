--[[------------------------------------------------------

  Basic tests for lubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.Painter')
local withUser = should:testWithUser()

function should.autoload()
  assertTrue(mimas.Path)
end

local function makePath(cx, cy, r)
  local path = mimas.Path()
  local STEPS = 50
  local STEP_SIZE = 2 * math.pi / STEPS
  local dx
  local dy
  for i=0,STEPS do
    local t = 2 * math.pi * i / STEPS
    local x = cx + r * math.cos(3*t)
    local y = cy + r * math.sin(5*t)
    if t == 0 then
      path:moveTo(x, y)
    else
      local dx = 25 - 50 * math.random()
      local dy = 25 - 50 * math.random()
      path:cubicTo(
        x-dx, y+dy,
        x+dx, y-dy,
        x, y
      )
    end
  end
  return path
end

function withUser.should.drawPath(t)
  t.win = mimas.Window()
  t.label = mimas.Label('Click on purple color')
  t.win:addWidget(t.label)
  t.label:move(10, 10)
  t.win:move(10, 10)
  t.win:resize(400, 400)
  t.win:show()
  t.x = 0
  t.y = 0

  function t.win:paint(p, w, h)
    p:setBrush(0.9, 1, 1, 0.3)
    p:setPen(0.5, mimas.Color(0.3, 1, 1))
    t.path = makePath(200, 200, 100)
    p:drawPath(t.path)
    self:update()
  end

  function t.win:click(x, y)
    if t.path:contains(x, y) then
      t.continue = true
    end
  end

  t.win:update()
  t:timeout(function(done)
    return done or t.continue
  end)
  t.win:close()
  assertTrue(t.continue)
end

function should.respondToOutlineForWidth()
  local p = mimas.Path()
  p:moveTo(0,0)
  p:lineTo(0,10)
  local outline = p:outlineForWidth(5)
  assertTrue(outline:contains(0, 5))
  assertTrue(outline:contains(2, 5))
  assertFalse(outline:contains(3, 5))
end

test.all()
