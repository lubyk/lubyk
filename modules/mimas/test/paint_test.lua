--[[------------------------------------------------------

  Basic tests for lubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.Painter')

function should.load_constants()
  assert_equal(1, mimas.SolidPattern)
end

function should.paint_path(t)
  -- we use the test env to protect from gc
  t.win = mimas.Window()
  t.win:move(300, 300)
  t.win:resize(100, 100)
  function t.win.paint(p, w, h)
    local box_padding = 5
    local path = mimas.Path()
    local hpen_width = 5 -- half pen width
    local bp = hpen_width + box_padding -- full box padding
    local arc_radius = 30
    local text_padding = 20
    path:moveTo(w, bp)
    path:lineTo(bp + arc_radius, bp)
    path:cubicTo(bp, bp, bp, bp, bp, bp + arc_radius)
    path:lineTo(bp, h - bp - arc_radius)
    path:cubicTo(bp, h - bp, bp, h - bp, bp + arc_radius, h - bp)
    path:lineTo(w, h - bp)
    p:setBrush(mimas.Color(0.5, 0.5, 0.5))
    p:setPen(mimas.Pen(hpen_width * 2, mimas.Color(0.2, 0.8, 0.8, 0.8)))
    p:drawPath(path)
    p:drawText(text_padding, text_padding, w - 2*text_padding, h - 2*text_padding, mimas.AlignCenter, 'Saturn')
  end
  t.win:show()

  t.thread = lk.Thread(function()
    sleep(4400)
    t.win:close()
    assert_true(true)
  end)
end

function should.accept_destroy_from_gui(t)
  t.win = mimas.Window()
  t.win:move(100, 170)
  t.win:resize(50, 50)
  t.win:show()

  t.thread = lk.Thread(function()
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

  thread = lk.Thread(function()
    win = nil
    collectgarbage('collect')
    -- not deleted by Lua, but marked as deleted in C++
    -- proof that win was deleted in C++
    assert_true(label:deleted())
  end)
end

test.all()