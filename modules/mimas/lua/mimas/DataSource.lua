--[[------------------------------------------------------

  mimas.DataSource
  ----------------

  Can replace the default (inner) source of data for
  ListView, TableView and friends.

--]]------------------------------------------------------
require 'worker'
local worker = worker
local constr = mimas_core.DataSource

function mimas.DataSource(data)
  local self = constr(worker)
  function self.columnCount()
    -- default useful for ListView
    return 1
  end
  if data then
    function self.rowCount()
      return #data
    end
    function self.data(row)
      return data[row]
    end
  end

  return self
end
