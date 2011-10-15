--[[------------------------------------------------------

  midi.In
  -------

  Create/open a midi port.

--]]------------------------------------------------------
require 'midi.In_core'

local constr = midi.In
local mt     = midi.In_

function midi.In(port_or_name, func)
  local self = constr()
  if type(port_or_name) == 'string' then
    -- open a virtual port
    self:virtualPort(port_or_name)
  else
    self:openPort(port_or_name)
  end
  self.receive = func or function() end
  self.thread = lk.Thread(function()
    self:loop()
  end)
  return self
end

function mt:loop()
  local thread  = self.thread
  local super   = self.super
  local read_fd = self:fd()
  while thread.should_run do
    sched:waitRead(read_fd)
    self.receive(super:pop())
  end
end
