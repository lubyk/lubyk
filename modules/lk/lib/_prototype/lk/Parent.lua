--[[------------------------------------------------------

  lk.Parent
  ---------

  Example of parent node able to create Bidirectional links
  with child nodes. To be used with lk.Child.

  Inlets
  --------------------------------------------------------
  [bang]     Print the list of connected children


  Outlets
  --------------------------------------------------------
  [register] Enable child registration by connecting to
             this outlet.

--]]
local CONNECT_TYPE = 'some.Type'

defaults {
}

list = list or {type = CONNECT_TYPE}

--     name,       connect value,    disconnect value
outlet('register', list,             false)

-- Connection debugging
function inlet.bang()
  print(yaml.dump(list))
end
  

