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

function add_tabs(list)
  list:addProcess{name = 'Two', hue = 0.7}
  list:addProcess{name = 'Process One', hue = 0.9}
  list:addProcess{name = 'Armand', hue = 0.3}
end

function should.add_widgets_to_list(t)

  t = {}
  t.list = editor.ProcessList()

  t.win = mimas.Window()
  t.lay = mimas.HBoxLayout(t.win)
  t.label = mimas.Label('hhle')
  t.label:setStyle('background: hsva(80, 255, 255, 40%)')
  t.lay:addWidget(t.label, 2, mimas.AlignCenter)
  --t.lay:setSpacing(0)
  t.lay:setContentsMargins(0,0,0,0)
  t.lay:addWidget(t.list, 0, mimas.AlignRight + mimas.AlignTop)

  add_tabs(t.list)

  t.win:resize(200, 200)
  t.win:move(100, 100)
  t.win:show()

  t.thread = lk.Thread(function()
    sleep(400)
    app:post(function()
      t.win:resize(300, 300)
    end)
    sleep(400)
    app:post(function()
      -- new widgets must be created in the GUI thread
      t.list:addProcess{name = 'Dune', hue = 0.2}
    end)
    sleep(400)
    t.list:removeProcess{name = 'Process One'}
    sleep(400)
    t.win:close()
  end)
end

test.gui()