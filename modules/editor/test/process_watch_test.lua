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

  math.randomseed(os.time())

  local service_count = 5
  for i=1,service_count do
    t.threads[i] = lk.Thread(function()
      local sl = 400 + 1000 * math.random()
      print(sl)
      io.flush()
      sleep(sl)
      print('new', string.format('Process %i', i))
      t.services[i] = lk.Service(string.format('Process %i', i))
      t.services[i].info.hue = math.random()
      sl = 1400 + 5000 * math.random()
      print(sl)
      io.flush()
      sleep(sl)
      print('kill', string.format('Process %i', i))
      t.services[i]:kill()
    end)
  end

  t.thread = lk.Thread(function()
    sleep(6000)
    app:post(function()
      t.win:resize(300, 300)
    end)
    sleep(6000)
    for i=1,service_count do
      t.threads[i]:join()
    end
    t.win:close()
  end)
end

test.gui()