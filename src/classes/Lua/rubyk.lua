original_require = original_require or require
rk = rk or {}

-- temporary until 'call_' is registered in 'rk' directly
rk.call = rk_call

function require(lib_name)
  local lib_paths = rk.call("/class/lib")
  local cpath = string.gsub(lib_paths, ":", "/lua/?.so;") .. "/lua/?.so"
  local path  = string.gsub(lib_paths, ":", "/lua/?.lua;") .. "/lua/?.lua"
  package.cpath = cpath
  package.path  = path
  original_require(lib_name)
end


function BangIO(info)
  return {true, info}
end

function RealIO(info)
  return {0.0, info}
end

function StringIO(info)
  return {"", info}
end

function RangeIO(min, max, info)
  return {0.0, min, max, info}
end

function SelectIO(values, info)
  return {"", values, info}
end

-- function HashIO(info)
--   return {{}, info}
-- end
--
function MatrixIO(info)
  return {cv.Mat(), info}
end

function MidiIO(info)
  return {oscit.MidiMessage(), info}
end

-- function AnyIO(info)
--   { ??? , info}
-- end


function Outlet(name, vtype)
  local self = build_outlet_(name, vtype)
  local send

  if (type(self) == 'string') then
    -- error
    print(self)
    send = function (v)
      print('outlet ' .. name .. ' could not be created (' .. self .. '). Cannot send:', v)
    end
  else
    send = function (v)
      send_(self, v)
    end
  end
  return { send = send }
end
