--[[------------------------------------------------------

  editor.ProcessList
  ------------------

  Displays the list of ProcessTabs.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.ProcessList = lib

-- constants
local WIDTH       = 120
local PADDING     = 10

function lib:init()
  self.list  = {}
  self.width = WIDTH
  self:setSizePolicy(mimas.Fixed, mimas.Minimum)
end

function lib:addProcess(process)
  local process_tab = editor.ProcessTab(process)
  process.tab = process_tab
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

function lib:removeProcess(process_name)
  for i, v in ipairs(self.list) do
    if v.name == process_name then
      app:post(function()
        v.super:__gc() -- delete
      end)
      table.remove(self.list, i)
      break
    end
  end
  self:resizeToFit()
end

local function placeElements(self, w)
  local y = PADDING
  for _, elem in ipairs(self.list) do
    elem:move(w - elem.width, y)
    y = y + PADDING + elem.height
  end
end

function lib:resizeToFit()
  local y = PADDING
  local width = 0
  -- find biggest
  for _, elem in ipairs(self.list) do
    if elem.width > width then
      width = elem.width
    end
    y = y + PADDING + elem.height
  end

  self:setSizeHint(width, y - PADDING)
  self:resize(width, y - PADDING)
end

function lib:resized(w, h)
  self.width  = w
  placeElements(self, w)
end

-- function lib:paint(p, w, h)
--   p:fillRect(0, 0, w, h, mimas.colors.Green:colorWithAlpha(0.2))
-- end
