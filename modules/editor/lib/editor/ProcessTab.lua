--[[------------------------------------------------------

  editor.ProcessTab
  -----------------

  Small tab on the right side of the PatchView or left
  side of the ControlView.

  The ProcessTab contains information on the remote
  Process such as:
   - name
   - alerts

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.ProcessTab = lib

-- constants
local private = {}
local box_padding = 1
local edit_padding = 3
local hpen_width = 1 -- half pen width
local bp = hpen_width + box_padding -- full box padding
local arc_radius = 8
local text_padding = 5
local EDIT_WIDTH = 80

function lib:init(process)
  self.process = process
  self:setName(process.name)
  self:setSizePolicy(mimas.Minimum, mimas.Fixed)
  if process.add_btn then
    private.makeAddBtn(self)
    self.pen   = mimas.Pen(hpen_width * 2, mimas.Color(0.3, 0.3, 0.8, 0.5), mimas.DotLine)
    self.brush = mimas.Brush(mimas.Color(0.3, 0.3, 0.3, 0.5))
  else
    self:setHue(process.hue)
  end
end

function lib:setHue(hue)
  self.pen   = mimas.Pen(hpen_width * 2, mimas.Color(self.process.hue, 0.3, 0.8, 0.8))
  self.brush = mimas.Brush(mimas.Color(self.process.hue, 0.3, 0.3, 0.8))
end

function lib:setName(name)
  self.name  = name
  local w, h = self.super:textSize(name)
  self.min_width  = w + 4 * text_padding -- 2 paddings on sides
  self.min_height = h + 2 * text_padding
  self:setSizeHint(self.min_width, self.min_height)
end

-- custom paint
function lib:paint(p, w, h)
  p:setBrush(self.brush)
  p:setPen(self.pen)
  p:drawRoundedRect(bp, bp, w + arc_radius + bp, h - 2 * bp, arc_radius)
  p:setPen(mimas.Pen(1, mimas.Color(0, 0, 1)))
  p:drawText(2 * text_padding, text_padding, w - 4*text_padding, h - 2*text_padding, mimas.AlignRight + mimas.AlignVCenter, self.name)
end

local MousePress, MouseRelease = mimas.MousePress, mimas.MouseRelease

function lib:click(x, y, op, btn, mod)
  if op == MouseRelease then
    -- open process in PatchingView
    self.process:toggleView()
  end
end

--=============================================== PRIVATE
local addbtn = {addbtn = true}
addbtn.__index = addbtn
setmetatable(addbtn, lib)

local START_DRAG_DIST = 4

function private:makeAddBtn()
  self.machine = self.process.machine
  self.zone    = self.machine.zone
  setmetatable(self, addbtn)
end

function addbtn:click(x, y, op, btn, mod)
  if op == MousePress then
    self.click_position = {x=x,y=y}
    local gx, gy = self:globalPosition()
    self.base_pos = {gx = gx, gy = gy}
  elseif op == MouseRelease then
    if self.dragging then
      -- drop
      self.dragging = false
      local gx, gy = self.ghost:globalPosition()
      local px, py = self.zone.main_view.patching_view:globalPosition()
      self.ghost.def = {gx - px, gy - py}
      -- Ask for name and create new process
      self.ghost.lbl_w = EDIT_WIDTH - 10
      self.ghost:update()
      self.edit = mimas.LineEdit()
      self.ghost:addWidget(self.edit)
      self.edit:resize(EDIT_WIDTH, self.min_height - edit_padding)
      self.edit:move(2*edit_padding, edit_padding)
      self.edit:setFocus()
      function self.edit.editingFinished(edit, name)
        self.ghost:setName(name)
        self.ghost.def.name = name
        self.edit:hide()
        self.edit = nil
        -- Make sure it is not called a second time
        edit.editingFinished = nil
        self.machine:createProcess(self.ghost.def)
        -- TODO: keep ghost visible for some time and blink until it becomes real
        self.ghost = nil
      end
    end
  end
end

local function manhattanDist(a, b)
  return math.abs(b.x - a.x) + math.abs(b.y - a.y)
end

function addbtn:mouse(x, y)
  local zone = self.zone
  local main_view = zone.main_view
  if self.click_position and not self.dragging and manhattanDist(self.click_position, {x=x,y=y}) > START_DRAG_DIST then
    -- start drag operation: self becomes ghost
    self.dragging = true
    self.ghost = editor.ProcessView { name = '', hue = math.random(), nodes = {}, pending_inlets = {}, delegate = zone }
    self.ghost.is_ghost = true
    self.ghost:resize(EDIT_WIDTH + 20,100)
    main_view:addWidget(self.ghost)
  end

  if self.dragging then
    -- dragging
    local gx = self.base_pos.gx + x - self.click_position.x
    local gy = self.base_pos.gy + y - self.click_position.y
    self.ghost:globalMove(gx, gy)
  end
end









