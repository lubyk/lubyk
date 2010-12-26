--[[------------------------------------------------------

  mimas.HBoxLayout
  ----------------

  Horizontal layout of GUI widgets.

--]]------------------------------------------------------
require 'mimas.HBoxLayout_core'
local constr = mimas.HBoxLayout
local mt = nil
function mimas.HBoxLayout(widget)
  local instance = constr(widget)
  if not mt then
    -- we have to wait for the first instance in order to
    -- access the metatable and rewrite the methods.
    mt = getmetatable(instance)
    local addWidget = mt.addWidget
    function mt.addWidget(self, other)
      addWidget(self, other:widget())
    end
  end
  return instance
end

