--[[------------------------------------------------------

  test mDNS for Lua
  -----------------

  mDNS implementation with callbacks to integrate with
  rubyk.

--]]------------------------------------------------------
require 'rubyk'

--local should = test.Suite('mimas')
--
--function should.create_empty_window()
  local app = mimas.Application()
  local win = mimas.Widget()
  win:resize(320, 240)

  local layout = mimas.HBoxLayout(win)
  --local label  = mimas.Label("Hello Mimas!")
  --layout:addWidget(label)

  local quit_btn = mimas.PushButton("Quit")

  -- this is the only difference with Qt:
  -- instead of using signals/slots, we use Lua
  -- callbacks (will make the whole thing easier
  -- and more portable).
  local callback = mimas.Callback(function()
    print("quit ?")
    app:quit()
  end)

  -- callback listens for quit_btn's clicked events.
  callback:connect(quit_btn, '2clicked()')

  layout:addWidget(quit_btn)

  local browser = mdns.Browser(rubyk.service_type, function(service)
    label:setText(service.name)
  end)

  win:show()
  app:exec()
--end

--test.all()