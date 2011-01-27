require('lubyk')

local app = mimas.Application()
local win = mimas.Widget()
win:resize(320, 240)

local layout = mimas.HBoxLayout(win)
local label  = mimas.Label("Mimas app...")
layout:addWidget(label)

local quit_btn = mimas.PushButton("Quit")

local callback = mimas.Callback(function()
  app:quit()
end)

-- callback listens for quit_btn's clicked events.
callback:connect(quit_btn, 'clicked')

layout:addWidget(quit_btn)

local counter = 0
-- execute 100ms after app started
local timer = rk.Timer(1000, function()
  counter = counter + 1
  app:post(function()
    label:setText(string.format("count: %i", counter))
  end)
  -- continue until 'timer' is gc or stopped.
  if counter > 10 then
    app:post(function()
      app:quit()
    end)
  end
end)
timer:start()
win:show()
app:exec()