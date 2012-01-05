--[[------------------------------------------------------

  editor.ProcessView
  ------------------

  The ProcessView is where the patch for a Process is
  displayed and edited.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.ProcessView = lib

local START_DRAG_DIST = 4
local DEFAULT_X = 100
local DEFAULT_Y = 100
local DEFAULT_W = 400
local DEFAULT_H = 400
local DRAG_CORNER = 20

-- constants
local BOX_PADDING = 1
local HPEN_WIDTH = 1 -- half pen width
local BP = HPEN_WIDTH + BOX_PADDING -- full box padding
local TEXT_PADDING = 5
local PAD  = BP + HPEN_WIDTH -- padding for inner shape (top/left)
local PAD2 = BP + 2*HPEN_WIDTH -- do not overlap with background

-- This method runs in the GUI thread.
function lib:init(process)
  process.view  = self
  self.process  = process
  self.zone     = process.zone
  self:setHue(process.hue or 0.3)
  self.nodes    = {}
  self:setName(process.name)
end

function lib:setName(name)
  self.name  = name
  self.lbl_w, self.lbl_h = self.super:textSize(name)
  self:update()
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

function lib:animate(max_wait, timeout_clbk)
  if self.thread then
    self.thread:kill()
  end
  self.thread = lk.Thread(function()
    local tab
    if self.process then
      tab = self.process.tab
    end
    local start_time = worker:now()
    local t = 0
    local i = 0
    while t <= max_wait do
      i = i + 1
      sleep(50)
      t = worker:now() - start_time
      -- blink while waiting for creation
      local sat = (0.75 + 0.2 * math.cos(t * math.pi / 750)) % 1.0
      local lbl_back  = mimas.Brush(self.process.hue, sat, sat, 0.5)
      if not self:deleted() then
        self.lbl_back = lbl_back
        self:update()
      end
      if tab and not tab:deleted() then
        tab.brush = lbl_back
        tab:update()
      end
    end
    if timeout_clbk then
      timeout_clbk()
    end
    self.thread = nil
  end)
end

function lib:setHue(hue)
  self.pen_color = mimas.Color(self.process.hue, 0.3, 0.8, 0.5)
  self.lbl_back  = mimas.Brush(self.process.hue, 0.5, 0.5, 0.5)
  self.back      = mimas.Brush(self.process.hue, 0.2, 0.2, 0.5)

  self:update()
end

-- custom paint
function lib:paint(p, w, h)
  local back = self.back
  local pen_color = self.pen_color
  if self.is_ghost then
    -- GHOST
    back = mimas.Brush(self.process.hue, 0.2, 0.2, 0.3)
    pen_color = mimas.Color(self.process.hue, 0.3, 0.8, 0.2)
  end
  if self.online and self == self.zone.process_view_under then
    -- under drag operation
    back = mimas.Brush(self.process.hue, 0.3, 0.3, 0.5)
  end
  editor.paintWithRoundedTitle(p, w, h, self.name, self.lbl_w, self.lbl_h, pen_color, mimas.Color(0, 0, 1), self.lbl_back, back)
end

function lib:delete()
  self.super:__gc()
end

local MousePress,       MouseRelease,       DoubleClick =
      mimas.MousePress, mimas.MouseRelease, mimas.DoubleClick

function lib:resized(w, h)
  self.w = w
  self.h = h
end

-- loop in all links to find click
-- FIXME: optimize: This is why we need to replace the view with
-- mimas.View and Widget by mimas.Item.
local function clickOnLink(self, x, y, type, btn, mod)
  local sx, sy = self:globalPosition()
  -- click global position
  local gx, gy = x + sx, y + sy
  for _, process in pairs(self.process.zone.found_processes) do
    if process.view then
      -- Only test if links are shown
      for _, node in pairs(process.nodes) do
        for _, outlet in ipairs(node.sorted_outlets) do
          for _, link in ipairs(outlet.links) do
            local link_view = link.view
            if link_view then
              -- link_view global position
              local lx, ly = link_view:globalPosition()
              -- position in link_view
              local rx, ry = gx - lx, gy - ly
              if rx >= 0 and rx <= link_view.w and
                ry >= 0 and ry <= link_view.h then

                if link_view.outline:contains(rx, ry) then
                  link_view:click(rx, ry, type, btn, mod)
                  return true
                end
              end
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
  local lw = self.lbl_w + 4 * TEXT_PADDING -- 2 paddings on sides
  local lh = self.lbl_h + 2 * TEXT_PADDING
  if type == MousePress then
    -- only drag when click is in the title
    if x < lw + PAD and y < lh + PAD then
      -- store position but only start drag when moved START_DRAG_DIST away
      self.click_position = {x = x, y = y}
      self.current_pos    = {x = process.x, y = process.y}
    elseif x > self.w - DRAG_CORNER and y > self.h - DRAG_CORNER then
      -- resize
      self.resize_position = {x = x, y = y}
    else
      -- Test if any link is clicked
      clickOnLink(self, x, y, type, btn, mod)
    end
  elseif type == DoubleClick then
    if x < lw + PAD and y < lh + PAD then
      -- close ?
    elseif not clickOnLink(self, x, y, type, btn, mod) then
      -- create new empty node
      local vx, vy = self:globalPosition()
      local node_def = { x = x - 15, y = y - 12, name = '', hue = self.process.hue}
      self.ghost_node = editor.Node.makeGhost(node_def, self.zone)
      local ghost = self.ghost_node
      ghost.gx = vx + node_def.x
      ghost.gy = vy + node_def.y
      ghost:globalMove(ghost.gx, ghost.gy)
      ghost:openEditor(function(abort)
        if not abort then
          node_def.name = ghost.name
          node_def.code = ghost.code
          -- create node
          self.process:newNode(node_def)
          self:update()
        end

        -- clear
        ghost:delete()
        self.ghost_node = nil
      end)
    end
  elseif type == MouseRelease then
    if process.resizing then
      -- resizing
      process.resizing = false
      process:change {
        w = self.w,
        h = self.h,
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
      self.zone:selectNodeView(nil)
      self.zone:selectLinkView(nil)
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
      local w = self.w + x - self.resize_position.x
      local h = self.h + y - self.resize_position.y
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

