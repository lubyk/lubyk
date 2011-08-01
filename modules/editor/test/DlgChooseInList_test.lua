--[[------------------------------------------------------

  Basic tests for DlgChooseInList
  --------------------------------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('editor.DlgChooseInList')

local function mockNetworks()
  local self = {'dune', 'hyperion'}
  function self:count()
    return #self
  end

  function self:data(row)
    return self[row]
  end
  return self
end

function should.loadCode()
  assertTrue(editor.DlgChooseInList)
end

function should.displayDialog(t)
  t.data = mockNetworks()
  t.dlg = editor.DlgChooseInList {
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
  t.timeout = lk.Thread(function()
    sleep(3000)
    t.dlg:close()
  end)
end


function should.displayDialogWithoutButtons(t)
  t.data = mockNetworks()
  t.dlg = editor.DlgChooseInList {
    data    = t.data,
    message = 'Choose host',
  }

  function t.dlg:selected(name)
    assertEqual('hyperion', name)
    self:close()
  end

  t.dlg:show()
  t.dlg:move(100,200)
  t.timeout = lk.Thread(function()
    sleep(2000)
    t.dlg:close()
  end)
end

test.gui()