--[[------------------------------------------------------

  # Debugging facility

  This is a helper to display information on object creation and
  destruction. Pass any information when creating the object and
  this information will be printed out on destruction.

  The core of this class is written in C++ to use finalizers.

--]]------------------------------------------------------
local lib = lk.Debug_core
lk.Debug  = lib

-- # Class functions

-- Create a new debug object with a name and some more arguments to be
-- printed.
--
--   obj.debug = lk.Debug("an obj", obj.id, obj.type)
--   -- optional callback function
--   function obj.debug:callback()
--     -- do something here
--   end
-- function lib.new(name, ...)

-- Printout inspect information on a table or userdata.
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
    local typ, ptr = lib.userdata(elem)
    print(string.format('<%s: %s>', typ, ptr))
  end
end

-- Return userdata type and pointer value.
-- function lib.userdata(udata)

-- Start logging program execution. The @hook_type@ can be 'l' (log every line),
-- 'r' (log return from function) or 'c' (log function calls = default).
--
-- Usage:
--
--   -- start logging
--   lk.Debug.logExecution('trial')
--   testFunction()
--   -- stop logging
--   lk.Debug.logExecution('trial', false)
function lib.logExecution(prefix, hook_type)
  if hook_type == false then
    -- disable logging
    debug.sethook()
    print(string.format('%16s ============================ END', prefix))
  else
    local hook_type = hook_type or 'c'
    function trace(event, line)
      local s = debug.getinfo(2).short_src
      print(string.format('%16s %s:%i', prefix, s, line))
    end
    debug.sethook(trace, hook_type)
    print(string.format('%16s ============================', prefix))
  end
end
