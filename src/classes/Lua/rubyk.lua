original_require = original_require or require
rk = rk or {}

-- temporary until 'call_' is registered in 'rk' directly
rk.call = rk_call

local info_key = '@info'
local type_key = '@type'
local sign_key = 'signature'
local name_key = 'name'
local min_key  = 'min'
local max_key  = 'max'
local values_key= 'values'

function require(lib_name)
  local lib_paths = rk.call("/class/lib")
  local cpath = string.gsub(lib_paths, ":", "/lua/?.so;") .. "/lua/?.so"
  local path  = string.gsub(lib_paths, ":", "/lua/?.lua;") .. "/lua/?.lua"
  package.cpath = cpath
  package.path  = path
  original_require(lib_name)
end

function AttrIO(info, name, signature)
  return {[info_key] = info, [type_key] = {[name_key] = name, [sign_key] = signature}}
end

function BangIO(info)
  return AttrIO(info, 'bang', 'T')
end

function RealIO(info)
  return AttrIO(info, 'real', 'f')
end

function StringIO(info)
  return AttrIO(info, 'string', 's')
end

function RangeIO(info, min, max)
  return {[info_key] = info, [type_key] = {[name_key] = 'real', [sign_key] = 'f', [min_key] = min, [max_key] = max}}
end

function SelectIO(info, values)
  return {[info_key] = info, [type_key] = {[name_key] = 'select', [sign_key] = 's', [values_key] = values}}
end

-- function Attribute::hash_io(info)
--   return {{}, info}
-- end
--
function MatrixIO(info)
  return AttrIO(info, 'matrix', 'M')
end

function MidiIO(info)
  return AttrIO(info, 'midi', 'm')
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
