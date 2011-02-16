--[[------------------------------------------------------

  editor.ProcessList test
  ----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.ProcessList')

function add_tabs(list)
  list:addProcess{name = 'Two', hue = 0.7}
  list:addProcess{name = 'Process One', hue = 0.9}
  list:addProcess{name = 'Armand', hue = 0.3}
end

function should.draw_process_list(t)
  t.list = editor.ProcessList()
  add_tabs(t.list)
  -- they should be sorted
  assert_equal('Armand', t.list.list[1].name)
  assert_equal('Process One', t.list.list[2].name)
  assert_equal('Two', t.list.list[3].name)

  t.list:move(100, 100)
  t.list:show()

  t.thread = lk.Thread(function()
    sleep(800)
    app:post(function()
      -- new widgets must be created in the GUI thread
      t.list:addProcess{name = 'Dune', hue = 0.2}
    end)
    sleep(800)
    t.list:removeProcess{name = 'Process One'}
    sleep(800)
    t.list:close()
    assert_true(true)
  end)
end

function should.remove_process_tab(t)
  t.list = editor.ProcessList()
  add_tabs(t.list)
  t.list:removeProcess{name = 'Process One'}
  assert_equal('Armand', t.list.list[1].name)
  assert_equal('Two', t.list.list[2].name)
  assert_nil(t.list.list[3])
end

test.gui()
