--[[------------------------------------------------------

  lk.Child
  --------

  Example of a child node able to create a Bidirectional
  link with a parent.

  Inlets
  --------------------------------------------------------
  [register] Connection through this inlet will register
             child in parent.


  Outlets
  --------------------------------------------------------
  none.

--]]
local CONNECT_TYPE = 'some.Type'

defaults {
}

function inlet.register()
  -- only used for registration
end

function connected(type, list)
  if type == CONNECT_TYPE then
    table.insert(list, url())
    return true
  end
end

function disconnected(type, list)
  local url = url()
  if type ~= CONNECT_TYPE then
    return
  end

  for i, v in ipairs(list) do
    if v == url then
      table.remove(list, i)
      break
    end
  end
end
