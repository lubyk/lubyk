--[[------------------------------------------------------

  mimas.GLWidget
  --------------

  A context to draw OpenGL content.

--]]------------------------------------------------------
local constr    = mimas_core.GLWidget
local mt        = mimas_core.GLWidget_
mimas.GLWidget_ = mt
local close  = mt.close

local addWidget = mt.addWidget
function mt:addWidget(self, other, ...)
  addWidget(self, other:widget(), ...)
end

local addLayout = mt.addLayout
function mt:addLayout(self, other)
  addLayout(self, other:layout())
end

function mimas.GLWidget()
  return constr()
end

function mt:close()
  -- close is like delete: ensure it only runs in GUI thread
  if self:deleted() then
    return false
  else
    app:post(function()
      close(self)
    end)
    return true
  end
end
