--[[------------------------------------------------------

  Basic tests for rubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('mimas')

function should.post_to_gui_thread()
  local app = mimas.Application()
  local win = mimas.Widget()
  local quit_called = false
  win:resize(320, 240)

  local layout = mimas.VBoxLayout(win)
  local label  = mimas.Label("Hello Mimas!")
  layout:addWidget(label)

  local quit_btn = mimas.PushButton("Quit")

  -- this is the only difference with Qt:
  -- instead of using signals/slots, we use Lua
  -- callbacks (will make the whole thing easier
  -- and more portable).
  local callback = mimas.Callback(function()
    quit_called = true
    app:quit()
  end)

  -- callback listens for quit_btn's clicked events.
  callback:connect(quit_btn, 'clicked')

  layout:addWidget(quit_btn)

  local browser = mdns.Browser(rubyk.service_type, function(service)
    -- FIXME: we are not in the GUI thread !
    -- FIXME: how do we do this safely ?
    app:post(function()
      label:setText(service.name)
    end)
  end)

  local counter = 0
  -- execute 100ms after app started
  local timer = rk.Timer(100, function()
    counter = counter + 1
    -- FIXME: we are not in the GUI thread !
    -- FIXME: how do we do this safely ?
    -- Is this a good solution ?
    -- This implies
    -- 1. An signal that emits a Callback pointer.
    -- 2. A slot that executes the callback and deletes.
    -- ==> Create/delete the callback all in C++
    app:post(function()
      label:setText(string.format("count: %i", counter))
    end)
    if counter >= 3g then
      app:quit()
      return 0
    end
    -- continue until 'timer' is gc or stopped.
  end)
  timer:start()
  win:show()
  app:exec()
  assert_true(quit_called or counter >= 3)
end

test.all()