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

local PAD  = BP + HPEN_WIDTH -- padding for inner shape (top/left)
local PAD2 = BP + 2*HPEN_WIDTH -- do not overlap with background

-- This method runs in the GUI thread.
function lib:init(process)
  process.view  = self
  self.process  = process
  self.delegate = process.delegate
  self.nodes    = {}
  self:setName(process.name)
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
  elseif self == self.delegate.process_view_under then
    -- under drag operation from Library
    p:setBrush(self.process.hue, 0.3, 0.3, 0.5)
  else
    p:setBrush(self.process.hue, 0.2, 0.2, 0.5)
  end
  p:setPen(pen)
  p:drawRoundedRect(BP, BP, w - 2 * BP, h - 2 * BP, ARC_RADIUS / 2)

  -- draw label background
  p:setPen(mimas.Pen()) -- no pen
  p:setBrush(brush)

  path:moveTo(lw+PAD, PAD)
  path:lineTo(PAD + ARC_RADIUS, PAD)
  path:cubicTo(PAD-1, PAD-1, PAD-1, PAD-1, PAD, PAD + ARC_RADIUS)
  path:lineTo(PAD, lh + PAD)
  path:lineTo(lw - ARC_RADIUS + PAD, lh + PAD)
  path:cubicTo(lw+PAD+1, lh+PAD+1, lw+PAD+1, lh+PAD+1, lw+PAD, lh+PAD-ARC_RADIUS)
  path:lineTo(lw+PAD, PAD)
  p:drawPath(path)

  -- draw label border (only bottom and right)
  p:setBrush(mimas.Brush())
  p:setPen(pen)

  path = mimas.Path()
  path:moveTo(PAD2, lh + PAD2)
  path:lineTo(lw - ARC_RADIUS + PAD2, lh + PAD2)
  path:cubicTo(lw+PAD2, lh+PAD2, lw+PAD2, lh+PAD2, lw+PAD2, lh+PAD2-ARC_RADIUS)
  path:lineTo(lw+PAD2, PAD2)
  p:drawPath(path)

  -- draw label text
  p:setPen(mimas.Pen(1, mimas.Color(0, 0, 1)))
  p:drawText(PAD+2*TEXT_PADDING, PAD+TEXT_PADDING, lw - 4*TEXT_PADDING, lh - 2*TEXT_PADDING, mimas.AlignRight + mimas.AlignVCenter, self.name)
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

-- loop in all links to find click
local function clickOnLink(self, x, y, type, btn, mod)
  local sx, sy = self:globalPosition()
  -- click global position
  local gx, gy = x + sx, y + sy
  for _, node in pairs(self.process.nodes) do
    for _, outlet in ipairs(node.sorted_outlets) do
      for _, link in ipairs(outlet.links) do
        local link_view = link.view
        if link_view then
          -- link_view global position
          local lx, ly = link_view:globalPosition()
          -- position in link_view
          local rx, ry = gx - lx, gy - ly
          if rx >= 0 and rx <= link_view.width and
            ry >= 0 and ry <= link_view.height then

            if link_view.outline:contains(rx, ry) then
              link_view:click(rx, ry, type, btn, mod)
              return true
            end
          end
        end
      end
    end
  end
  return false
end

function lib:click(x, y, type, btn, mod)
  local process = self.process
  if type == MousePress then
    -- only drag when click is in the title
    if x < self.lbl_w + PAD and y < self.lbl_h + PAD then
      -- store position but only start drag when moved START_DRAG_DIST away
      self.click_position = {x = x, y = y}
      self.current_pos    = {x = process.x, y = process.y}
    elseif x > self.width - DRAG_CORNER and y > self.height - DRAG_CORNER then
      -- resize
      self.resize_position = {x = x, y = y}
    else
      -- Test if any link is clicked
      clickOnLink(self, x, y, type, btn, mod)
    end
  elseif type == DoubleClick then
    if x < self.lbl_w + PAD and y < self.lbl_h + PAD then
      -- close ?
    elseif not clickOnLink(self, x, y, type, btn, mod) then
      -- create new empty node
      process:newNode { x = x - 30, y = y - 10 }
    end
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
    elseif not clickOnLink(self, x, y, type, btn, mod) then
      -- deselect all
      self.delegate:selectNodeView(nil)
      self.delegate:selectLinkView(nil)
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
      -- redraw global links
      self.process:redrawViews()
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

local function distanceToSlot(gx, gy, view)
  local vx, vy = view:globalPosition()
  vx = gx - vx
  vy = gy - vy
  return math.sqrt(vx*vx + vy*vy)
end

local SLOTW = editor.SlotView.SLOTW
local SLOTH = editor.SlotView.SLOTH

--- Find the closest slot view from the global x and global y coordinates.
-- @return closest slot and distance to center of slot.
function lib:closestSlotView(gx, gy, for_type, skip_node)
  gx = gx - SLOTW/2
  gy = gy - SLOTH/2
  -- find the closest slot
  local d, view, list_name = nil
  if for_type == 'editor.Outlet' then
    list_name = 'sorted_inlets'
  else
    list_name = 'sorted_outlets'
  end

  for _, node in pairs(self.process.nodes) do
    if node ~= skip_node then
      for _, slot in ipairs(node[list_name]) do
        local di = distanceToSlot(gx, gy, slot.view)
        if not d or di < d then
          d = di
          view = slot.view
        end
      end
    end
  end
  return view, d
end

