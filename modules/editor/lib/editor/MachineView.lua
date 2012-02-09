--[[------------------------------------------------------

  editor.MachineView
  ------------------

  Displays the list of ProcessTabs.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.MachineView = lib

-- constants
local WIDTH       = 120
local PADDING     = 5
-- half pen width
local HPEN_W      = 1

-- Structure of boxes:
--
-- lay
-- +----------------+
-- |   title        |
-- +----------------+
-- |   vbox         |
-- +----------------+
-- |   stretch      |
-- +----------------+
--
function lib:init(machine)
  self.process_list = {}
  self.machine = machine
  self.zone    = machine.zone
  self.lay = mimas.VBoxLayout(self)
  self.lay:setContentsMargins(0,0,0,0)
  self.lay:setSpacing(PADDING)
  self.title = mimas.Widget()
  self.title:setSizePolicy(mimas.Minimum, mimas.Fixed)
  self.lay:addWidget(self.title)
  self.vbox = mimas.VBoxLayout()
  self.lay:addWidget(self.vbox)

  self:setName(machine.name)
  self:layoutChanged(editor.Settings.show.Patch, editor.Settings.show.View)
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

function lib:addProcess(service)
  local name = service.name
  local view
  if name == '' then
    -- morph
    view = editor.MorphTab(service)
  elseif service.stem_name then
    -- stem cell
    view = editor.StemTab(service)
  else
    -- process
    view = editor.ProcessTab(service)
  end

  -- keep list sorted
  local add_pos = -1
  for i, elem in ipairs(self.process_list) do
    if view.name < elem.name then
      add_pos = i
      break
    end
  end
  if add_pos == -1 then
    -- add at end
    table.insert(self.process_list, view)
    self.vbox:insertWidget(-1, view, 0, mimas.AlignRight)
  else
    table.insert(self.process_list, add_pos, view)
    self.vbox:insertWidget(add_pos-1, view, 0, mimas.AlignRight)
  end
  self.machine_list:updatePosition()
end

function lib:removeProcess(service_name)
  for i, v in ipairs(self.process_list) do
    if v.name == service_name then
      v.super:__gc() -- delete
      table.remove(self.process_list, i)
      break
    end
  end
  self.machine_list:updatePosition()
end

-- custom paint
function lib:paint(p, w, h)
  local pen_color = mimas.Color(0, 0, 0.25, 0.5)
  local lbl_back  = mimas.Brush(0, 0, 0.25, 0.5)
  local back      = mimas.Brush(0, 0, 0, 0.80)

  -- Add 40 to go beyond border
  editor.paintWithRoundedTitle(p, w + self.pad_right, h, self.name, self.lbl_w, self.lbl_h, pen_color, mimas.Color(0, 0, 0.8), lbl_back, back, self.lbl_align, self.pad_left)
end

function lib:layoutChanged(patch_visible, controls_visible)
  local align = 0
  if controls_visible then
    align = 1
    self.pad_right = 0
  else
    self.pad_right = 40
  end

  if patch_visible then
    align = align + 2
    self.pad_left = 0
  else
    self.pad_left = -40
  end

  if align == 0 then
    -- none visible
    self.pad_left  = 0
    self.pad_right = 0
    self.align = mimas.AlignCenter
    self.tab_align = mimas.AlignCenter
    self.vbox:setContentsMargins(10,0,10,5)
  elseif align == 3 then
    -- both visible
    self.align = mimas.AlignCenter
    self.tab_align = mimas.AlignCenter
    self.vbox:setContentsMargins(10,0,10,5)
  elseif align == 2 then
    -- label left, tabs to the right
    self.lbl_align = mimas.AlignLeft
    self.tab_align = mimas.AlignRight
    self.vbox:setContentsMargins(10,0,0,5)
  else
    -- label right, tabs to the left
    self.lbl_align = mimas.AlignRight
    self.tab_align = mimas.AlignLeft
    self.vbox:setContentsMargins(0,0,10,5)
  end

  for _, view in ipairs(self.process_list) do
    self.vbox:setAlignment(view, self.tab_align)
    view:setAlignment(self.tab_align)
  end

  self:update()
end

function lib:minimumSize()
  local w, h = self.lbl_w + 30, self.lbl_h + 8
  for _, tab in ipairs(self.process_list) do
    local pw, ph = tab:minimumSize()
    w = math.max(pw, w)
    h = h + ph + PADDING
  end
  return w, h
end
