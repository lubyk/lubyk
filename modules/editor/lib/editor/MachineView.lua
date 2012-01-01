--[[------------------------------------------------------

  editor.MachineView
  ------------------

  Displays the list of ProcessTabs.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.MachineView = lib

-- constants
local WIDTH       = 120
local PADDING     = 10
-- half pen width
local HPEN_W      = 1

-- Structure of boxes:
--
-- lay
-- +----------------+
-- |   title        |
-- +----------------+
-- | hbox           |
-- | +---------+    |
-- | |  vbox   |    |
-- | +---------+    |
-- +----------------+
-- | stretch        |
-- +----------------+
--
function lib:init(machine)
  self.process_list = {}
  self.machine = machine
  self.lay = mimas.VBoxLayout(self)
  self.lay:setContentsMargins(0,0,0,0)
  self.title = mimas.Widget()
  self.title:setSizePolicy(mimas.Minimum, mimas.Fixed)
  self.lay:addWidget(self.title)
  self.hbox = mimas.HBoxLayout()
  self.vbox = mimas.VBoxLayout()
  self.vbox:setContentsMargins(10,0,0,5)
  self.hbox:addWidget(self.vbox)
  self.lay:addWidget(self.hbox)
  self:addProcess {name = '+', hue = 0, add_btn = true, machine_view = machine_view}
  self:setName(machine.name)
end

function lib:createProcess(name)
  -- show dialog for name and tell machine to create
  -- a new process
end

function lib:setName(name)
  self.name  = name
  self.lbl_w, self.lbl_h = self.super:textSize(name)
  local w = self.lbl_w + 30
  local h = self.lbl_h + 15
  self.title:setSizeHint(w, h)
  self.title:resize(w, h)
  self:update()
end

function lib:addProcess(process)
  local process_tab = editor.ProcessTab(process)
  process.tab = process_tab
  -- keep list sorted
  local add_pos = -1
  for i, elem in ipairs(self.process_list) do
    if process_tab.name < elem.name or elem.add_btn then
      add_pos = i
      break
    end
  end
  if add_pos == -1 then
    -- add at end
    table.insert(self.process_list, process_tab)
    self.vbox:insertWidget(-1, process_tab, 0, mimas.AlignRight)
  else
    table.insert(self.process_list, add_pos, process_tab)
    self.vbox:insertWidget(add_pos-1, process_tab, 0, mimas.AlignRight)
  end
end

function lib:removeProcess(process_name)
  for i, v in ipairs(self.process_list) do
    if v.name == process_name then
      v.super:__gc() -- delete
      table.remove(self.process_list, i)
      break
    end
  end
end

-- custom paint
function lib:paint(p, w, h)
  local pen_color = mimas.Color(0, 0, 0.25, 0.5)
  local lbl_back  = mimas.Brush(0, 0, 0.25, 0.5)
  local back      = mimas.Brush() --0, 0, 0.2, 0.5)

  -- Add 40 to go beyond border
  editor.paintWithRoundedTitle(p, w + 40, h, self.name, self.lbl_w, self.lbl_h, pen_color, mimas.Color(0, 0, 0.8), lbl_back, back)
end
