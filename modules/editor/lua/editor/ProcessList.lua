--[[------------------------------------------------------

  editor.ProcessList
  ------------------

  Displays the list of ProcessTabs.

--]]------------------------------------------------------
local lib = mimas.WidgetClass()
editor.ProcessList = lib

-- constants
local width       = 120
local padding     = 10
local elem_height = 40

function lib:init()
  self.list  = {}
  self.width = width
  function self.super.resized(w, h)
    self:resized(w, h)
  end
end

function lib:addTab(process_tab)
  -- keep list sorted
  local add_pos = -1
  for i, elem in ipairs(self.list) do
    if process_tab.name < elem.name then
      add_pos = i
      break
    end
  end
  if add_pos == -1 then
    -- add at end
    table.insert(self.list, process_tab)
  else
    table.insert(self.list, add_pos, process_tab)
  end

  self.super:addWidget(process_tab)
  self:resizeToFit()
end

function lib:removeTab(tab_name)
  for i, v in ipairs(self.list) do
    if v.name == tab_name then
      v.super:__gc() -- delete
      table.remove(self.list, i)
      break
    end
  end
  self:resizeToFit()
end

function lib:resizeToFit()
  local y = padding
  local width = 0
  -- find biggest
  for _, elem in ipairs(self.list) do
    if elem.width > width then
      width = elem.width
    end
    y = y + padding + elem.height
  end

  self:setMinimumSize(width, y-padding)
  self:resize(self.width, y-padding)
end

function lib:resized(w, h)
  self.width  = w
  local y = padding
  for _, elem in ipairs(self.list) do
    elem:move(w - elem.width, y)
    y = y + padding + elem.height
  end
end