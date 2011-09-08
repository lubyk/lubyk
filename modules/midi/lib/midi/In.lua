--[[------------------------------------------------------

  midi.In
  -------

  Create/open a midi port.

--]]------------------------------------------------------
require 'midi.In_core'

local constr = midi.In
function midi.In(port_or_name, func)
  local instance = constr()
  if type(port_or_name) == 'string' then
    -- open a virtual port
    instance:virtual_port(port_or_name)
  else
    instance:open_port(port_or_name)
  end
  if func then
    instance.receive = func
  end
  return instance
end
