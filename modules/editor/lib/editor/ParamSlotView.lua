--[[------------------------------------------------------

  editor.ParamSlotView
  -------------------

  Displays a control slot during connection edit.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.ParamSlotView = lib
lib.stype = 'param'
local private = {}

--=============================================== CONSTANTS
local HPEN_WIDTH = 1

function lib:init(name, node, link_editor)
  self.link_editor = link_editor
  self.node  = node
  self.name  = name
  self:setTitle(name)

  self:setHue(node.hue)
  self:setAlignment(mimas.AlignLeft)
end

function lib:connected()
  return self.node.controls[self.name] and true
end

lib.setTitle     = editor.ProcessTab.setTitle
lib.paint        = editor.ProcessTab.paint
lib.setAlignment = editor.ProcessTab.setAlignment

lib.setHue       = editor.CtrlSlotView.setHue
lib.select       = editor.CtrlSlotView.select
lib.resized      = editor.CtrlSlotView.resized
lib.click        = editor.CtrlSlotView.click
lib.mouse        = editor.CtrlSlotView.mouse
