--[[------------------------------------------------------

  mimas.HBoxLayout
  ----------------

  Horizontal layout of GUI widgets.

--]]------------------------------------------------------
local constr = mimas_core.HBoxLayout
local mt = mimas_core.HBoxLayout_
local worker = worker

local addWidget = mt.addWidget
local addLayout = mt.addLayout
function mt.addWidget(self, other)
  if other.layout then
    -- sub layout
    addLayout(self, other:layout())
  else
    addWidget(self, other:widget())
  end
end

function mimas.HBoxLayout(parent)
  if parent then
    return constr(parent:widget())
  else
    return constr()
  end
end

