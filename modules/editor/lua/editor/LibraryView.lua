--[[------------------------------------------------------

  editor.LibraryView
  ------------------

  Displays the LibraryView.

--]]------------------------------------------------------
local lib = mimas.WidgetClass()
editor.LibraryView = lib

-- constants
local WIDTH       = 120
local PADDING     = 10
local MIN_HEIGHT  = 50

function lib:init()
  self.width  = WIDTH
  self.height = MIN_HEIGHT
  self:setSizePolicy(mimas.Fixed, mimas.Minimum)
  self:setSizeHint(WIDTH, MIN_HEIGHT)
  self:resize(self.width, self.height)
end

function lib:resized(w, h)
  self.width  = w
  self.height = h
end

function lib:paint(p, w, h)
  p:fillRect(0, 0, w, h, mimas.Color(0, 0, 0.18))
end
