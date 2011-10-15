--[[------------------------------------------------------

  editor.ProcessTab test
  ----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.NodeView')

local function mockDelegate()
 local delegate = {dragging = {}}
 return delegate
end

local function mockNode()
  return editor.Node{
    process  = {pending_inlets = {}, delegate = mockDelegate()},
    name = 'Dummy ProcessTab',
    hue  = 0.9,
    x    = 100,
    y    = 200,
    inlets = {
      {name = 'a'},
      {name = 'b'},
    },
    outlets = {
      {name = 'c'},
      {name = 'd'},
      {name = 'e'},
      {name = 'f'},
      {name = 'g'},
    }
  }
end

function should.drawNodeView(t)
  t.view = editor.NodeView(mockNode())
  local delegate = t.view.node.delegate
  t.view:show()
  t.view:move(10, 10)

  sleep(1000)
  delegate.dragging.type = 'outlet'
  t.view:update()
  sleep(1000)
  delegate.dragging.type = 'inlet'
  t.view:update()
  sleep(1000)
  t.view:close()
end

test.all()
