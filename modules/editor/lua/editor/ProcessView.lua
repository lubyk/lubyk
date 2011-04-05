--[[------------------------------------------------------

  editor.ProcessView
  ------------------

  The ProcessView is where the patch for a Process is
  displayed and edited.

--]]------------------------------------------------------
local lib = mimas.WidgetClass()
editor.ProcessView = lib

-- constants
local BOX_PADDING = 1
local HPEN_WIDTH = 1 -- half pen width
local BP = HPEN_WIDTH + BOX_PADDING -- full box padding
local ARC_RADIUS = 8
local TEXT_PADDING = 5
local START_DRAG_DIST = 4
local DEFAULT_X = 100
local DEFAULT_Y = 100
local DEFAULT_W = 400
local DEFAULT_H = 400
local DRAG_CORNER = 20

-- This method runs in the GUI thread.
function lib:init(process)
  process.view  = self
  self.process  = process
  self.delegate = process.delegate
  self.nodes    = {}
  self:setName(process.name)
  -- trigger a full view rebuild
  process:updateView()
end

function lib:setName(name)
  self.name  = name
  local w, h = self.super:textSize(name)
  self.lbl_w = w + 4 * TEXT_PADDING -- 2 paddings on sides
  self.lbl_h = h + 2 * TEXT_PADDING
  app:post(function()
    self:update()
  end)
end

function lib:processChanged()
  local process = self.process
  if not process.dragging and not process.resizing then
    -- dragging finished and we have an update: become real
    self.is_ghost        = false
    self.click_position  = nil
    self.resize_position = nil
  end
  self:resize(process.w or DEFAULT_W, process.h or DEFAULT_H)
  self:move(  process.x or DEFAULT_X, process.y or DEFAULT_Y)
  self:update()
end

-- custom paint
function lib:paint(p, w, h)
  local pen   = mimas.Pen(HPEN_WIDTH * 2, mimas.Color(self.process.hue, 0.3, 0.8, 0.5))
  local brush = mimas.Brush(self.process.hue, 0.5, 0.5, 0.5)

  -- label width/height
  local lw = self.lbl_w
  local lh = self.lbl_h
  local path = mimas.Path()

  -- draw rounded surface
  if self.is_ghost then
    p:setBrush(self.process.hue, 0.2, 0.2, 0.3)
    pen = mimas.Pen(HPEN_WIDTH * 2, mimas.Color(self.process.hue, 0.3, 0.8, 0.2))
  else
    p:setBrush(self.process.hue, 0.2, 0.2, 0.5)
  end
  p:setPen(pen)
  p:drawRoundedRect(BP, BP, w - 2 * BP, h - 2 * BP, ARC_RADIUS / 2)

  -- draw label background
  p:setPen(mimas.Pen()) -- no pen
  p:setBrush(brush)

  local pad  = BP + HPEN_WIDTH -- padding for inner shape (top/left)
  path:moveTo(lw+pad, pad)
  path:lineTo(pad + ARC_RADIUS, pad)
  path:cubicTo(pad-1, pad-1, pad-1, pad-1, pad, pad + ARC_RADIUS)
  path:lineTo(pad, lh + pad)
  path:lineTo(lw - ARC_RADIUS + pad, lh + pad)
  path:cubicTo(lw+pad+1, lh+pad+1, lw+pad+1, lh+pad+1, lw+pad, lh+pad-ARC_RADIUS)
  path:lineTo(lw+pad, pad)
  p:drawPath(path)

  -- draw label border (only bottom and right)
  p:setBrush(mimas.Brush())
  p:setPen(pen)
  local pad2 = BP + 2*HPEN_WIDTH -- do not overlap with background
  path = mimas.Path()
  path:moveTo(pad2, lh + pad2)
  path:lineTo(lw - ARC_RADIUS + pad2, lh + pad2)
  path:cubicTo(lw+pad2, lh+pad2, lw+pad2, lh+pad2, lw+pad2, lh+pad2-ARC_RADIUS)
  path:lineTo(lw+pad2, pad2)
  p:drawPath(path)

  -- draw label text
  p:setPen(mimas.Pen(1, mimas.Color(0, 0, 1)))
  p:drawText(pad+2*TEXT_PADDING, pad+TEXT_PADDING, lw - 4*TEXT_PADDING, lh - 2*TEXT_PADDING, mimas.AlignRight + mimas.AlignVCenter, self.name)
end

function lib:delete()
  self.super:__gc()
end

local MousePress,       MouseRelease,       DoubleClick =
      mimas.MousePress, mimas.MouseRelease, mimas.DoubleClick

function lib:resized(w, h)
  self.width  = w
  self.height = h
end

function lib:click(x, y, type, btn, mod)
  local process = self.process
  if type == MousePress then
    local pad = BP + HPEN_WIDTH
    -- only drag when click is in the title
    if x < self.lbl_w + pad and y < self.lbl_h + pad then
      -- store position but only start drag when moved START_DRAG_DIST away
      self.click_position = {x = x, y = y}
      self.current_pos    = {x = process.x, y = process.y}
    elseif x > self.width - DRAG_CORNER and y > self.height - DRAG_CORNER then
      -- resize
      self.resize_position = {x = x, y = y}
    else
      print(x, self.width - DRAG_CORNER, y, self.height - DRAG_CORNER)
    end
  elseif type == DoubleClick then
    -- close ?
  elseif type == MouseRelease then
    if process.resizing then
      -- resizing
      process.resizing = false
      process:change {
        w = self.width,
        h = self.height,
      }
    elseif process.dragging then
      -- drop
      process.dragging = false
      process:change {
        x = self.current_pos.x,
        y = self.current_pos.y,
      }
    end
  end
end

local function manhattanDist(a, b)
  return math.abs(b.x - a.x) + math.abs(b.y - a.y)
end

local function makeGhost(self)
  self.is_ghost = true
end

function lib:mouse(x, y)
  local process = self.process
  if self.click_position and not process.dragging and manhattanDist(self.click_position, {x=x,y=y}) > START_DRAG_DIST then
    -- start drag operation: self becomes ghost
    process.dragging = true
    makeGhost(self)
  elseif self.resize_position and not process.resizing and manhattanDist(self.resize_position, {x=x,y=y}) > START_DRAG_DIST then
    -- start resize operation: self becomes ghost
    process.resizing = true
    makeGhost(self)
  end

  if self.is_ghost then
    if self.click_position then
      -- dragging
      local x = self.current_pos.x + x - self.click_position.x
      local y = self.current_pos.y + y - self.click_position.y
      self:move(x, y)
      self.current_pos = {x = x, y = y}
    elseif self.resize_position then
      -- resizing
      local w = self.width  + x - self.resize_position.x
      local h = self.height + y - self.resize_position.y
      self.resize_position.x = x
      self.resize_position.y = y
      self:resize(w, h)
    end
  end
end