--[[------------------------------------------------------

  _control.lk.Slider
  ------------------

  Async slider.

--]]------------------------------------------------------
local lib = lk.SubClass(editor, 'Control')
_control.lk.Slider = lib

-- default slider size
local DEFAULT = {w = 40, h = 100}
local TEXT_PADDING = 5

function lib:init(id, view)
  self:initControl(id, view)
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
  self.is_horizontal = w > h
end

function lib:control(x, y)
  local s
  if self.is_horizontal then
    local w = self.w
    s = x / w
  else
    local h = self.h
    s = (h - y) / h
  end
  if s < 0 then
    s = 0
  elseif s > 1 then
    s = 1
  end
  self.s = s
  self:update()
  self.conn_s.change(s)
end

local noBrush = mimas.EmptyBrush
local noPen   = mimas.EmptyPen

function lib:paintControl(p, w, h)
  local conn = self.conn_s
  -- remote value
  local v = conn.value
  if self.is_horizontal then
    --=============================================== HORIZONTAL
    local s = self.remote_s * w
    p:fillRect(0, 0, s, h, self.fill_color)
    if self.show_thumb then
      -- thumb
      local t = self.s * w
      local half_thumb = w / 20
      p:fillRect(t-half_thumb, 0, 2*half_thumb, h, self.thumb_color)
      -- show current value
      p:drawText(TEXT_PADDING, 0, w - TEXT_PADDING, h, mimas.AlignLeft + mimas.AlignVCenter, self.conn_s:printValue())
    end
  else
    --=============================================== VERTICAL
    local s = self.remote_s * h
    p:fillRect(0, h-s, w, s, self.fill_color)
    if self.show_thumb then
      -- thumb
      local t = (1 - self.s) * h
      local half_thumb = h / 20
      p:fillRect(0, t-half_thumb, w, 2*half_thumb, self.thumb_color)
      -- show current value
      p:drawText(0, h - TEXT_PADDING - 20, w, 20, mimas.AlignCenter, self.conn_s:printValue())
    end
  end


  -- border
  p:setPen(self.pen)
  p:setBrush(noBrush)
  p:drawRect(0, 0, w, h)
end

