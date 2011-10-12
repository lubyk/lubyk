--[[------------------------------------------------------

  Run this file along with slider_saturn.lua
  --------------------------------------------

  zmq + mDNS (Basic test for GUI / process sync)

--]]------------------------------------------------------
require 'lubyk'
math.randomseed(os.time())
math.random()

app = mimas.Application()
win = mimas.Window()
win:move(10, 10)
win:resize(200, 300)

mlayout = mimas.VBoxLayout(win)
label   = mimas.Label("Start 'Venus' service")

mlayout:addWidget(label)

quit_btn = mimas.Button('Quit', function()
  sched:quit()
end)
mlayout:addWidget(quit_btn)


slider = mimas.Slider(mimas.Vertical)
slider:setValue(50)
slider:setHue(math.random())
mlayout:addWidget(slider)

value = 0

client = lk.Client(function(val)
  value = val
  slider:setValue(val)
end)

-- Mimas listens to messages from planet Saturn
client:subscribe('Saturn')

function slider:sliderChanged(val)
  if val ~= value then
    value = val
    if value == 0 then
      -- quit remote
      client:send('Saturn', 'Quit')
    else
      -- send value to 'Saturn' service
      --print(client:request('Saturn', value))
      local ok, err = client:send('Saturn', value)
      if not ok then
        print(err)
      end
    end
  end
end

win:show()
run()
