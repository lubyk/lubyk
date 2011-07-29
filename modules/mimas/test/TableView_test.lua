--[[------------------------------------------------------

  Basic tests for TableView
  -------------------------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('mimas.TableView')
local data = {
  head={'first name', 'last name', 'profession'},
       {'György',     'Ligeti',    'Composer'},
       {'John',       'Cage',      'Composer'},
       {'Marina',     'Abramović', 'Perormance artist'},
       {'Damian',     'Marley',    'Musician'},
}

function should.loadCode()
  assertTrue(mimas.TableView)
end

function should.displayTable(t)
  t.view = mimas.TableView()
  t.view:setAlternatingRowColors(true)
  t.view:setVisibleHeaders(mimas.Vertical, false)
  t.view:setGridStyle(mimas.NoPen)

  function t.view.columnCount()
    return #data.head
  end

  function t.view.rowCount()
    return #data
  end

  function t.view.data(row_i, column_i)
    local row = data[row_i]
    if row then
      return row[column_i]
    else
      return nil
    end
  end

  function t.view.header(column_i, orientation)
    if orientation == mimas.Horizontal then
      return data.head[column_i]
    else
      return nil
    end
  end
  t.view:show()
  assertTrue(true)

  t.thread = lk.Thread(function()
    sleep(800)
    t.view:close()
  end)
end

function should.respondToUpdate(t)
  t.view = mimas.TableView()
  t.view:show()
  assertPass(function()
    t.view:update()
  end)
  t.view:close()
end
test.all()

