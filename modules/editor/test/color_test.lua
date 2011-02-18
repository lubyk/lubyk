--[[------------------------------------------------------

  mimas.Color
  -----------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.Color')

function make_colors()
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
  make_colors()

  collectgarbage('collect')
  local before = collectgarbage('count')
  make_colors()

  collectgarbage('collect')
  local after = collectgarbage('count')
  if testing_gui then
    -- with other tests messing around, we have to be more tolerant
    assert_less_then(before * 1.01, after)
  else
    -- when running the test alone
    assert_equal(before, after)
  end
  t.win:show()
  t.win:close()
end

function should.create_new_color()
  local color = mimas.Color(0.54, 0.11, 0.22, 0.33)
  local other = color:colorWithHue(0.32)
  assert_in_range(0.31999, 0.32001, other:hue())
  assert_in_range(0.10999, 0.11001, other:saturation())
  assert_in_range(0.21999, 0.22001, other:value())
  assert_in_range(0.32999, 0.33001, other:alpha())
  assert_not_equal(color, other)
end

local Col = mimas.WidgetClass()
function Col:init(name, color)
  self.name  = name
  self.color = color
end

function Col:paint(p, w, h)
  p:fillRect(0, 0, w, h, self.color)
  if self.name == 'white' then
    p:setPen(mimas.blackPen)
  else
    p:setPen(mimas.whitePen)
  end
  p:drawText(0, 0, w, h, mimas.AlignCenter, self.name)
end

function should.create_constant_colors(t)
  t.win = mimas.Window()
  t.win:resize(200,340)
  t.lay = mimas.VBoxLayout(t.win)
  t.col = {}
  for k, color in pairs(mimas.colors) do
    t.col[k] = Col(k, color)
    t.lay:addWidget(t.col[k])
  end
  t.timer = lk.Thread(function()
    sleep(3000)
    t.win:close()
  end)
  t.win:show()
  assert_true(true)
end
test.all()