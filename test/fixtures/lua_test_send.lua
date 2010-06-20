inlet('value', RealIO('Any real value.'))
plus = Outlet('plus', RealIO('Input value plus one.'))

function value(r)
  if r then
    plus.send(r+1)
    return r+1
  else
    return 0
  end
end
