--[[------------------------------------------------------

  mimas.VBoxLayout
  ----------------

  Vertical layout of GUI widgets.

--]]------------------------------------------------------
require 'mimas.VBoxLayout_core'
local constr = mimas.VBoxLayout
local mt = nil
function mimas.VBoxLayout(parent)
  local instance = constr(parent:widget())
  if not mt then
    -- we have to wait for the first instance in order to
    -- access the metatable and rewrite the methods.
    mt = getmetatable(instance)
    local addWidget = mt.addWidget
    function mt.addWidget(self, other)
      addWidget(self, other:widget())
    end

    local addLayout = mt.addLayout
    function mt.addLayout(self, other)
      addLayout(self, other:layout())
    end
  end
  return instance
end

