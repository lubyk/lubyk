--[[------------------------------------------------------

  editor.ControlView
  ------------------

  Displays a control view with all the populated controls.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.ControlView = lib

-- constants
function lib:init(def, zone)
  self.zone = zone
end

function lib:remove()
  if not self:deleted() then
    self:hide()
    self:__gc()
  end
end

