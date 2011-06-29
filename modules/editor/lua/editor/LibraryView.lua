--[[------------------------------------------------------

  editor.LibraryView
  ------------------

  Displays the LibraryView.

--]]------------------------------------------------------
local lib = mimas.WidgetClass('editor.LibraryView')
editor.LibraryView = lib

-------------------------------------------------------- PRIVATE
-- constants
local WIDTH       = 120
local PADDING     = 10
local ROW_HEIGHT  = 10
local BAR_HEIGHT  = 50 -- toolbar at the top
local MIN_HEIGHT  = BAR_HEIGHT + ROW_HEIGHT + 50
local DRAG_POS_X  = 10
local DRAG_POS_Y  = 10
local START_DRAG_DIST = 4

local function placeElements(self)
  local w, h = self.width, self.height
  self.list_view:move(0, BAR_HEIGHT)
  self.list_view:resize(self.width, self.height - BAR_HEIGHT)
  self.list_view:update()
end

local MousePress,       MouseRelease,       DoubleClick =
      mimas.MousePress, mimas.MouseRelease, mimas.DoubleClick

local function clickInList(self, node_def, x, y, type, btn, mod)
  if type == MousePress and node_def then
    -- store position but only start drag when moved START_DRAG_DIST away
    self.click_position = {
      x        = x,
      y        = y,
      node_def = node_def,
    }

  elseif type == DoubleClick then
    -- noop
  elseif type == MouseRelease then
    if self.dragging then
      -- drop
      self.ghost:openEditor(function()
        local node_def = self.click_position.node_def
        node_def.name = self.ghost.edit:text()
        local process_view = self.delegate.process_view_under
        if process_view then
          local process = process_view.process
          -- create node
          -- target:change {}
          local vx, vy = process_view:globalPosition()
          local x = self.ghost.gx - vx
          local y = self.ghost.gy - vy
          process:newNode {
            x = self.ghost.gx - vx,
            y = self.ghost.gy - vy,
            code = node_def.code,
            name = node_def.name,
          }
          process_view:update()
        end

        -- clear
        self.delegate.process_view_under = nil
        self.ghost:delete()
        self.ghost = nil
        self.click_position = nil
        self.dragging = nil
      end)
    else
      -- select row ?
    end
  end
  return false -- Pass to normal list selection and click handling
end

local function manhattanDist(a, b)
  return math.abs(b.x - a.x) + math.abs(b.y - a.y)
end

local function mouseInList(self, x, y)
  if not self.dragging and self.click_position and manhattanDist(self.click_position, {x=x,y=y}) > START_DRAG_DIST then
    -- start drag operation: self becomes ghost
    self.dragging = true
    self.gx, self.gy = self.list_view:globalPosition()
    -- mock a node for NodeView
    self.ghost = editor.Node.makeGhost(self.click_position.node_def, self.delegate)
  end

  local ghost = self.ghost
  if ghost then
    ghost.gx = self.gx + x - DRAG_POS_X
    ghost.gy = self.gy + y - DRAG_POS_Y
    ghost:globalMove(ghost.gx, ghost.gy)
    local delegate = self.delegate
    local old_process_view_under = delegate.process_view_under
    delegate.process_view_under = delegate:processViewAtGlobal(ghost.gx + DRAG_POS_X, ghost.gy + DRAG_POS_Y)
    if delegate.process_view_under then
      delegate.process_view_under:update()
    end
    if old_process_view_under then
      old_process_view_under:update()
    end
  end
end

-------------------------------------------------------- PUBLIC
function lib:init(library)
  self.library  = library
  self.delegate = library.delegate

  --============ ListView ===============
  -- Display list of found prototypes
  self.list_view = mimas.ListView()
  self.super:addWidget(self.list_view)
  function self.list_view.rowCount()
    return library:nodeCount()
  end

  function self.list_view.data(row_i)
    return library:node(row_i).name
  end

  function self.list_view.click(x, y, type, btn, mod)
    local row_i, node_def = self.list_view:indexAt(x, y)
    if row_i then
      node_def = library:node(row_i)
    end
    return clickInList(self, node_def, x, y, type, btn, mod)
  end

  function self.list_view.mouse(x, y)
    mouseInList(self, x, y)
  end
  --=====================================

  self.width  = WIDTH
  self.height = MIN_HEIGHT
  self:setSizePolicy(mimas.Fixed, mimas.Minimum)
  self:setSizeHint(WIDTH, MIN_HEIGHT)
  self:resize(self.width, self.height)
  placeElements(self)
end

function lib:resized(w, h)
  self.width  = w
  self.height = h
  placeElements(self)
end

function lib:paint(p, w, h)
  p:fillRect(0, 0, w, h, mimas.Color(0, 0, 0.18))
end
