--[[------------------------------------------------------

  Basic tests for lubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'lubyk'
require 'mimas'
local should   = test.Suite('mimas.Slider')
local withUser = should:testWithUser()
withUser.should2 = {}
local should2 = {}

function should.autoload()
  assertTrue(mimas.Slider)
end

function should.acceptDestroyFromGui(t)
  local win = mimas.Window()
  win:resize(200, 200)
  win:show()
  local slider = mimas.Slider(mimas.Horizontal, win)
  sleep(100)
  win = nil
  t:timeout(function(done)
    collectgarbage() -- deletes window --> delete slider (child widget)
    return done or slider:deleted()
  end)
  assertTrue(slider:deleted())
end

function withUser.should.acceptDestroyFromLua(t)
  t.win = mimas.Window()
  t.win:move(100,50)
  t.win:resize(100, 100)
  t.layout = mimas.VBoxLayout(t.win)
  t.label  = mimas.Label("Slider destroyed by Lua in 1s")
  t.layout:addWidget(t.label)
  t.slider = mimas.Slider(mimas.Horizontal)
  t.slider:setValue(0.5)
  t.layout:addWidget(t.slider)
  t.win:show()
  t.thread = lk.Thread(function()
    t.slider = nil
    collectgarbage('collect')
    -- Slider destroyed by Lua
    t.label:setText("Slider should2 be deleted. Click to continue.")
  end)

  function t.win:click()
    t.continue = true
  end

  t:timeout(function(done)
    return done or t.continue
  end)
  assertTrue(t.continue)
  t.win:close()
end

function withUser.should.drawOneSlider(t)
  t.win = mimas.Window()
  t.win:move(100, 150)

  t.layout = mimas.VBoxLayout(t.win)
  t.label  = mimas.Label("Move to zero to close...")
  t.layout:addWidget(t.label)

  t.slider = mimas.Slider(mimas.Horizontal)
  t.slider:setValue(50)
  t.layout:addWidget(t.slider)

  function t.slider:sliderChanged(value)
    -- do something with value change
    self:setValue(value)
    if value == 0 then
      t.continue = true
    end
    t.label:setText(string.format('value: %f', value))
  end

  t.win:show()
  t:timeout(function(done)
    return done or t.continue
  end)
  t.win:close()
  assertTrue(t.continue)
end

function withUser.should.styleSlider(t)
  t.win = mimas.Window()
  t.win:move(100, 250)

  t.layout = mimas.VBoxLayout(t.win)
  local layout = t.layout
  t.label  = mimas.Label("Styling tests...")
  layout:addWidget(t.label)

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
    local d = {}
    t[style_test] = d
    d.label  = mimas.Label(style_test)
    layout:addWidget(d.label)
    d.slider = mimas.Slider(mimas.Horizontal, function(self, value)
      self:setValue(value)
    end)
    layout:addWidget(d.slider)
    d.slider:setStyle(style_test)
    d.slider:setValue(0.3 + math.random() * 0.5)
  end

  t.label2  = mimas.Label("Slider has 'test_name'. Move to change hue (0=>-1).")
  layout:addWidget(t.label2)
  t.slider2 = mimas.Slider(mimas.Horizontal, function(self, value)
    if value == 0 then
      self:setHue(-1)
      self:setValue(0.5)
    else
      self:setHue(value)
      self:setValue(value)
    end
  end)
  t.slider2:setName('test_name')
  t.slider2:setValue(0.5)
  layout:addWidget(t.slider2)

  t.label3  = mimas.Label("Move to change color hue.")
  layout:addWidget(t.label3)
  t.slider3 = mimas.Slider(mimas.Horizontal, function(self, value)
    self:setStyle(string.format('color:hsv(%i, 255, 255)', value * 360))
    self:setValue(value)
  end)
  t.slider3:setValue(80/360)
  layout:addWidget(t.slider3)

  -- Make the slider move
  t.i = 0
  t.timer = lk.Timer(100, function()
    --- BUGS ---
    t.i = t.i + 0.05
    local val = 0.5 + 0.5 * math.sin(t.i)
    -- BUG...
    t.slider3:setStyle(string.format('color:hsv(%i, 255, 255)', val * 360))
    t.slider3:setValue(val)
  end)
  t.timer:start()

  t.win:show()
  function t.win:click()
    t.timer:stop()
    t.continue = true
  end

  t:timeout(function(done)
    return done or t.continue
  end)
  assertTrue(t.continue)
  t.win:close()
end

function withUser.should.syncTwoSliders(t)
  t.win = mimas.Window()
  t.win:move(320, 50)

  t.layout = mimas.HBoxLayout(t.win)
  t.label  = mimas.Label("Move to zero to quit...")
  t.layout:addWidget(t.label)

  t.slider1 = mimas.Slider(mimas.Vertical)
  t.slider1:setValue(50)
  t.layout:addWidget(t.slider1)

  t.slider2 = mimas.Slider(mimas.Vertical)
  t.slider2:setValue(50)
  t.layout:addWidget(t.slider2)

  -- Use function (simplest method)
  function t.slider1:sliderChanged(value)
    if value == 0 then
      t.continue = true
    end
    t.slider2:setValue(value)
  end

  -- Use a callback (enables many listeners)
  t.callback2 = mimas.Callback(function(self, value)
    t.slider1:setValue(value)
    t.label:setText(string.format('value: %f', value))
  end)

  t.callback2:connect(t.slider2, 'sliderChanged(double)')

  t.win:show()
  t:timeout(function(done)
    return done or t.continue
  end)
  t.win:close()
  assertTrue(t.continue)
end

test.all()
