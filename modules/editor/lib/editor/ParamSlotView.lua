--[[------------------------------------------------------

  editor.ParamSlotView
  -------------------

  Displays a control slot during connection edit.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.ParamSlotView = lib
local private = {}

--=============================================== CONSTANTS
local HPEN_WIDTH = 1

function lib:init(name, node, ctrl_link_view)
  self.ctrl_link_view = ctrl_link_view
  self.node  = node
  self.name  = name
  self:setTitle(name)

  self:setHue(node.hue)
  self:setAlignment(mimas.AlignLeft)
end

function lib:connected()
  local list = self.node.controls[self.name]
  return list and true
end

lib.setHue       = editor.CtrlSlotView.setHue
lib.setTitle     = editor.ProcessTab.setTitle
lib.paint        = editor.ProcessTab.paint
lib.setAlignment = editor.ProcessTab.setAlignment

