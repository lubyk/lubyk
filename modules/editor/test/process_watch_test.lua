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

function should.add_widgets_to_list(t)
  t.watch = editor.ProcessWatch()
  t.win = mimas.Window()
  t.lay = mimas.HBoxLayout(t.win)
  t.label = mimas.Label('Control')
  t.label:setStyle('background: hsva(80, 255, 255, 40%); text-align:center;')
  t.lay:addWidget(t.label, 2, mimas.AlignCenter)
  --t.lay:setSpacing(0)
  t.lay:setContentsMargins(0,0,0,0)
  t.lay:addWidget(t.watch.view, 0, mimas.AlignRight + mimas.AlignTop)

  t.win:resize(200, 200)
  t.win:move(100, 100)
  t.win:show()

  t.threads = {}

  t.threads  = {}
  t.services = {}
  t.name = {'Dune', 'Hyperion', 'Dorsai', 'Andromeda', 'Pandora'}

  math.randomseed(os.time())

  local service_count = 5
  for i=1,service_count do
    t.threads[i] = lk.Thread(function()
      sleep(500 * math.random())
      t.services[i] = lk.Service(t.name[i] or string.format('Process %i', i))
      t.services[i].info.hue = math.random()
      sleep(2000 + 2000 * math.random())
      t.services[i]:kill()
      sleep(1000)
    end)
  end

  t.thread = lk.Thread(function()
    sleep(2000)
    app:post(function()
      t.win:resize(300, 300)
    end)
    for i=1,service_count do
      t.threads[i]:join()
    end
    t.win:close()
  end)
end

test.gui()