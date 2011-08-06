--require 'lubyk'
--
--app = mimas.Application()
--win = mimas.Window()
--lay = mimas.HBoxLayout(win)
--pw = editor.ProcessWatch()
--lay:addWidget(pw.view)
--
--win:show()
--
--app:exec()

require 'lubyk'

local should = test.Suite('editor.ProcessWatch')

local function mockDelegate()
  return editor.Zone()
end

function should.add_widgets_to_list(t)
  t.delegate = mockDelegate()
  t.watch = editor.ProcessWatch(t.delegate)
  t.win = mimas.Window()
  t.lay = mimas.HBoxLayout(t.win)
  t.label = mimas.Label('Control')
  t.label:setStyle('background: hsva(80, 255, 255, 40%); text-align:center;')
  t.lay:addWidget(t.label, 2, mimas.AlignCenter)
  --t.lay:setSpacing(0)
  t.lay:setContentsMargins(0,0,0,0)
  t.lay:addWidget(t.delegate.process_list_view, 0, mimas.AlignRight + mimas.AlignTop)

  t.win:resize(200, 200)
  t.win:move(100, 100)
  t.win:show()

  t.threads = {}

  t.threads  = {}
  t.services = {}
  t.name  = {'Dune', 'Hyperion', 'Dorsai', 'Andromeda', 'Pandora'}
  -- The short values are to test servers going out before
  -- we can connect.
  t.sleep = {500, 3000, 1500}

  math.randomseed(os.time())

  local service_count = 7
  for i=1,service_count do
    t.threads[i] = lk.Thread(function()
      sleep(500 * math.random())
      t.services[i] = lk.Service(t.name[i] or string.format('Process %i', i))
      t.services[i].info.hue = math.random()
      sleep(t.sleep[i] or 1000 + 5000 * math.random())
      t.services[i]:kill()
      sleep(1000)
    end)
  end

  t.thread = lk.Thread(function()
    sleep(1000)
    app:post(function()
      t.win:resize(300, 300)
    end)
    for i=1,service_count do
      t.threads[i]:join()
    end
    assertTrue(true)
    t.win:close()
  end)
end

test.gui()