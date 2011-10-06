--[[------------------------------------------------------

  mimas.GLWidget
  --------------

  A context to draw OpenGL content.

--]]------------------------------------------------------
local mt        = mimas_core.GLWidget_
mimas.GLWidget_ = mt
local close  = mt.close

local constr  = mimas_core.GLWidget
function mimas.GLWidget(...)
  return mimas.bootstrap('GLWidget', constr, ...)
end

--=============================================== COPY CODE FROM Widget
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

-- default keyboard action
-- TODO: add CMD+W
function mt:keyboard(key, on)
  if on then
    if key == mimas.ESC then
      -- ESC
      self:close()
    elseif key == mimas.Space then
      self:swapFullScreen()
    end
  end
end

local close  = mt.close
function mt:close()
  if self:deleted() then
    return false
  else
    close(self)
    return true
  end
end

function mt:center()
  local w, h = app:screenSize()
  local sw, sh = self:size()
  self:move((w - sw) / 2, (h - sh) / 2)
end

