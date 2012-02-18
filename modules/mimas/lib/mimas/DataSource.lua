--[[------------------------------------------------------

  mimas.DataSource
  ----------------

  Can replace the default (inner) source of data for
  ListView, TableView and friends.

--]]------------------------------------------------------
local constr      = mimas_core.DataSource
local mt          = mimas_core.DataSource_
mimas.DataSource_ = mt

function mimas.DataSource(data)
  local self = constr()
  function self.columnCount()
    -- default useful for ListView
    return 1
  end
  if data then
    function self:rowCount()
      return #data
    end
    function self:data(row)
      return data[row]
    end
  end

  return self
end

local mt = mimas_core.DataSource_

function mt:__index(key)
  return rawget(mt, key) or mt.getCallback(self, key)
end
