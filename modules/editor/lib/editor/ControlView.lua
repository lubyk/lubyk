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
  self.cache = {}
  self.name = name
  self.zone = zone
  self.morph= zone.morph
  -- Contains a list of callbacks to trigger on view
  -- update. The callbacks are called with the changed parameters
  -- before the changes are applied and are removed before trigger.
  self.on_update_callbacks = {}

  -- List of widgets by id.
  self.widgets = {}
  -- Update view from definition.
  self:update(def)
end

function lib:remove()
  if not self:deleted() then
    self:hide()
    self:__gc()
  end
end

-- When new widgets are created, provide a free
-- name.
function lib:nextName(typ)
  local typ  = string.lower(string.match(typ, '%.([^%.]+)$') or typ)
  local fmt  = string.format
  local base = string.sub(typ, 1, 1)
  local widgets = self.widgets
  local i = 0
  local key
  repeat
    i = i + 1
    key = fmt('%s%i', base, i)
  until not widgets[key]
  return key
end

local getControl = editor.Control.getControl

-- This is a callback when the view is remotely changed.
function lib:update(changes)
  lk.deepMerge(self, 'cache', changes)
  local widgets = self.widgets
  for id, def in pairs(changes) do
    private.triggerUpdateCallbacks(self, id, def)
    local widget = widgets[id]
    if def == false then
      if widget then
        -- remove widget
        widget:delete()
        widgets[id] = nil
      end
    elseif not widget then
      -- new
      local ctor = getControl(def.type)

      if ctor then
        widget = ctor(id, self)
        widgets[id] = widget
        widget:set(def)
        self:addWidget(widget)
        widget:show()
      end
    else
      -- update
      -- zone is needed to create/update connections
      widget:set(def, self.zone)
    end
  end
end

function lib:onUpdate(id, callback)
  local list = self.on_update_callbacks[id]
  if not list then
    list = {}
    self.on_update_callbacks[id] = list
  end
  table.insert(list, callback)
end

function private:triggerUpdateCallbacks(id, changes)
  local list = self.on_update_callbacks[id]
  if list then
    self.on_update_callbacks[id] = nil
    for _, clbk in ipairs(list) do
      clbk(changes)
    end
  end
end

-- This is called by the GUI when user updates controls in the view.
function lib:change(def)
  self.morph:change {
    _views = {
      [self.name] = def,
    },
  }
end

function lib:paint(p, w, h)
  p:fillRect(0, 0, w, h, mimas.Color(0, 0, 0.12))
end
