--[[------------------------------------------------------

  mimas.GroupBox
  --------------

  Group widgets with a frame and a title.

--]]------------------------------------------------------
mimas.GroupBox = mimas_core.GroupBox
local mt       = mimas_core.GroupBox_
mimas.GroupBox_ = mt

local constr  = mimas.GroupBox
function mimas.GroupBox(...)
  return mimas.bootstrap('GroupBox', constr, ...)
end

--=============================================== 
local addWidget = mt.addWidget
function mt:addWidget(other, ...)
  addWidget(self, other:widget(), ...)
end

local setParent = mt.setParent
function mt:setParent(other, ...)
  setParent(self, other:widget(), ...)
end

local addLayout = mt.addLayout
function mt:addLayout(other)
  addLayout(self, other:layout())
end

