--[[------------------------------------------------------

  editor.PatchingView
  -------------------

  Displays all the ProcessViews.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.PatchingView = lib

-- constants

function lib:init()

end

function lib:resized(w, h)
  self.width  = w
  self.height = h
end

function lib:showProcessView(process_view)
  self.super:addWidget(process_view)
end

--function lib:paint(p, w, h)
--  p:fillRect(0, 0, w, h, mimas.colors.Blue:colorWithAlpha(0.2))
--end
