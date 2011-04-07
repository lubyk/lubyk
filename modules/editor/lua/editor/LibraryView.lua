--[[------------------------------------------------------

  editor.LibraryView
  ------------------

  Displays the LibraryView.

--]]------------------------------------------------------
local lib = mimas.WidgetClass()
editor.LibraryView = lib

-------------------------------------------------------- PRIVATE
-- constants
local WIDTH       = 120
local PADDING     = 10
local ROW_HEIGHT  = 10
local BAR_HEIGHT  = 50 -- toolbar at the top
local MIN_HEIGHT  = BAR_HEIGHT + ROW_HEIGHT + 50

local function placeElements(self)
  local w, h = self.width, self.height
  self.list_view:move(0, BAR_HEIGHT)
  self.list_view:resize(self.width, self.height - BAR_HEIGHT)
  self.list_view:update()
end

-------------------------------------------------------- PUBLIC
function lib:init(library)
  self.library = library

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