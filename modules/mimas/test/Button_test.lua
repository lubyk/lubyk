--[[------------------------------------------------------

  Basic tests for lubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.Button')
local withUser = should:testWithUser()

function withUser.should.connectToCallback(t)
  t.win = mimas.Window()
  t.win:move(100, 100)
  t.win:resize(200,200)

  t.btn = mimas.Button("click me", t.win)
  t.callback = mimas.Callback(function()
    t.continue = true
  end)

  t.callback:connect(t.btn, 'clicked')
  t.win:show()
  t:timeout(function(done)
    if done or t.continue then
      t.win:close()
      assertTrue(t.continue)
      return true -- done
    end
  end)
end

function withUser.should.createWithFunction(t)
  t.win = mimas.Window()
  t.win:move(200, 200)
  t.win:resize(200,200)
  t.lay = mimas.HBoxLayout(t.win)
  t.btn = mimas.Button("click me", function()
    t.continue = true
  end)
  collectgarbage('collect')
  -- should not remove the callback because it is saved with
  -- the button's env
  t.lay:addWidget(t.btn)
  t.win:show()
  t:timeout(function(done)
    if done or t.continue then
      t.win:close()
      assertTrue(t.continue)
      return true
    end
  end)
end

function should.styleButtons(t)
  t.win = mimas.Window()
  t.win:move(100, 400)
  t.win:resize(200,200)
  t.lay = mimas.VBoxLayout(t.win)

  -- can use rgb(), rgba(), hsv(), hsva() or #00FA88 (caps)
  local tests = {
    '.button {color:#EAA844}',
    '.button {border: 3px solid #EAA844; border-radius:5}',
    '.button {background-color: hsva(80, 255, 255, 40%)}',
    '.button:hover {background:#EAA844}'
  }

  for _, style_test in ipairs(tests) do
    local lbl = mimas.Button(string.format("setStyle: %s", style_test))
    t.lay:addWidget(lbl)
    lbl:setStyleSheet(style_test)
    -- avoid GC
    t[style_test] = lbl
  end
  t.win:show()
  t.timeout = lk.Thread(function()
    sleep(3000)
    t.win:close()
  end)
end

test.all()
