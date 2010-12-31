--[[------------------------------------------------------

  Basic tests for rubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('mimas')

local app = mimas.Application()

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