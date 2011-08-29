--[[------------------------------------------------------

  mimas.Widget
  ------------

  ...

--]]------------------------------------------------------
mimas.Widget  = mimas_core.Widget
local mt      = mimas_core.Widget_
mimas.Widget_ = mt

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

function mt:center()
  local w, h = app:screenSize()
  local sw, sh = self:size()
  self:move((w - sw) / 2, (h - sh) / 2)
end

local getOpenFileName = mt.getOpenFileName

function mt:getOpenFileName(caption, base_dir, filter, options)
  return getOpenFileName(self, caption, base_dir or '', filter or '', options or 0)
end

local getExistingDirectory = mt.getExistingDirectory
function mt:getExistingDirectory(caption, base_dir, options)
  return getExistingDirectory(self, caption, base_dir or '', options or mimas.ShowDirsOnly)
end
