--[[------------------------------------------------------

  mimas.Color
  -----------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.Color')
local withUser = should:testWithUser()

function makeColors()
  collectgarbage('collect')
  local colors = {}
  for i = 1,200 do
    table.insert(colors, mimas.Color(i/200))
  end

  for i = 1,200 do
    table.insert(colors, colors[i]:colorWithAlpha(0.5))
  end
end

function should.create_many_colors_and_gc(t)
  t.win = mimas.Window()
  -- warmup
  makeColors()

  collectgarbage('collect')
  local before = collectgarbage('count')
  makeColors()

  collectgarbage('collect')
  local after = collectgarbage('count')
  if testing_gui then
    -- with other tests messing around, we have to be more tolerant
    assertLessThen(before * 1.01, after)
  else
    -- when running the test alone
    assertEqual(before, after)
  end
  t.win:show()
  t.win:close()
end

function should.createNewColor()
  local color = mimas.Color(0.54, 0.11, 0.22, 0.33)
  local other = color:colorWithHue(0.32)
  assertInRange(0.31999, 0.32001, other:hue())
  assertInRange(0.10999, 0.11001, other:saturation())
  assertInRange(0.21999, 0.22001, other:value())
  assertInRange(0.32999, 0.33001, other:alpha())
  assertNotEqual(color, other)
end

local Col = lk.SubClass(mimas, 'Widget')
function Col:init(name, color)
  self.name  = name
  self.color = color
end

function Col:paint(p, w, h)
  p:fillRect(0, 0, w, h, self.color)
  if self.name == 'white' then
    p:setPen(mimas.blackPen)
  else
    p:setPen(mimas.WhitePen)
  end
  p:drawText(0, 0, w, h, mimas.AlignCenter, self.name)
end

function Col:click()
  -- pass click to parent
  return false
end

function withUser.should.createConstantColors(t)
  t.win = mimas.Window()
  t.win:move(10,10)
  t.win:resize(200,340)
  t.lay = mimas.VBoxLayout(t.win)
  t.col = {}
  for k, color in pairs(mimas.colors) do
    t.col[k] = Col(k, color)
    t.lay:addWidget(t.col[k])
  end
  t.win:show()
  function t.win:click()
    t.continue = true
  end
  t:timeout(function(done)
    return done or t.continue
  end)
  assertTrue(t.continue)
  t.win:close()
end
test.all()
