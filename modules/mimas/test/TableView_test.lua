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
function should.displayTable(t)
  t.view = mimas.TableView()
  function t.view.columnCount()
    return #data[1]
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
end
test.all()

