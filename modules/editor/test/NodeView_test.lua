--[[------------------------------------------------------

  editor.ProcessTab test
  ----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.NodeView')

-- editor needs a 'main' state with currently selected objects
-- and such things
editor.main = {dragging = {}}

local function mockNode()
  return editor.Node{
    process = {pending_inlets = {}},
    name = 'Dummy ProcessTab',
    hue  = 0.9,
    x    = 100,
    y    = 200,
    inlets = {
      a = {},
      b = {},
    },
    outlets = {
      c = {},
      d = {},
      e = {},
      f = {},
      g = {},
    }
  }
end

function should.drawNodeView(t)
  t.view = editor.NodeView(mockNode())
  t.view:show()

  t.thread = lk.Thread(function()
    sleep(1000)
    editor.main.dragging.type = 'outlet'
    app:post(function()
      t.view:update()
    end)
    sleep(1000)
    editor.main.dragging.type = 'inlet'
    app:post(function()
      t.view:update()
    end)
    sleep(1000)
    t.view:close()
    assertTrue(true)
  end)
end

test.gui()
