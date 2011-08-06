--[[------------------------------------------------------

  Basic tests for ZoneChooser
  -----------------------------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('editor.SplashScreen')

function should.loadCode()
  assertTrue(editor.SplashScreen)
end

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
  function self:zonesDataSource()
    return mockList{'foobar', 'baz'}
  end

  function self:hostsDataSource()
    return mockList{'localhost', 'example.com'}
  end

  function self:selectZone(network_name)
    print('selected', network_name)
  end

  function self:startZone(zone_name, host_name, path)
    print(string.format("Start zone '%s' with morph path '%s' on '%s'", zone_name, host, path))
  end

  return self
end

function should.displayDialog(t)
  t.dlg = editor.SplashScreen(mockDelegate())
  t.timeout = lk.Thread(function()
    sleep(11000)
    t.dlg:close()
  end)
end

test.gui()