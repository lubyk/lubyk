--[[------------------------------------------------------

  Basic tests for rubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'rubyk'
require 'mimas'
local should = test.Suite('mimas')
-- until we fix ownership
collectgarbage('stop')
local app = mimas.Application()

local i = 0
local tim_slider
-- cannot create timer in function.. Why ?
local tim = rk.Timer(20, function()
  i = i + 0.05
  local val = 0.5 + 0.5 * math.sin(i)
  app:post(function()
    tim_slider:setStyle(string.format('color:hsv(%i, 255, 255)', val * 360))
    tim_slider:setValue(val)
  end)
end)

function should.draw_one_slider()
  local win = mimas.Widget()
  win:move(100, 100)

  local layout = mimas.VBoxLayout(win)
  local label  = mimas.Label("Move to zero to quit...")
  layout:addWidget(label)

  local slider = mimas.Slider(mimas.Horizontal)
  slider:setValue(50)
  layout:addWidget(slider)

  local callback = mimas.Callback(function(value)
    -- do something with value change
    if value == 0 then
      app:quit()
    end
  end)

  -- callback listens for quit_btn's clicked events.
  callback:connect(slider, 'valueChanged(double)')

  local callback2 = mimas.Callback(function(value)
    label:setText(string.format('value: %f', value))
  end)
  -- callback listens for quit_btn's clicked events.
  callback2:connect(slider, 'valueChanged(double)')

  win:show()
end

function should.style_slider()
  local win = mimas.Widget()
  win:move(100, 200)

  local layout = mimas.VBoxLayout(win)
  local label  = mimas.Label("Styling tests...")
  layout:addWidget(label)

  -- can use rgb(), rgba(), hsv(), hsva() or #00FA88 (caps)
  -- color = slider value (WindowText)
  -- background = background (Window)
  -- alternate-background-color = border color (AlternateBase)
  -- selection-background-color: not used (Highlighted)
  -- selection-color: slider button (HighlightedText)


  -- text / back / select
  local tests = {
    'color:hsv(30,200,200)',
    'background:hsv(30,200,200)',
    'alternate-background-color:hsv(30,200,200)',
    'selection-color:hsv(30,200,200)',
    'qproperty-border:5',
    'qproperty-hue:188',
  }

  for _, style_test in ipairs(tests) do
    local label  = mimas.Label(style_test)
    layout:addWidget(label)
    local slider = mimas.Slider(mimas.Horizontal, function(self, value)
      self:setValue(value)
    end)
    layout:addWidget(slider)
    slider:setStyle(style_test)
    slider:setValue(0.3 + math.random() * 0.5)
  end

  local label  = mimas.Label("Slider has 'test_name'. Move to change hue (0=>-1).")
  layout:addWidget(label)
  local slider = mimas.Slider(mimas.Horizontal, function(self, value)
    if value == 0 then
      self:setHue(-1)
      self:setValue(0.5)
    else
      self:setHue(value * 360)
      self:setValue(value)
    end
  end)
  slider:setName('test_name')
  slider:setValue(0.5)
  layout:addWidget(slider)

  local label  = mimas.Label("Move to change color hue.")
  layout:addWidget(label)
  tim_slider = mimas.Slider(mimas.Horizontal, function(self, value)
    self:setStyle(string.format('color:hsv(%i, 255, 255)', value * 360))
    self:setValue(value)
  end)
  tim_slider:setValue(80/360)
  layout:addWidget(tim_slider)
  tim:start()
  win:show()
end

function should.sync_two_sliders()
  local win = mimas.Widget()
  win:move(300, 100)

  local layout = mimas.HBoxLayout(win)
  local label  = mimas.Label("Move to zero to quit...")
  layout:addWidget(label)

  local slider1 = mimas.Slider(mimas.Vertical)
  slider1:setValue(50)
  layout:addWidget(slider1)

  local slider2 = mimas.Slider(mimas.Vertical)
  slider2:setValue(50)
  layout:addWidget(slider2)

  local callback1 = mimas.Callback(function(value)
    if value == 0 then
      app:quit()
    end
    slider2:setValue(value)
  end)

  local callback2 = mimas.Callback(function(value)
    slider1:setValue(value)
    label:setText(string.format('value: %f', value))
  end)

  -- callback listens for quit_btn's clicked events.
  callback1:connect(slider1, 'valueChanged(double)')

  -- callback listens for quit_btn's clicked events.
  callback2:connect(slider2, 'valueChanged(double)')

  win:show()
end

app:post(function()
  test.all()
end)

app:exec()