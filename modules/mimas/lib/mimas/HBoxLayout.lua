--[[------------------------------------------------------

  mimas.HBoxLayout
  ----------------

  Horizontal layout of GUI widgets.

--]]------------------------------------------------------
local constr      = mimas_core.HBoxLayout
local mt          = mimas_core.HBoxLayout_
mimas.HBoxLayout_ = mt

function mimas.HBoxLayout(parent)
  if parent then
    return constr(parent:widget())
  else
    return constr()
  end
end

local addWidget = mt.addWidget
local addLayout = mt.addLayout
function mt:addWidget(other, ...)
  if other.layout then
    -- sub layout
    addLayout(self, other:layout(), ...)
  else
    addWidget(self, other:widget(), ...)
  end
end

function mt:addLayout(other, ...)
  -- sub layout
  addLayout(self, other:layout(), ...)
end

local insertWidget = mt.insertWidget
local insertLayout = mt.insertLayout
function mt:insertWidget(pos, other, ...)
  if other.layout then
    -- sub layout
    insertLayout(self, pos, other:layout(), ...)
  else
    insertWidget(self, pos, other:widget(), ...)
  end
end

function mt:insertLayout(pos, other, ...)
  insertLayout(self, pos, other:layout(), ...)
end

local setAlignment = mt.setAlignment
function mt:setAlignment(widget, ...)
  return setAlignment(self, widget:widget(), ...)
end
