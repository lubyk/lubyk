--[[------------------------------------------------------

  mimas.TabWidget
  ---------------

  A TabWidget displays widgets with tabs.

--]]------------------------------------------------------
local constr  = mimas_core.TabWidget
local mt      = mimas_core.TabWidget_
mimas.TabWidget_   = mt
mimas.TabWidget    = constr

local function styleFix(self)
  -- FIXME: why doesn't the rule QTabBar::tab:first work with a
  -- single element ?
  if self:count() > 1 then
    self:setStyleSheet [[
    QTabBar::tab {
      margin-left:-1px;
    }
    QTabBar::tab:first {
      margin-left:0;
    }
    ]]
  else
    self:setStyleSheet [[
    QTabBar::tab {
      margin-left:0px;
    }
    ]]
  end
end

local addTab    = mt.addTab
local insertTab = mt.insertTab
function mt:addTab(widget, label)
  local pos = addTab(self, widget:widget(), label)
  styleFix(self)
  return pos
end

function mt:insertTab(pos, widget, label)
  local pos = insertTab(self, pos, widget:widget(), label)
  styleFix(self)
  return pos
end

local Key_Left, Key_Right = mimas.Key_Left, mimas.Key_Right
local Modifier = mimas.AltModifier
function mt:keyboard(key, on, chr, mod)
  -- FIXME: Why don't we capture keyPress of arrow keys ?
  -- FIXME: Why can't we capture ControlModifier with arrows ?
  if not on and bit.band(mod, Modifier) ~= 0 then
    if key == Key_Right then
      local selected = self:currentIndex()
      local count = self:count()
      self:setCurrentIndex((selected+1) % count)
      return
    elseif key == Key_Left then
      local selected = self:currentIndex()
      local count = self:count()
      self:setCurrentIndex((selected+count-1) % count)
      return
    end
  end
  -- Continue processing
  return true
end
