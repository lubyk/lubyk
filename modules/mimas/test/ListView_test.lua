--[[------------------------------------------------------

  Basic tests for ListView
  ------------------------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('mimas.ListView')
local data = {'György', 'John', 'Marina', 'Damian'}
function should.displayList(t)
  t.view = mimas.ListView()

  function t.view.rowCount()
    return #data
  end

  function t.view.data(row_i)
    return data[row_i]
  end
  t.view:show()
  t.thread = lk.Thread(function()
    sleep(2000)
    t.view:close()
    assertTrue(true)
  end)
end

function should.respondToClick(t)
  t.view = mimas.ListView()

  function t.view.rowCount()
    return #data
  end

  function t.view.data(row_i)
    return data[row_i]
  end

  function t.view.click(x, y)
    local i, j = t.view:indexAt(x, y)
    assertEqual(2, i)
  end

  function t.view.mouse(x, y)
    print('mouse', x, y)
  end

  t.view:show()
  t.thread = lk.Thread(function()
    sleep(2000)
    t.view:close()
    assertTrue(true)
  end)
end

function should.useDataSource(t)
  t.view = mimas.ListView()
  t.data = mimas.DataSource()

  function t.data.rowCount()
    return #data
  end

  function t.data.data(row)
    return data[row]
  end

  t.view:setModel(t.data)

  function t.view.select(row)
    assertTrue(2, row)
    t.view:close()
  end
  t.view:show()
end

test.all()

