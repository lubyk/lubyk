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

local function mock_node()
  return editor.Node{
    name = 'Dummy ProcessTab',
    hue  = 0.9,
    x    = 100,
    y    = 200,
    inlets = {
      {},
      {},
    },
    outlets = {
      {},
      {},
      {},
      {},
      {},
    }
  }
end

function should.draw_node_view(t)
  t.view = editor.NodeView(mock_node())
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
    assert_true(true)
  end)
end

test.gui()
