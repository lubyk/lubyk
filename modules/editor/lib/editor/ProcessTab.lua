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
local MouseRelease = mimas.MouseRelease
local box_padding = 1
local edit_padding = 3
local hpen_width = 1 -- half pen width
local bp = hpen_width + box_padding -- full box padding
local arc_radius = 8
local text_padding = 5
local EDIT_WIDTH = 90

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


function lib:click(x, y, op, btn, mod)
  if op == MouseRelease then
    -- open process in PatchingView
    self.process:toggleView()
  end
end

--=============================================== PRIVATE

function private:makeAddBtn()
  self.add_btn = true
  function self:click(x, y, op, btn, mod)
    if op == MouseRelease then
      -- Ask for name and create new process
      self.width = EDIT_WIDTH
      self:setSizeHint(EDIT_WIDTH, self.min_height)
      self:resize(self.min_width, self.min_height)
      self.edit = mimas.LineEdit()
      self:addWidget(self.edit)
      self.edit:resize(EDIT_WIDTH - 2 * edit_padding, self.min_height - 2 * edit_padding)
      self.edit:move(2*edit_padding, edit_padding)
      self.edit:setFocus()
      function self.edit.editingFinished(edit, name)
        self.edit = nil
        edit:hide()
        -- Make sure it is not called a second time
        edit.editingFinished = nil
        self.width = self.old_width
        self:setSizeHint(self.min_width, self.min_height)
        self:resize(self.min_width, self.min_height)
        self.process.machine_view:resizeAll()
        self.process.machine_view:createProcess(name)
      end
    end
  end
end
