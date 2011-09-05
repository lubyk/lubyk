--[[------------------------------------------------------

  midi.In
  -------

  Create/open a midi port.

--]]------------------------------------------------------
require 'midi.In_core'
require 'worker'

local constr = midi.In
local worker = worker
function midi.In(port_or_name, func)
  local instance = constr(worker)
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
