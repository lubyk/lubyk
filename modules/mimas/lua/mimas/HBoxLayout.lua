--[[------------------------------------------------------

  mimas.HBoxLayout
  ----------------

  Horizontal layout of GUI widgets.

--]]------------------------------------------------------
local constr = mimas_core.HBoxLayout
local mt = mimas_core.HBoxLayout_
local worker = worker

local addWidget = mt.addWidget
function mt.addWidget(self, other)
  addWidget(self, other:widget())
end

local addLayout = mt.addLayout
function mt.addLayout(self, other)
  addLayout(self, other:layout())
end

function mimas.HBoxLayout(parent)
  local instance
  if parent then
    instance = constr(worker, parent:widget())
  else
    instance = constr(worker)
  end
  return instance
end

