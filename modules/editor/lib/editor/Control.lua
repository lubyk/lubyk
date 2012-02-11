--[[------------------------------------------------------

  editor.Control
  --------------

  Base class for all control widgets.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.Control = lib

local private = {}

--=============================================== PUBLIC
function lib:init(name)
  self.name = name
  self.connectors = {}
end


function lib:connector(key)
  return self['conn_'..key]
end
--=============================================== PROTECTED
function lib:setupConnectors(def)
  local conn = self
  for key, info in pairs(def) do
    conn['conn_'..key] = editor.Connector(key, self, info)
  end
end
