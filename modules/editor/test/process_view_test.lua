--[[------------------------------------------------------

  editor.ProcessView test
  -----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

-- editor needs a 'main' state with currently selected objects
-- and such things. FIXME: remove.
editor.main = {}

local should = test.Suite('editor.ProcessView')

function mock_process()
  local process = {
    name          = 'Dummy ProcessView',
    hue           = 0.3,
    nodes         = {},
    pending_nodes = {},
  }
  setmetatable(process, editor.Process)
  return process
end

local should2 = {}

function should2.draw_process_view(t)
  t.view = editor.ProcessView(mock_process())
  t.view:move(100, 100)
  t.view:show()

  t.thread = lk.Thread(function()
    sleep(2000)
    t.view:setName('Home')
    sleep(2000)
    t.view:close()
    assert_true(true)
  end)
end

local function mock_node(name, hue, x, y, links)
  local node = {
    name = name,
    hue  = hue,
    x    = x,
    y    = y,
    inlets = {
      in1  = {},
      in2  = {},
    },
    outlets = {
      out1 = {},
      out2 = {},
      out3 = {},
    },
    links = links,
  }
  return node
end

function should.draw_nodes_in_process_view(t)
  local process = mock_process()
  t.view = editor.ProcessView(process)
  t.view:move(100, 100)
  t.view:show()

  t.thread = lk.Thread(function()
    sleep(500)
    process:update {
      a = mock_node('metro', 0.9, 120, 100, {out1 = 'b/in/in1'}),
      b = mock_node('value', 0.7, 100, 200),
    }
    sleep(5000)
    t.view:close()
    assert_true(true)
  end)
end

function should2.draw_overlay_process_view(t)
  t.win = mimas.Window()
  t.view = editor.ProcessView(mock_process())
  t.win:addWidget(t.view)
  t.view:move(30, 40)
  t.view:resize(260, 240)
  t.win:move(100, 100)
  t.win:resize(320, 300)
  t.win:show()

  function t.win.mouse(x, y)
    t.view:move(x - 10, y - 10)
    t.win:update()
  end

  t.thread = lk.Thread(function()
    sleep(2000)
    t.view:setName('Home Run')
    sleep(1400)
    t.win:close()
    assert_true(true)
  end)
end

test.gui()
