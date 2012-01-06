--[[------------------------------------------------------

  editor.ControlView
  ------------------

  Displays a control view with all the populated controls.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.ControlView = lib

local private = {}

-- constants
function lib:init(name, def, zone)
  self.name = name
  self.zone = zone
  self.widgets = {}
  -- Update view from definition
  self:update(def)
end

function lib:remove()
  if not self:deleted() then
    self:hide()
    self:__gc()
  end
end

function lib:update(changes)
  local widgets = self.widgets
  for id, def in pairs(changes) do
    local widget = widgets[id]
    if v == false then
      if widget then
        -- remove widget
        widget:__gc()
        widgets[id] = nil
      end
    elseif widget then
      -- update
    else
      -- create
      local typ = def.type
      -- TODO: we could try to retrieve from _G but this would
      -- allow arbitrary code execution on the GUI.... Not sure we want this...
      if typ == 'mimas.Slider' then
        local widget = mimas.Slider(def.orientation or mimas.Vertical)
        widget.id = id
        widgets[id] = widget
        self:addWidget(widget)
        private.placeWidget(self, widget, def)
        private.connectWidget(self, widget, def)
      end
    end
  end
end

function private:placeWidget(widget, def)
  if def.x or def.y then
    local x = def.x or widget:x()
    local y = def.y or widget:y()
    widget:move(x, y)
  end

  if def.w or def.h then
    local w = def.w or widget:width()
    local h = def.h or widget:height()
    widget:resize(w, h)
  end
  if def.hue then
    widget:setHue(def.hue)
  end
end

function private:connectWidget(widget, def)
end
