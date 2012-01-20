--[[------------------------------------------------------

  editor.PatchView
  -------------------

  Displays all the ProcessViews.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.PatchView = lib

-- constants

function lib:init(zone)
  self.zone = zone
end

function lib:resized(w, h)
  self.w = w
  self.h = h
  print('resized', w, h)
  self.zone.machine_list:updatePosition()
end

function lib:showProcessView(process_view)
  self.super:addWidget(process_view)
end

function lib:hide()
  self.super:hide()
  self.zone.machine_list:patchViewChanged(false)
end

function lib:show()
  self.super:show()
  self.zone.machine_list:patchViewChanged(true)
end
