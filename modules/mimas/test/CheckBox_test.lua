--[[------------------------------------------------------

  mimas.CheckBox test
  -------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.CheckBox')
local withUser = should:testWithUser()

function withUser.should.connectToCallback(t)
  t.win = mimas.Window()
  t.win:move(100, 100)
  t.win:resize(200,200)

  t.btn = mimas.CheckBox("click me", t.win)
  t.callback = mimas.Callback(function()
    t.continue = true
  end)

  t.btn2 = mimas.CheckBox('ignore me')
  t.btn2:setCheckable(false)
  t.win:addWidget(t.btn2)
  t.btn2:move(40,40)
  t.callback:connect(t.btn, 'clicked')
  t.win:show()
  -- t:test {
  --   -- This is the action that is triggered when the test is run in batch
  --   -- (not just this file). This is also what we want the user to do for
  --   -- testing.
  --   action   = mimas.AutoPilot:click(t.btn),
  --   -- What exactly are we expecting
  --   expected = function() return t.continue end
  -- }
  t:timeout(function()
    return t.continue
  end)
  t.win:close()
  assertTrue(t.continue)
end

function withUser.should.createWithFunction(t)
  t.win = mimas.Window()
  t.win:move(200, 200)
  t.win:resize(200,200)
  t.lay = mimas.HBoxLayout(t.win)
  local btn = mimas.CheckBox("click me too", function(b, state)
    t.continue = not state
  end)
  btn:setChecked(true)
  collectgarbage('collect')
  -- should not remove the callback because it is saved with
  -- the button's env
  t.lay:addWidget(btn)
  t.win:show()
  t:timeout(function()
    return t.continue
  end)
  t.win:close()
  assertTrue(t.continue)
end

function should.styleCheckBoxs(t)
  t.win = mimas.Window()
  t.win:move(100, 400)
  t.win:resize(200,200)
  t.lay = mimas.VBoxLayout(t.win)

  -- can use rgb(), rgba(), hsv(), hsva() or #00FA88 (caps)
  local tests = {
    '.checkbox {color:#EAA844}',
    '.checkbox {border: 3px solid #EAA844; border-radius:5}',
    '.checkbox {background-color: hsva(80, 255, 255, 40%)}',
    '.checkbox:hover {background:#EAA844}'
  }

  for _, style_test in ipairs(tests) do
    local lbl = mimas.CheckBox(string.format("setStyle: %s", style_test))
    t.lay:addWidget(lbl)
    lbl:setStyleSheet(style_test)
    -- avoid GC
    t[style_test] = lbl
  end
  t.win:show()
  t.timeout = lk.Thread(function()
    sleep(30000)
    t.win:close()
  end)
end

test.all()

