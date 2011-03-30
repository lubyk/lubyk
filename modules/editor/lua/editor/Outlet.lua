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
local function createLink(self, target_url)
  if self.links[target_url] then
    return
  end
  local process = self.node.process
  local target, err  = process:get(target_url, editor.Inlet)
  if target == false then
    error(err)
  elseif not target then
    target, err = process:pendingInlet(target_url)
    if not target then
      error(err)
    end
  end
  local link = editor.Link(self, target)
  self.links[target_url] = link
  if self.view then
    link:updateView()
  end
end

-- PUBLIC
setmetatable(lib, {
  -- new method
 __call = function(table, node, name, def)
  local instance = {
    node  = node,
    name  = name,
    links = {},
  }
  setmetatable(instance, lib)

  instance:set(def)

  if node.view then
    instance:updateView()
  end
  return instance
end})

function lib:set(def)
  if def.links then
    for _, target_url in ipairs(def.links) do
      createLink(self, target_url)
    end
  end
end

-- Create or update view.
function lib:updateView()
  if not self.view then
    self.view = editor.SlotView(self)
    self.node.view:addWidget(self.view)
  else
    self:updateLinkViews()
  end
end

-- Called when slot moved
function lib:updateLinkViews()
  for _,link in pairs(self.links) do
    link:updateView()
  end
end


function lib:connect(inlet)
  app:post(function()
    local link = editor.LinkView(self.view, inlet.view)
    node.patch.view:addWidget(link)
    table.insert(self.links, link)
  end)
end
