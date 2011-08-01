--[[------------------------------------------------------

  mimas.Widget
  ------------

  ...

--]]------------------------------------------------------
local constr = mimas_core.Widget
local worker = worker
local mt     = mimas_core.Widget_
local close  = mt.close

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

function mimas.Widget(...)
  local win = constr(worker, ...)
  -- default keyboard action
  -- TODO: add CMD+W
  function win.keyboard(key, on)
    if on then
      if key == mimas.ESC then
        -- ESC
        win:close()
      elseif key == mimas.Space then
        win:swapFullScreen()
      end
    end
  end
  return win
end

function mt:close()
  -- close is like delete: ensure it only runs in GUI thread
  if not self:deleted() then
    app:post(function()
      close(self)
    end)
  end
end
