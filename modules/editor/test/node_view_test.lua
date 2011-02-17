--[[------------------------------------------------------

  editor.ProcessTab test
  ----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.NodeView')

local function mock_node()
  return {
    name = 'Dummy ProcessTab',
    hue  = 0.9,
    inlets = {
      {hue = 0.2},
      {hue = 0.4},
    },
    outlets = {
      {hue = 0.6},
      {hue = 0.2},
      {hue = 0.9},
      {hue = 0.9},
      {hue = 0.9},
    }
  }
end

function should.draw_node_view(t)
  t.tab = editor.NodeView(mock_node())
  t.tab:move(100, 100)
  t.tab:show()

  t.thread = lk.Thread(function()
    sleep(9800)
    t.tab:close()
    assert_true(true)
  end)
end

test.gui()
