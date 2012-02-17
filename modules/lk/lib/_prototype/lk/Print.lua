--[[------------------------------------------------------

  Print
  -----

  Print incomming values.

--]]------------------------------------------------------

function inlet.print(v)
  if type(v) == 'table' then
    print(yaml.dump(v))
  else
    print(v)
  end
end
