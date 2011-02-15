--[[------------------------------------------------------

  mimas.GLWidget
  --------------

  A context to draw OpenGL content.

--]]------------------------------------------------------
local constr = mimas_core.GLWidget
local mt = mimas_core.GLWidget_
local worker = worker

local addWidget = mt.addWidget
function mt:addWidget(self, other, ...)
  addWidget(self, other:widget(), ...)
end

local addLayout = mt.addLayout
function mt:addLayout(self, other)
  addLayout(self, other:layout())
end

function mimas.GLWidget()
  return constr(worker)
end

