--[[------------------------------------------------------

  mimas.GLWindow
  --------------

  A window to draw OpenGL content.

--]]------------------------------------------------------
local constr = mimas_core.GLWindow
local mt = nil
local worker = worker
function mimas.GLWindow()
  local instance = constr(worker)
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

