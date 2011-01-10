--[[------------------------------------------------------

  Basic tests for OpenGL
  ----------------------

  ...

--]]------------------------------------------------------
require 'rubyk'
require 'gl'
local should = test.Suite('gl (TODO)')

function should.load_gl()
  assert_true(gl)
end

--function should.accept_destroy_from_gui(t)
--  t.win = mimas.GLWindow()
--  t.win:move(100, 170)
--  t.win:resize(50, 50)
--  t.win:show()
--
--  t.thread = rk.Thread(function()
--    sleep(200)
--    t.win:close()
--    while not t.win:deleted() do
--      sleep(200)
--    end
--    -- should be deleted by GUI
--    assert_match('NULL', t.win:__tostring())
--  end)
--end
--
--function should.accept_destroy_from_Lua()
--  local win = mimas.GLWindow()
--  win:move(100, 240)
--  win:resize(50, 50)
--  win:show()
--  local label = mimas.Label("Hop", win)
--
--  thread = rk.Thread(function()
--    win = nil
--    collectgarbage('collect')
--    -- not deleted by Lua, but marked as deleted in C++
--    -- proof that win was deleted in C++
--    assert_true(label:deleted())
--  end)
--end

test.all()