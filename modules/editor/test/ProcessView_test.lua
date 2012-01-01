--[[------------------------------------------------------

  editor.ProcessView test
  -----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.ProcessView')
local should2 = {}

function mockProcess()
  local process = {
    name           = 'Dummy ProcessView',
    hue            = 0.3,
    nodes          = {},
    pending_inlets = {},
    delegate       = editor.Zone(),
  }
  setmetatable(process, editor.Process)
  return process
end


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
      {name = 'in1'},
      {name = 'in2'},
    },
    outlets = {
      {name = 'out1'},
      {name = 'out2', links = links},
      {name = 'out3'},
    },
  }
  return node
end

function should2.setViewInProcess()
  local process = mockProcess()
  local view    = editor.ProcessView(process)
  assertEqual(view, process.view)
end

function should.drawNodesInProcessView(t)
  local process = mockProcess()
  -- Make as if changes are directly notified back without
  -- a remote.
  process.change = editor.Process.set

  t.view = editor.ProcessView(process)
  t.view:move(100, 100)
  t.view:show()

  t.thread = lk.Thread(function()
    sleep(300)
    process:set {
      nodes = {
        metro = mockNode('metro', 0.9, 120, 100),
      },
    }
    sleep(300)
    process:set {
      nodes = {
        value = mockNode('value', 0.7, 100, 200),
      },
    }
    sleep(300)
    process:set {
      nodes = {
        metro = {outlets = {{name = 'out1', links = {'value/in/in1'}}}},
        value = mockNode('value', 0.7, 100, 200),
      },
    }
    sleep(300)
    process:set {
      nodes = {
        value = {outlets = {{name = 'out1', links = {'foo/in/in2'}}}},
      },
    }
    sleep(10)
    process:set {
      nodes = {
        foo   = mockNode('foo', 0.5, 280, 80),
      },
    }
    sleep(1800)
    for t=0,1,0.005 do
      sleep(10)
      process:set {
        nodes = {
          foo = {y = 80 + 200 * t, x = 280 + 100*math.sin(t*5)},
        },
      }
    end
    sleep(5800)
    --t.view:close()
    assertTrue(true)
  end)
end

local function mockDelegate(t)
  local delegate = {}
  t.views = {}
  function delegate:addProcess(process)
    local view = editor.ProcessView(process)
    view:move(100, 500)
    view:show()
    t.views[process.name] = view
  end

  function delegate:removeProcess(process)
    local view = t.views[process.name]
    if view then
      view:close()
      t.views[process.name] = nil
    end
  end

  return delegate
end

function should.syncFromRemote(t)
  t.remote   = lk.Process(fixture.path('simple.yml'))
  t.delegate = mockDelegate(t)
  t.watch    = lk.ProcessWatch():addDelegate(t.delegate)
end

test.all()
