--[[------------------------------------------------------

  editor.ProcessList
  ------------------

  Displays the list of ProcessTabs.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.ProcessList = lib

-- constants
local WIDTH       = 120
local PADDING_TOP = 0
-- space between elements
local PADDING_E   = 5
local PADDING_L   = 3

function lib:init(machine_view)
  self.list    = {}
  self.machine_view = machine_view
  self.width   = WIDTH
  self:setSizePolicy(mimas.Fixed, mimas.Minimum)
  self:addProcess {name = '+', hue = 0, add_btn = true, machine_view = machine_view}
end

function lib:addProcess(process)
  local process_tab = editor.ProcessTab(process)
  process.tab = process_tab
  -- keep list sorted
  local add_pos = -1
  for i, elem in ipairs(self.list) do
    if process_tab.name < elem.name or elem.add_btn then
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
      v.super:__gc() -- delete
      table.remove(self.list, i)
      break
    end
  end
  self:resizeToFit()
end

local function placeElements(self, w)
  local y = PADDING_TOP
  for _, elem in ipairs(self.list) do
    elem:move(w - elem.width, y)
    y = y + PADDING_E + elem.height
  end
end

function lib:resizeToFit()
  local y = PADDING_TOP
  local width = PADDING_L
  -- find biggest
  for _, elem in ipairs(self.list) do
    if elem.width > width then
      width = elem.width + PADDING_L
    end
    y = y + PADDING_E + elem.height
  end

  self:setSizeHint(width, y)
  placeElements(self, width)
  self:resize(width, y)
  self.width  = width
  self.height = y
end

function lib:resized(w, h)
  self.width  = w
  placeElements(self, w)
end

