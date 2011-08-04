--[[------------------------------------------------------

  Basic tests for SimpleDialog
  --------------------------------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('editor.SimpleDialog')

local function mockList(data)
  local self = mimas.DataSource()
  data = data or {'dune', 'hyperion'}
  function self.rowCount()
    return #data
  end

  function self.data(row)
    return data[row]
  end
  return self
end

function should.loadCode()
  assertTrue(editor.SimpleDialog)
end

function should.displayDialog(t)
  t.data = mockList()
  t.dlg = editor.SimpleDialog {
    data    = t.data,
    message = 'Choose network',
    cancel  = 'Cancel',
    ok      = 'New',
  }

  function t.dlg:select(row)
    assertEqual(2, row)
    self:close()
  end

  function t.dlg:ok()
    self:close()
  end

  t.dlg:show()
  t.dlg:move(100, 50)
  t.timeout = lk.Thread(function()
    sleep(3000)
    t.dlg:close()
  end)
end


function should.displayDialogWithoutButtons(t)
  t.data = mockList()
  t.dlg = editor.SimpleDialog {
    data    = t.data,
    message = 'Choose host',
  }

  function t.dlg:select(row)
    assertEqual(2, row)
    self:close()
  end

  t.dlg:show()
  t.dlg:move(100, 250)
  t.timeout = lk.Thread(function()
    sleep(2000)
    t.dlg:close()
  end)
end

function should.displayNextView(t)
  t.win = mimas.Window()
  t.lay = mimas.VBoxLayout(t.win)
  t.dlg = editor.SimpleDialog {
    data    = mockList(),
    message = 'Choose network (press new)',
    ok      = 'New network',
  }
  t.lay:addWidget(t.dlg)

  function t.dlg:ok(name)
    t.dlg:close()
    t.dlg = editor.SimpleDialog {
      data    = mockList{'localhost', 'example.com'},
      message = 'Choose host',
      cancel  = 'Abort',
      line    = 'first name',
      line_value = 'Gaspard',
      line2   = 'last name',
      line2_value = 'Bucher',
    }

    function t.dlg:select(row)
      t.win:close()
    end

    function t.dlg:cancel()
      t.win:close()
    end
    t.lay:addWidget(t.dlg)
  end


  t.win:show()
  t.win:move(100, 450)
  t.timeout = lk.Thread(function()
    sleep(4000)
    t.win:close()
  end)
end

test.gui()