--[[------------------------------------------------------

  mimas.DataSource
  ----------------

  Can replace the default (inner) source of data for
  ListView, TableView and friends.

--]]------------------------------------------------------
require 'worker'
local worker = worker
local constr = mimas_core.DataSource

function mimas.DataSource()
  local self = constr(worker)
  function self.columnCount()
    -- default useful for ListView
    return 1
  end
  return self
end
