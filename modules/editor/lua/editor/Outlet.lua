--[[------------------------------------------------------

  editor.Outlet
  -------------

  The editor.Outlet stores basic information on the remote
  outlet and manages links.

--]]------------------------------------------------------

local lib     = {type='editor.Outlet'}
lib.__index   = lib
editor.Outlet = lib

-- PRIVATE
-- Create a single link
local function create_link(self, target_url)
  local process = self.node.process
  local target  = process:get(target_url, 'editor.Inlet')
  if not target then
    local target, err = process:inlet_pending(target_url)
    if not target then
      error(err)
    end
  end
  table.insert(self.links, editor.Link(self, target))
end

-- PUBLIC
setmetatable(lib, {
  -- new method
 __call = function(table, name, definition, node)
  local instance = {
    node  = node,
    name  = name,
    links = {},
  }
  setmetatable(instance, lib)
  if links then
    for _, target_url in ipairs(links) do
      create_link(instance, target_url)
    end
  end
  return instance
end})

function lib:connect(inlet)
  app:post(function()
    local link = editor.LinkView(self.view, inlet.view)
    node.patch.view:addWidget(link)
    table.insert(self.links, link)
  end)
end