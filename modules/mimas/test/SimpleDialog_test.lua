--[[------------------------------------------------------

  Basic tests for SimpleDialog
  --------------------------------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('mimas.SimpleDialog')
local withUser = test.UserSuite('mimas.SimpleDialog')

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
  assertTrue(mimas.SimpleDialog)
end

function withUser.should.displayDialog(t)
  -- Who are you ?
  -- +----------------+
  -- | First name     |
  -- | [......]       |
  -- | Last name      |
  -- | [......]       |
  -- +----------------+
  -- [ok]  [cancel]

  t.dlg = mimas.SimpleDialog {
    'Who are you ?',
    {'vbox', box=true, style='background: #222',
      'First name',
      {'input', 'first_name', 'change to Bob'},
      'Last name',
      {'input', 'last_name',  'Morane'},
    },
    {'hbox',
      {}, -- this adds stretch
      {'btn', 'Cancel'},
      {'btn', 'OK', default=true},
    },
  }
  
  t.dlg:resize(200,200)
  t.dlg:move(1000,500)
  t.dlg:show()
  function t.dlg:btn(name)
    if name == 'OK' and t.dlg.form.first_name == 'Bob' then
      t.continue = true
    end
  end

  t:timeout(function(done)
    return done or t.continue
  end)
  t.dlg:close()
  assertTrue(t.continue)
end

function withUser.should.askForPath(t)
  -- Who are you ?
  -- First name [______]
  -- Last name  [______]
  -- Directory  [___] [...]
  -- [ok]  [cancel]
  t.dlg = mimas.SimpleDialog {
    'Who are you ?',
    {'vbox',
      {'hbox', 'First name', {'input', 'first_name', 'Gaspard'}},
      {'hbox', 'Last name',  {'input', 'last_name',  'Bucher'}},
      {'hbox', 'In lubyk',   {'input', 'dir', lfs.currentdir(), folder=true}},
    },
    {'hbox',
      {'btn', 'Cancel'},
      {'btn', 'OK', default = true},
    },
  }
  t.dlg:resize(200,200)
  t.dlg:move(1000,500)
  t.dlg:show()
  function t.dlg:btn(name)
    if name == 'OK' and string.match(t.dlg.form.dir,'lubyk') then
      t.continue = true
    end
  end

  t:timeout(function(done)
    return done or t.continue
  end)
  t.dlg:close()
  assertTrue(t.continue)
end

function should.displayDialog(t)
  -- TODO: test list
  --assertTrue(false)
end

  
test.all()
