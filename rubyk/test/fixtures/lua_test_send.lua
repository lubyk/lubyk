inlet('value', RealIO('real', 'Any real value.'))
plus = Outlet('plus', RealIO('real', 'Input value plus one.'))

function value(r)
  plus.send(r+1)
end
