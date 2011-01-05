require 'rubyk'
--------------- /////////////////  FIXME: Mimas Widgets should not be GC (they are gc by app).
collectgarbage('stop') -- avoid problems until we fix

app = mimas.Application()
win = mimas.Window()
win:move(100, 400)

mlayout = mimas.VBoxLayout(win)
label   = mimas.Label("Start 'Venus' service")

mlayout:addWidget(label)


slider = mimas.Slider(mimas.Vertical)
slider:setValue(50)
mlayout:addWidget(slider)

value = 0

client = rk.Client(function(val)
  print('Mimas <---', val)
  value = val
  slider:setValue(val)
end)

sleep(100)
-- Mimas listens to messages from planet Saturn
client:subscribe('Saturn')

callback = mimas.Callback(function(val)
  -- move app into local env until _G is properly shared in sub func
--  local app = app
  if val ~= value then
    if val == 1.0 then
      app:post(function()
        app:quit()
      end)
    end
    value = val
    print('Mimas --->', value)
    -- client not created in this thread, let's hope zmq handles this
    client:send('Saturn', value)
  end
end)

-- callback listens for slider's valueChanged and
-- sends notifications
callback:connect(slider, 'valueChanged(double)')

-- connected becomes true when 'Mars' appears on the network
while not client:connected() do
  sleep(100) -- make sure everything is ready before sending
end
print("Connected")
win:show()
app:exec()