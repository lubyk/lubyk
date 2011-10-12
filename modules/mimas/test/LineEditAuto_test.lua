--[[------------------------------------------------------

  Basic tests for LineEditAuto
  ----------------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.LineEditAuto')
local withUser = should:testWithUser()

function should.loadCoad()
  assertType('table', mimas.LineEditAuto)
end

function withUser.should.callback(t)
  t.win = mimas.Window()
  t.win:move(100, 120)
  t.win:resize(200, 200)
  t.db = editor.Library(sqlite3.open_memory())
  t.db:sync()

  t.lb = mimas.LineEditAuto(t.win, "type 'm=l'", '^(.+= *)(.+)')
  t.lb:selectAll()

  t.win:addWidget(t.lb)
  t.lb:move(40, 40)

  -- ====================================================
  -- How this particular auto-complete works is defined by the following
  -- methods
  function t.lb:cueChanged(cue)
    self.db_pattern = '%' .. cue .. '%'
  end

  function t.lb:resultCount()
    return t.db:nodeCount(self.db_pattern)
  end

  function t.lb:data(row)
    local data
    if row then
      -- data for list
      data = t.db:node(self.db_pattern, row)
    else
      -- data for inline
      data = t.db:node(self.cue .. '%')
    end
    if data then
      return data.name
    else
      return nil
    end
  end

  function t.lb:editingFinished(text)
    self:autoFinished()
    assertEqual('m=lubyk.Value', text)
    t.continue = true
  end
  -- ====================================================

  t.win:show()

  t:timeout(function(done)
    return done or t.continue
  end)
  t.win:close()
  assertTrue(t.continue)
end

test.all()
