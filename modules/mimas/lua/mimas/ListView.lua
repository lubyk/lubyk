--[[------------------------------------------------------

  mimas.ListView
  --------------

  Display data as a list.

--]]------------------------------------------------------
require 'worker'
local worker = worker
local constr = mimas_core.ListView
local mt     = mimas_core.ListView_
local close  = mt.close

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

function mt:close()
  -- close is like delete: ensure it only runs in GUI thread
  app:post(function()
    close(self)
  end)
end
