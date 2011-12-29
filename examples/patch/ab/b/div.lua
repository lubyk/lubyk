inlet('input', 'Information on input [type].')
outlet('output', 'Information on output [type].')

t = t or 0
function inlet.input(val)
  -- print and pass through
  if val % 300 == 0 then
    t = t + 1
    output(t)
  end
end
print("RELOADED")
