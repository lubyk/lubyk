--[[------------------------------------------------------

  _control.lk.Pad
  ------------------

  Async slider.

--]]------------------------------------------------------
local lib = lk.SubClass(editor, 'Control')
_control.lk.Pad = lib

local const = _control.lk.Slider.const
local TEXT_PADDING = 5

function lib:init(id, view)
  self:initControl(id, view)
  self:setupConnectors {
    x = 'Pad x value',
    y = 'Pad y value',
  }
  self:resize(const.DEFAULT.h, const.DEFAULT.h)
end

function lib:resized(w, h)
  self.w = w
  self.h = h
end

function lib:control(x, y)
  self.conn_x.change(x / self.w)
  local h = self.h
  self.conn_y.change((h - y) / h)
  self:update()
end

local noBrush = mimas.EmptyBrush
local noPen   = mimas.EmptyPen

function lib:paintControl(p, w, h)
  local cx = self.conn_x
  local cy = self.conn_y
  local x = cx.remote_value * w
  local y = cy.remote_value * h

  if cx.inverted and cy.inverted then
    p:fillRect(x, 0,   w-x, h-y, self.fill_color)
  elseif cx.inverted then
    p:fillRect(x, h-y, w-x, y,   self.fill_color)
  elseif cy.inverted then
    p:fillRect(0, 0,   x,   h-y, self.fill_color)
  else
    p:fillRect(0, h-y, x,   y,   self.fill_color)
  end

  if self.show_thumb then
    -- thumb
    local tx, ty = cx.value * w, (1 - cy.value) * h
    local half_thumb = math.min(math.min(w, h) / const.THUMB_RATIO, const.THUMB_MAX)

    p:setBrush(self.thumb_color)
    p:drawEllipse(tx - half_thumb, ty - half_thumb, 2 * half_thumb, 2 * half_thumb)
    -- show current value
    local txt = string.format('%s / %s', cx:printValue(), cy:printValue())
    p:drawText(
      const.TEXT_PADDING,
      0,
      w - const.TEXT_PADDING,
      h - const.TEXT_PADDING,
      mimas.AlignLeft + mimas.AlignBottom,
      txt
    )
  end
  
  -- border
  p:setPen(self.pen)
  p:setBrush(noBrush)
  p:drawRect(0, 0, w, h)
end
