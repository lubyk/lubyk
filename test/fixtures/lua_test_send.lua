inlet('value', RealIO('Any real value.'))
plus = Outlet('plus', RealIO('Input value plus one.'))

function value(r)
  plus.send(r+1)
end
