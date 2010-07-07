inlet('value', Attribute::real_io('Any real value.'))
plus = Outlet('plus', Attribute::real_io('Input value plus one.'))

function value(r)
  if r then
    plus.send(r+1)
    return r+1
  else
    return 0
  end
end
