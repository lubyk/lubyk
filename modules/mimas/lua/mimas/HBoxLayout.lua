--[[------------------------------------------------------

  mimas.HBoxLayout
  ----------------

  Horizontal layout of GUI widgets.

--]]------------------------------------------------------
local constr = mimas_core.HBoxLayout
local mt = nil
function mimas.HBoxLayout(parent)
  local instance
  if parent then
    instance = constr(parent:widget())
  else
    instance = constr()
  end
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

