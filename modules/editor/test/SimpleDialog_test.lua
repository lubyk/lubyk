--[[------------------------------------------------------

  Basic tests for SimpleDialog
  --------------------------------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('editor.SimpleDialog')

local function mockList(list)
  local self = list or {'dune', 'hyperion'}
  function self:count()
    return #self
  end

  function self:data(row)
    return self[row]
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
    new     = 'New',
  }

  function t.dlg:selected(name)
    assertEqual('hyperion', name)
    self:delete()
  end

  function t.dlg:new()
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

  function t.dlg:selected(name)
    assertEqual('hyperion', name)
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
    new     = 'New network',
  }
  t.lay:addWidget(t.dlg)

  function t.dlg:new(name)
    t.dlg:close()
    t.dlg = editor.SimpleDialog {
      data    = mockList{'localhost', 'example.com'},
      message = 'Choose host',
      cancel  = 'Abort',
      line    = 'name'
    }

    function t.dlg:selected(name)
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
    sleep(  4000)
    t.win:close()
  end)
end

test.gui()