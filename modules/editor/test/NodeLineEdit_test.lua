--[[------------------------------------------------------

  Basic tests for NodeLineEdit
  ----------------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.NodeLineEdit')

function should.loadCoad()
  assertType('function', editor.NodeLineEdit)
end

function should.callback(t)
  t.win = mimas.Window()
  t.win:move(100, 120)
  t.win:resize(200, 200)
  t.db = editor.Library(sqlite3.open_memory())
  t.db:sync()

  t.lb = editor.NodeLineEdit(t.win, "type 'm=l'", t.db)
  t.lb:selectAll()

  t.win:addWidget(t.lb)
  t.lb:move(40, 40)

  t.win:show()

  -- visual check
  assertTrue(true)
  t.thread = lk.Thread(function()
    sleep(12000)
    t.win:close()
  end)
end

test.gui()
