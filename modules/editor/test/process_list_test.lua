--[[------------------------------------------------------

  editor.ProcessList test
  ----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.ProcessList')

function addTabs(list)
  list:addProcess{name = 'Two', hue = 0.7}
  list:addProcess{name = 'Process One', hue = 0.9}
  list:addProcess{name = 'Armand', hue = 0.3}
end

function should.drawProcessList(t)
  t.list = editor.ProcessList()
  addTabs(t.list)
  -- they should be sorted
  assertEqual('Armand', t.list.list[1].name)
  assertEqual('Process One', t.list.list[2].name)
  assertEqual('Two', t.list.list[3].name)

  t.list:move(100, 100)
  t.list:resize(100, 300)
  t.list:show()

  t.thread = lk.Thread(function()
    sleep(800)
    app:post(function()
      -- new widgets must be created in the GUI thread
      t.list:addProcess{name = 'Dune', hue = 0.2}
    end)
    sleep(800)
    t.list:removeProcess('Process One')
    sleep(800)
    t.list:close()
    assertTrue(true)
  end)
end

function should.removeProcessTab(t)
  t.list = editor.ProcessList()
  addTabs(t.list)
  t.list:removeProcess('Process One')
  assertEqual('Armand', t.list.list[1].name)
  assertEqual('Two', t.list.list[2].name)
  assertNil(t.list.list[3])
end

test.gui()
