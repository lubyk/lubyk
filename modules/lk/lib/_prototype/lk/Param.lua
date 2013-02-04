--[[------------------------------------------------------

  h1. lk.Param

  Transforms a pipe into a parameter.

--]]------------------------------------------------------
local i, o, p, print = lubyk()

p {
  -- Declare a in/out parameter
  s = 0,
}

-- Declare an output
o.output = {
  -- On connection, send current param value.
  connect = function() return s end,
}

-- On parameter change, send new value out.
function p.s(v)
  o.output(v)
end

-- On input change, set param and notify value.
function i.input(v)
  -- Set param and notify
  p.s = v

  -- Pass value to output
  o.output(v)
end


