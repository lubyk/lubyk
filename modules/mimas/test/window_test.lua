--[[------------------------------------------------------

  Basic tests for rubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('mimas')

function should.display_window(t)
  -- we use the test env to protect from gc
  t.win = mimas.Window()
  t.layout = mimas.HBoxLayout(t.win)
  t.win:move(100, 100)
  t.win:show()
  t.label = mimas.Label("Window closes in 200ms")
  t.layout:addWidget(t.label)

  t.thread = rk.Thread(function()
    sleep(200)
    t.win:close()
    assert_true(true)
  end)
end

function should.accept_destroy_from_gui(t)
  t.win = mimas.Window()
  t.win:move(100, 170)
  t.win:resize(50, 50)
  t.win:show()

  t.thread = rk.Thread(function()
    sleep(200)
    t.win:close()
    while not t.win:deleted() do
      sleep(200)
    end
    -- should be deleted by GUI
    assert_match('NULL', t.win:__tostring())
  end)
end

function should.accept_destroy_from_Lua()
  local win = mimas.Window()
  win:move(100, 240)
  win:resize(50, 50)
  win:show()
  local label = mimas.Label("Hop", win)

  thread = rk.Thread(function()
    win = nil
    collectgarbage('collect')
    -- not deleted by Lua, but marked as deleted in C++
    -- proof that win was deleted in C++
    assert_true(label:deleted())
  end)
end

test.all()