--[[------------------------------------------------------

  _control.lk.Switch
  ------------------

  On/Off switch.

--]]------------------------------------------------------
local lib = lk.SubClass(editor, 'Control')
_control.lk.Switch = lib

local DEFAULT = {
  w = 15,
  h = 15,
}

function lib:init(id, view)
  self:initControl(id, view)
  self:setupConnectors {
    s = 'On/Off',
  }
  self:resize(DEFAULT.h, DEFAULT.h)
end

function lib:resized(w, h)
  self.w = w
  self.h = h
end

function lib:control(x, y, typ)
  if typ == 'click' then
    local cs = self.conn_s
    cs.change(not cs.remote_value)
  end
end

local noBrush = mimas.EmptyBrush
local noPen   = mimas.EmptyPen

function lib:paintControl(p, w, h)
  local cs = self.conn_s
  local on = cs.remote_value

  if on then
    p:fillRect(0, 0, w, h, self.fill_color)
  end

  if self.show_thumb then
    -- thumb
    p:fillRect(0, 0, w, h, self.thumb_color)
  end
  
  -- border
  p:setPen(self.pen)
  p:setBrush(noBrush)
  p:drawRect(0, 0, w, h)
end

