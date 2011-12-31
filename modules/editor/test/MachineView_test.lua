--[[------------------------------------------------------

  editor.MachineView test
  -----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.MachineView')

function addTabs(list)
  list:addProcess{name = 'Two', hue = 0.7}
  list:addProcess{name = 'Process One', hue = 0.9}
  list:addProcess{name = 'Armand', hue = 0.3}
end

function should.drawMachineView(t)
  local machine = editor.Machine('venus', {ip='localhost'})
  t.win = mimas.Window()
  t.win:move(100,100)
  t.win:resize(400,400)
  t.list = editor.MachineView(machine)
  t.win:addWidget(t.list)
  addTabs(t.list)
  t.win:show()

  sleep(800)
  t.list:addProcess{name = 'Dune', hue = 0.2}
  sleep(1800)
  t.list:removeProcess('Process One')
  sleep(111800)
  t.win:close()
  assertTrue(true)
end

test.all()
