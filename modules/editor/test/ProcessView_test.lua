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

function mockProcess()
  local process = {
    name           = 'Dummy ProcessView',
    hue            = 0.3,
    nodes          = {},
    pending_inlets = {},
  }
  setmetatable(process, editor.Process)
  return process
end

local should2 = {}

function should2.drawProcessView(t)
  t.view = editor.ProcessView(mockProcess())
  t.view:move(100, 100)
  t.view:show()

  t.thread = lk.Thread(function()
    sleep(2000)
    t.view:setName('Home')
    sleep(2000)
    t.view:close()
    assertTrue(true)
  end)
end

local function mockNode(name, hue, x, y, links)
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
      out2 = {links = links},
      out3 = {},
    },
  }
  return node
end

function should.setViewInProcess()
  local process = mockProcess()
  local view    = editor.ProcessView(process)
  assertEqual(view, process.view)
end

function should.drawNodesInProcessView(t)
  local process = mockProcess()
  t.view = editor.ProcessView(process)
  t.view:move(100, 100)
  t.view:show()

  t.thread = lk.Thread(function()
    sleep(500)
    process:set {
      nodes = {
        metro = mockNode('metro', 0.9, 120, 100, {'value/in/in1'}),
        value = mockNode('value', 0.7, 100, 200),
      },
    }
    sleep(5000)
    t.view:close()
    assertTrue(true)
  end)
end

function should2.drawOverlayProcessView(t)
  t.win = mimas.Window()
  t.view = editor.ProcessView(mockProcess())
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
    assertTrue(true)
  end)
end

test.gui()
