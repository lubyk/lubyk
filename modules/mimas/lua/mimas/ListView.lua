--[[------------------------------------------------------

  mimas.ListView
  --------------

  Display data as a list.

--]]------------------------------------------------------
require 'worker'
local worker = worker
local constr = mimas_core.ListView

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
