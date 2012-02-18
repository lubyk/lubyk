--[[------------------------------------------------------

  mimas.ListView
  --------------

  Display data as a list.

--]]------------------------------------------------------
local constr    = mimas_core.ListView
local lib       = mimas_core.ListView_
mimas.ListView_ = lib
local close  = lib.close

function mimas.ListView(parent)
  local self = constr(worker)
  if parent then
    parent:addWidget(self)
  end
  function self.columnCount()
    return 1
  end
  return self
end

function lib:close()
  -- close is like delete: ensure it only runs in GUI thread
  if not self:deleted() then
    close(self)
  end
end

-- refresh
function lib:reset()
  self.data_source:reset()
end

