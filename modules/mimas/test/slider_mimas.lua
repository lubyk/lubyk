require 'rubyk'

app = mimas.Application()
local win = mimas.Widget()
win:move(100, 400)

local layout = mimas.VBoxLayout(win)
local label  = mimas.Label("Start 'Venus' service")
layout:addWidget(label)

local slider = mimas.Slider(mimas.Vertical)
slider:setValue(50)
layout:addWidget(slider)

local value = nil

local service = rk.Service('Mimas', rubyk.service_type, function(self, val)
  print('Mimas <---', val)
  value = val
  slider:setValue(val)
end, 7020)

-- Mimas listens to messages from planet Saturn
service:connect('Saturn')

local callback = mimas.Callback(function(val)
  if val ~= value then
    if val == 1.0 then
      app:post(function()
        app:quit()
      end)
    end
    value = val
    print('Mimas --->', value)
    service:send('Saturn', value)
  end
end)

-- callback listens for slider's valueChanged and
-- sends notifications
callback:connect(slider, 'valueChanged(double)')
-- connected becomes true when 'Mars' appears on the network
while not service:connected() do
  worker:sleep(100) -- make sure everything is ready before sending
end
print("Connected")
win:show()
app:exec()