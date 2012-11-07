--[[------------------------------------------------------

  lk.Child
  --------

  Example of a child node able to create a Bidirectional
  link with a parent. To be used with lk.Parent.

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

local parent

-- Shared data with parent
data = data or {}

function inlet.register(elem, conn)
  if parent then
    -- Disconnect
    parent[url()] = nil
  end

  if conn then
    -- Bidirectional connection
    if elem.type == CONNECT_TYPE then
      -- Example of registration.
      parent = elem
      elem[url()] = data
      -- Inform GUI that the connection is bidirectional.
      conn.type = 'Bidirectional'
    else
      error('Invalid connection type. Expecting "%s", found "%s".', CONNECT_TYPE, elem.type)
    end
  end
end

