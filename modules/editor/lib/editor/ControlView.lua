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

function lib:update(changes)
  lk.deepMerge(self, 'cache', changes)
  local widgets = self.widgets
  for id, def in pairs(changes) do
    local widget = widgets[id]
    if v == false then
      if widget then
        -- remove widget
        widget:__gc()
        widgets[id] = nil
      end
      return
    elseif widget then
      -- update
    else
      -- create
      local ctor = editor.Control.getConstructor(def.type)

      if ctor then
        local widget = ctor()
        widget.id = id
        widgets[id] = widget
        widget:set(def, self.zone)
        self:addWidget(widget)
        widget:show()
      end
    end
  end
end

function lib:disconnect(conn)
  if not conn.remove then
    table.insert(self.disconnected, conn)
  end
  conn.node = nil
  -- TODO: mark connector as disabled
  -- connection.connector:disable()
end

--=============================================== 
------ THESE METHODS SHOULD EXIST IN THE CONTROLS (mimas.Slider, etc)
function private:connectToNode(conn)
  local node = conn.process
  for i, part in ipairs(conn.parts) do
    node = node:findNode(part)
  end
  conn.node = node
  node:connectControl(conn)
end

function lib:paint(p, w, h)
  p:fillRect(0, 0, w, h, mimas.Color(0, 0, 0.12))
end
