--[[------------------------------------------------------

  Basic tests for LineEditAuto
  ----------------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.LineEditAuto')

function should.loadCoad()
  assertType('table', mimas.LineEditAuto)
end

function should.callback(t)
  t.win = mimas.Window()
  t.win:move(100, 120)
  t.win:resize(200, 200)
  t.db = editor.Library(sqlite3.open_memory())
  t.db:sync()

  t.lb = mimas.LineEditAuto("type 'm=l'", '^(.+= *)(.+)')
  t.lb:selectAll()

  t.win:addWidget(t.lb)
  -- FIXME: we need this because we cannot find a useable Lua object with
  -- LineEdit:parent(). This could be fixed with minimal bindings for QWidget (not mimas::Widget).
  t.lb.parent = t.win
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
  -- ====================================================

  t.win:show()

  -- visual check
  assertTrue(true)
  t.thread = lk.Thread(function()
    sleep(12000)
    t.win:close()
  end)
end

test.all()