--[[------------------------------------------------------

  mimas.Widget
  ------------

  ...

--]]------------------------------------------------------
local constr = mimas_core.Widget
local worker = worker
local mt     = mimas_core.Widget_

local addWidget = mt.addWidget
function mt:addWidget(other, ...)
  addWidget(self, other:widget(), ...)
end

local addLayout = mt.addLayout
function mt:addLayout(other)
  addLayout(self, other:layout())
end

function mimas.Widget(...)
  return constr(worker, ...)
end
