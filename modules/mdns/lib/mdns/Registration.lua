--[[------------------------------------------------------

  mdns.Registration
  -----------------

  This class registers a service name for a give service
  type and calls a callback when the service is announced.
  (this is an implementation of the ZeroConf or Bonjour
  registration).

--]]------------------------------------------------------
require 'mdns.core'
local lib = mdns.Registration_core
mdns.Registration = lib
local private = {}

local function dummy()
  -- noop
end

local ctx = mdns.Context()

--- We should provide a socket to inform when registration is over (callback).
-- This socket could be the default zmq.REQ socket used to by lk.Service ?
local new = lib.new
function lib.new(service_type, name, port, txt, func)
  if not func then
    if type(txt) == 'function' then
      func = txt
      txt = nil
    end
  end
  txt = private.buildTXT(txt)
  local self = new(ctx, service_type, name, port, txt)
  self.callback = func or dummy
  self.txt = txt
  self.thread = lk.Thread(function()
    while true do
      sched:waitRead(self:fd())
      self:callback(self:getService())
    end
  end)
  ctx:addRegistration(self)
  return self
end

function lib:kill()
  if self.thread then
    self.thread:kill()
    self.thread = nil
  end
end


--=============================================== PRIVATE
local INVALID_KEY_CHARS = '[^a-zA-Z]'

--- Build a TXT record from a Lua table.
function private.buildTXT(dict)
  if not dict then
    return ''
  elseif type(dict) ~= 'table' then
    error('txt field is not a table')
  end

  local txt = ''
  for key, value in pairs(dict) do
    if type(value) == 'number' then
      value = string.format('%i', value)
    end
    if type(key) == 'string' and type(value) == 'string' then
      if string.match(key, INVALID_KEY_CHARS) then
        error(string.format("Invalid key '%s' in TXT record (contains invalid characters).", key))
      end
      if string.len(key) + string.len(value) > 254 then
        error(string.format("Invalid value '%s' in TXT record (value + key is too long).", value))
      end
      txt = txt .. string.char(string.len(key) + 1 + string.len(value)) .. key
                .. '=' .. value
    end
  end
  if string.len(txt) > 400 then
    error(string.format("TXT record size is too big (%i bytes)", string.len(txt)))
  end
  return txt
end

