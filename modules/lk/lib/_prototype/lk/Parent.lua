--[[------------------------------------------------------

  lk.Parent
  ---------

  Example of parent node able to create Bidirectional links
  with child nodes.

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

outlet 'register'

list = {}

function childConnected(func)
  return func(CONNECT_TYPE, list)
end

function childDisconnected(func)
  func(CONNECT_TYPE, list)
end

function inlet.bang()
  print(yaml.dump(list))
end
  

