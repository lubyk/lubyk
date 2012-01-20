--[[------------------------------------------------------

  editor
  ------

  FIXME: description for module 'editor'

--]]------------------------------------------------------
editor = Autoload('editor')

math.randomseed(os.time())
math.random()
--=================================================== UTILITIES

-- constants
local BOX_PADDING = 1
local HPEN_WIDTH = 1 -- half pen width
local BP = HPEN_WIDTH + BOX_PADDING -- full box padding
local ARC_RADIUS = 8
local TEXT_PADDING = 5
local PAD  = BP + HPEN_WIDTH -- padding for inner shape (top/left)
local PAD2 = BP + 2*HPEN_WIDTH -- do not overlap with background

function editor.paintWithRoundedTitle(p, w, h, title, lw, lh, pen_color, lbl_color, lbl_back, back, align, pad_left)
  align = align or mimas.AlignLeft
  pad_left = pad_left or 0
  local lw = lw + 4 * TEXT_PADDING -- 2 paddings on sides
  local lh = lh + 2 * TEXT_PADDING
  local path = mimas.Path()
  -- label border (only bottom and right)
  local label_path = mimas.Path()
  p:setPen(HPEN_WIDTH * 2, pen_color)
  p:setBrush(back)
  p:drawRoundedRect(BP + pad_left, BP, w - pad_left - 2 * BP, h - 2 * BP, ARC_RADIUS / 2)

  -- draw label background
  p:setPen(mimas.Pen()) -- no pen
  p:setBrush(lbl_back)

  if align == mimas.AlignRight then
    -- top-left
    path:moveTo(w - lw - PAD, PAD)
    -- top-right
    path:lineTo(w - PAD - ARC_RADIUS, PAD)
    path:cubicTo(w-PAD+1, PAD-1, w-PAD+1, PAD-1, w-PAD, PAD + ARC_RADIUS)
    path:lineTo(w - PAD, lh + PAD)
    -- bottom-right
    path:lineTo(w - lw + ARC_RADIUS - PAD, lh + PAD)
    -- bottom-left
    path:cubicTo(w-lw-PAD-1, lh+PAD+1, w-lw-PAD-1, lh+PAD+1, w-lw-PAD, lh+PAD-ARC_RADIUS)
    -- top-left
    path:lineTo(w-lw-PAD, PAD)

    -- bottom-left
    label_path:moveTo(w-PAD2, lh + PAD2)
    -- bottom-right
    label_path:lineTo(w - lw + ARC_RADIUS - PAD2, lh + PAD2)
    label_path:cubicTo(w-lw-PAD2, lh+PAD2, w-lw-PAD2, lh+PAD2, w-lw-PAD2, lh+PAD2-ARC_RADIUS)
    -- top-right
    label_path:lineTo(w-lw-PAD2, PAD2)
  else
    -- top-right
    path:moveTo(lw + PAD, PAD)
    -- top-left
    path:lineTo(PAD + ARC_RADIUS, PAD)
    path:cubicTo(PAD-1, PAD-1, PAD-1, PAD-1, PAD, PAD + ARC_RADIUS)
    path:lineTo(PAD, lh + PAD)
    -- bottom-left
    path:lineTo(lw - ARC_RADIUS + PAD, lh + PAD)
    -- bottom-right
    path:cubicTo(lw+PAD+1, lh+PAD+1, lw+PAD+1, lh+PAD+1, lw+PAD, lh+PAD-ARC_RADIUS)
    -- top-right
    path:lineTo(lw+PAD, PAD)

    -- bottom-left
    label_path:moveTo(PAD2, lh + PAD2)
    -- bottom-right
    label_path:lineTo(lw - ARC_RADIUS + PAD2, lh + PAD2)
    label_path:cubicTo(lw+PAD2, lh+PAD2, lw+PAD2, lh+PAD2, lw+PAD2, lh+PAD2-ARC_RADIUS)
    -- top-right
    label_path:lineTo(lw+PAD2, PAD2)
  end
  p:drawPath(path)

  -- draw label border (only bottom and right)
  p:setBrush(mimas.Brush())
  p:setPen(HPEN_WIDTH * 2, pen_color)
  p:drawPath(label_path)

  -- draw label text
  p:setPen(1, lbl_color)
  p:drawText(PAD+2*TEXT_PADDING, PAD+TEXT_PADDING, w - 4*TEXT_PADDING - 2*PAD, lh - 2*TEXT_PADDING, align + mimas.AlignVCenter, title)
end

