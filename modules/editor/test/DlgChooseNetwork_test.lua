--[[------------------------------------------------------

  Basic tests for DlgChooseNetwork
  --------------------------------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('editor.DlgChooseNetwork')

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
  assertTrue(editor.DlgChooseNetwork)
end

function should.displayDialog(t)
  t.data = mockNetworks()
  t.dlg = editor.DlgChooseNetwork(t.data, function(name)
    assertEqual('hyperion', name)
    t.dlg:delete()
  end)

  function t.dlg:click()
    t.dlg:close()
  end
  t.dlg:show()
  t.timeout = lk.Thread(function()
    sleep(3000)
    t.dlg:close()
  end)
end

test.gui()