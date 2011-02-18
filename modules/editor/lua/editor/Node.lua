--[[------------------------------------------------------

  editor.Node
  -----------

  The Node contains all the information to communicate with
  the remote and also contains a NodeView and slots.

--]]------------------------------------------------------

local lib   = {}
lib.__index = lib
editor.Node = lib

setmetatable(lib, {
  -- new method
 __call = function(table, info)
  local instance = info
  setmetatable(instance, lib)
  instance.inlets = instance.inlets or {}
  instance.outlet = instance.outlet or {}
  instance:setHue(info.hue or 0.2)
  return instance
end})

function lib:setHue(hue)
  self.hue      = hue
  self.color    = mimas.Color(self.hue, 0.3, 0.8, 0.8)
  self.bg_color = mimas.Color(self.hue, 0.2, 0.2)
end
