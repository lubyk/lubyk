--[[------------------------------------------------------

  lk.Debug
  --------

  This is a helper to display information on object
  creation and destruction. Pass any information when
  creating the object and these information will be
  printed out on destruction.

  Usage: lk.Debug('some name', ...)

--]]------------------------------------------------------
require 'lk.Debug_core'
local constr = lk.Debug
local lib   = {}
lib.__index = lib
lk.Debug    = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, ...)
  return constr(...)
end})

function lib.inspect(elem)
  if type(elem) == 'table' then
    for k,v in pairs(elem) do
      print(k, v)
      if type(v) == 'table' then
        for k2, v2 in pairs(v) do
          print('', k2, v2)
        end
      end
    end
  elseif type(elem) == 'userdata' then
    local typ, ptr = lk.Debug_userdata(elem)
    print(string.format('<%s: %s>', typ, ptr))
  end
end

--- Class method to start logging execution
function lib.logExecution(prefix, hook_type)
  if hook_type == false then
    -- disable logging
    debug.sethook()
    print(string.format('%16s ============================ END', prefix))
  else
    local hook_type = hook_type or 'l'
    function trace(event, line)
      local s = debug.getinfo(2).short_src
      print(string.format('%16s %s:%i', prefix, s, line))
    end
    debug.sethook(trace, hook_type)
    print(string.format('%16s ============================', prefix))
  end
end
