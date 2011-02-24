--[[------------------------------------------------------

  midi.Out
  --------

  Create/open a midi port.

--]]------------------------------------------------------
require 'midi.Out_core'
require 'worker'

local constr = midi.Out
local worker = worker
function midi.Out(port_or_name)
  local instance = constr(worker)
  if type(port_or_name) == 'string' then
    -- open a virtual port
    instance:virtual_port(port_or_name)
  else
    instance:open_port(port_or_name)
  end
  return instance
end
