--[[------------------------------------------------------

  _control.lk.Slider
  ------------------

  Async slider.

--]]------------------------------------------------------
local lib = lk.SubClass(editor, 'Control')
_control.lk.Slider = lib

function lib:init(name)
  self.name     = name
  self.s        = 0
  self.remote_s = 0
  self:setHue(math.random())
  self:setupConnectors {
    s = 'Slider value',
  }
end

function lib:changed(key, s)
  self.remote_s = s
  self:update()
end

function lib:resized(w, h)
  self.w = w
  self.h = h
end

function lib:setHue(h)
  self.fill_color = mimas.Color(h, 0.5, 0.5)
  self.pen = mimas.Pen(4, mimas.Color(h, 1.0, 1.0))
  self.thumb_color = mimas.Color(h, 0, 1, 0.5)
end

function lib:mouse(x, y)
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

