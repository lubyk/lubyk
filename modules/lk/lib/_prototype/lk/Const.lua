--[[------------------------------------------------------

  lk.Const
  --------

  Constant value. Can be used to propagate a value to 
  child nodes.

  Inlets
  --------------------------------------------------------
  [value] Can be used to change the value.


  Outlets
  --------------------------------------------------------
  [value] Value is sent to child nodes.

--]]

defaults {
  v = 0,
}

-- The function is used to fetch the current value and
-- send this on child connection.
outlet('value', function()
  return v
end)

function inlet.value(v)
  -- Notify GUI of value changes by using 'param'
  param.v = v
  -- Send value through outlet
  value(v)
end

