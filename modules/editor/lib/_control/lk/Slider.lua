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
  self:setupConnectors {
    s = 'Slider value',
  }
  self:resize(DEFAULT.w, DEFAULT.h)
end

function lib:resized(w, h)
  self.w = w
  self.h = h
  self.is_horizontal = w > h
end

function lib:control(x, y)
  local s
  if self.is_horizontal then
    s = x / self.w
  else
    local h = self.h
    s = (h - y) / h
  end
  self.conn_s.change(s)
  self:update()
end

local noBrush = mimas.EmptyBrush
local noPen   = mimas.EmptyPen

function lib:paintControl(p, w, h)
  local cs = self.conn_s

  if self.is_horizontal then
    --=============================================== HORIZONTAL
    local s = cs.remote_value * w
    p:fillRect(0, 0, s, h, self.fill_color)
    if self.show_thumb then
      -- thumb
      local t = cs.value * w
      local half_thumb = w / 20
      p:setBrush(self.thumb_color)
      p:drawRect(t-half_thumb, 0, 2*half_thumb, h)
      -- show current value
      p:drawText(TEXT_PADDING, 0, w - TEXT_PADDING, h, mimas.AlignLeft + mimas.AlignVCenter, cs:printValue())
    end
  else
    --=============================================== VERTICAL
    local s = cs.remote_value * h
    p:fillRect(0, h-s, w, s, self.fill_color)
    if self.show_thumb then
      -- thumb
      local t = (1 - cs.value) * h
      local half_thumb = h / 20
      p:setBrush(self.thumb_color)
      p:drawRect(0, t-half_thumb, w, 2*half_thumb)
      -- show current value
      p:drawText(0, h - TEXT_PADDING - 20, w, 20, mimas.AlignCenter, cs:printValue())
    end
  end


  -- border
  p:setPen(self.pen)
  p:setBrush(noBrush)
  p:drawRect(0, 0, w, h)
end

