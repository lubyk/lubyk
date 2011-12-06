--[[------------------------------------------------------

  mimas.VBoxLayout
  ----------------

  Vertical layout of GUI widgets.

--]]------------------------------------------------------
local constr      = mimas_core.VBoxLayout
local mt          = mimas_core.VBoxLayout_
mimas.VBoxLayout_ = mt

local addWidget = mt.addWidget
local addLayout = mt.addLayout
function mt:addWidget(other)
  if other.layout then
    -- sub layout
    addLayout(self, other:layout())
  else
    addWidget(self, other:widget())
  end
end

function mt:addLayout(other)
  -- sub layout
  addLayout(self, other:layout())
end

function mimas.VBoxLayout(parent)
  if parent then
    return constr(parent:widget())
  else
    return constr()
  end
end
