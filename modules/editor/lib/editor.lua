--[[------------------------------------------------------

  editor
  ------

  FIXME: description for module 'editor'

--]]------------------------------------------------------
editor = Autoload('editor')

--=================================================== UTILITIES

-- constants
local BOX_PADDING = 1
local HPEN_WIDTH = 1 -- half pen width
local BP = HPEN_WIDTH + BOX_PADDING -- full box padding
local ARC_RADIUS = 8
local TEXT_PADDING = 5
local PAD  = BP + HPEN_WIDTH -- padding for inner shape (top/left)
local PAD2 = BP + 2*HPEN_WIDTH -- do not overlap with background

function editor.paintWithRoundedTitle(p, w, h, title, lw, lh, pen_color, lbl_color, lbl_back, back)
  local lw = lw + 4 * TEXT_PADDING -- 2 paddings on sides
  local lh = lh + 2 * TEXT_PADDING
  local path = mimas.Path()
  p:setPen(HPEN_WIDTH * 2, pen_color)
  p:setBrush(back)
  p:drawRoundedRect(BP, BP, w - 2 * BP, h - 2 * BP, ARC_RADIUS / 2)

  -- draw label background
  p:setPen(mimas.Pen()) -- no pen
  p:setBrush(lbl_back)

  path:moveTo(lw+PAD, PAD)
  path:lineTo(PAD + ARC_RADIUS, PAD)
  path:cubicTo(PAD-1, PAD-1, PAD-1, PAD-1, PAD, PAD + ARC_RADIUS)
  path:lineTo(PAD, lh + PAD)
  path:lineTo(lw - ARC_RADIUS + PAD, lh + PAD)
  path:cubicTo(lw+PAD+1, lh+PAD+1, lw+PAD+1, lh+PAD+1, lw+PAD, lh+PAD-ARC_RADIUS)
  path:lineTo(lw+PAD, PAD)
  p:drawPath(path)

  -- draw label border (only bottom and right)
  p:setBrush(mimas.Brush())
  p:setPen(HPEN_WIDTH * 2, pen_color)

  path = mimas.Path()
  path:moveTo(PAD2, lh + PAD2)
  path:lineTo(lw - ARC_RADIUS + PAD2, lh + PAD2)
  path:cubicTo(lw+PAD2, lh+PAD2, lw+PAD2, lh+PAD2, lw+PAD2, lh+PAD2-ARC_RADIUS)
  path:lineTo(lw+PAD2, PAD2)
  p:drawPath(path)

  -- draw label text
  p:setPen(1, lbl_color)
  p:drawText(PAD+2*TEXT_PADDING, PAD+TEXT_PADDING, lw - 4*TEXT_PADDING, lh - 2*TEXT_PADDING, mimas.AlignRight + mimas.AlignVCenter, title)
end

