--[[------------------------------------------------------

  mimas.VBoxLayout
  ----------------

  Vertical layout of GUI widgets.

--]]------------------------------------------------------
local constr = mimas_core.VBoxLayout
local mt     = mimas_core.VBoxLayout_
local worker = worker

local addWidget = mt.addWidget
function mt:addWidget(other)
  addWidget(self, other:widget())
end

local addLayout = mt.addLayout
function mt:addLayout(other)
  addLayout(self, other:layout())
end

function mimas.VBoxLayout(parent)
  local instance
  if parent then
    instance = constr(worker, parent:widget())
  else
    instance = constr(worker)
  end
  return instance
end

