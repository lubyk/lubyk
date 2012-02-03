--[[------------------------------------------------------

  mimas.Widget
  ------------

  ...

--]]------------------------------------------------------
mimas.Widget  = mimas_core.Widget
local mt      = mimas_core.Widget_
mimas.Widget_ = mt
-- We use this from C++. Make sure it is loaded to use the
-- Lua changes before being used.
require 'mimas.Menu'

local constr  = mimas.Widget
function mimas.Widget_constr(parent, flag)
  if flag and parent then
    return constr(parent:widget(), flag)
  elseif flag then
    return constr(flag)
  elseif type(parent) == 'table' then
    return constr(parent:widget())
  elseif parent then
    return constr(parent)
  else
    return constr()
  end
end

function mimas.Widget(...)
  return mimas.bootstrap('Widget', mimas.Widget_constr, ...)
end


--=============================================== COPY CODE TO GLWidget
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

--=============================================== dialog (maybe we should have
--                                                        this in GLWidget too)

local getOpenFileName = mt.getOpenFileName

function mt:getOpenFileName(caption, base_dir, filter, options)
  return getOpenFileName(self, caption, base_dir or '', filter or '', options or 0)
end

local getExistingDirectory = mt.getExistingDirectory
function mt:getExistingDirectory(caption, base_dir, options)
  return getExistingDirectory(self, caption, base_dir or '', options or mimas.ShowDirsOnly)
end
