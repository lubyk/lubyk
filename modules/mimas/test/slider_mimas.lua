--[[------------------------------------------------------

  Run this file along with slider_saturn.lua
  --------------------------------------------

  zmq + mDNS (Basic test for GUI / process sync)

--]]------------------------------------------------------
require 'lubyk'

app = mimas.Application()
win = mimas.Window()
win:move(10, 10)

mlayout = mimas.VBoxLayout(win)
label   = mimas.Label("Start 'Venus' service")

mlayout:addWidget(label)

quit_btn = mimas.PushButton('Quit', function()
  app:quit()
end)
mlayout:addWidget(quit_btn)


slider = mimas.Slider(mimas.Vertical)
slider:setValue(50)
mlayout:addWidget(slider)

value = 0

client = lk.Client(function(val)
  value = val
  slider:setValue(val)
end)

sleep(100)
-- Mimas listens to messages from planet Saturn
client:subscribe('Saturn')

callback = mimas.Callback(function(val)
  if val ~= value then
    value = val
    if value == 0 then
      -- quit remote
      client:send('Saturn', nil)
    else
      -- send value to 'Saturn' service
      local ok, err = client:send('Saturn', value)
      if not ok then
        print(err)
      end
    end
  end
end)

-- callback listens for slider's valueChanged and
-- sends notifications
callback:connect(slider, 'valueChanged(double)')

win:show()
app:exec()