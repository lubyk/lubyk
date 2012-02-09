--[[------------------------------------------------------

  editor.PatchView
  -------------------

  Displays all the ProcessViews.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.PatchView = lib

-- constants

function lib:init(zone_view)
  self.zone_view = zone_view
  self.zone = zone_view.zone
end

function lib:resized(w, h)
  self.w = w
  self.h = h
  self.zone_view.machine_list:updatePosition()
end

function lib:showProcessView(process_view)
  self.super:addWidget(process_view)
end

function lib:hide()
  self.super:hide()
  if self.zone_view.machine_list then
    self.zone_view.machine_list:patchViewChanged(false)
  end
end

function lib:show()
  self.super:show()
  if self.zone_view.machine_list then
    self.zone_view.machine_list:patchViewChanged(true)
  end
end
