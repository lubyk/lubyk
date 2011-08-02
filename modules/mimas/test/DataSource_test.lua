--[[------------------------------------------------------

  Basic tests for DataSource
  --------------------------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('mimas.DataSource')

function should.loadCode()
  assertTrue(mimas.DataSource)
end

-- Uses with ListView tested in ListView_test
function should.respondToData(t)
  local ds = mimas.DataSource()
  function ds.data(row, col)
    return row + 10
  end

  assertType('function', ds.data)

  assertEqual(14, ds.data(4, 5))
  -- just to make sure it quits
  t.win = mimas.Window()
  t.win:show()
  t.win:close()
end

test.all()