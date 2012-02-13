--[[------------------------------------------------------

  _control.lk.Slider
  ------------------

  Async slider.

--]]------------------------------------------------------
local lib = lk.SubClass(editor, 'Control')
_control.lk.Slider = lib

-- default slider size
local DEFAULT = {w = 40, h = 100}

function lib:init(name)
  self:initControl(name)
  self.s        = 0
  self.remote_s = 0
  self:setupConnectors {
    s = 'Slider value',
  }
  self:resize(DEFAULT.w, DEFAULT.h)
end

function lib:changed(key, s)
  self.remote_s = s
  self:update()
end

function lib:resized(w, h)
  self.w = w
  self.h = h
end

function lib:mouse(x, y)
  if not self.enabled then
    return
  end
  local h = self.h
  local s = (h - y) / h
  if s < 0 then
    s = 0
  elseif s > 1 then
    s = 1
  end
  self.s = s
  self:update()
  self.conn_s.change(s)
end

lib.click = lib.mouse

local noBrush = mimas.EmptyBrush
local noPen   = mimas.EmptyPen

function lib:paint(p, w, h)
  if self.is_ghost then
    return self:paintGhost(p, w, h)
  end
  -- remote value
  local s = self.remote_s * h
  p:fillRect(0, h-s, w, s, self.fill_color)

  -- thumb
  local t = (1 - self.s) * h
  local half_thumb = h / 20
  p:fillRect(0, t-half_thumb, w, 2*half_thumb, self.thumb_color)

  -- border
  p:setPen(self.pen)
  p:setBrush(noBrush)
  p:drawRect(0, 0, w, h)
end

