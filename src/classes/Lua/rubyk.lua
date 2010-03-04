function NilIO(info)
  return {nil, info; n=2}
end

function BangIO(info)
  return {nil, info; n=2}
end

function RealIO(units, info)
  return {0.0, units, info}
end

function StringIO(units, info)
  return {"", units, info}
end

function RangeIO(min, max, units, info)
  return {0.0, min, max, units, info}
end

function SelectIO(values, units, info)
  return {"", values, units, info}
end

-- function HashIO(info)
--   return {{}, info}
-- end
--
function MatrixIO(info)
  return {cv.Mat(), info}
end

-- function MidiIO(info)
--   {MidiMessage(), info}
-- end

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
