--[[------------------------------------------------------

  Basic tests for NetworkChooser
  -----------------------------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('editor.SplashScreen')

local function mockList(data)
  local self = mimas.DataSource()
  function self.rowCount()
    return #data
  end
  function self.data(row)
    return data[row]
  end
  return self
end

local function mockDelegate()
  local self = {}
  function self:networksDataSource()
    return mockList{'foobar', 'baz'}
  end

  function self:hostsDataSource()
    return mockList{'localhost', 'example.com'}
  end

  function self:selectNetwork(network_name)
    print('selected', network_name)
  end

  function self:startNetwork(host_name, path)
    print(string.format('start network with "%s" on "%s"', path, host_name))
  end

  return self
end

function should.loadCode()
  assertTrue(editor.SplashScreen)
end

function should.displayDialog(t)
  t.dlg = editor.SplashScreen(mockDelegate())
  t.timeout = lk.Thread(function()
    sleep(113000)
    t.dlg:close()
  end)
end

test.gui()