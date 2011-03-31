--[[------------------------------------------------------

  editor.Link
  -----------

  The Link contains a reference to the source and the
  target. When the target cannot be found, the link points
  to a "floating" inlet (inlet not connected to a Node).

--]]------------------------------------------------------

local lib   = {type='editor.Link'}
lib.__index = lib
editor.Link = lib

setmetatable(lib, {
  --- Create a new editor.Link reflecting the content of a remote
  -- link. If the process view is not shown, the LinkView is not
  -- created.
 __call = function(base, source, target, target_url)
  local instance = {
    source  = source,
    target  = target,
    target_url = target_url,
  }
  -- register in source and target
  table.insert(target.links, instance)
  table.insert(source.links, instance)
  setmetatable(instance, lib)
  return instance
end})

function lib:updateView()
  if not self.view and self.target.view then
    -- Create link view
    self.view = editor.LinkView(self.source.view, self.target.view)
    self.source.node.process.view:addWidget(self.view)
    self.view:lower() -- send to back
  end

  if self.view then
    self.view:slotMoved()
  end
end

local function removeFromList(self, links)
  for i, link in ipairs(links) do
    if link == self then
      table.remove(links, i)
      break
    end
  end
end
-- called from Outlet (source)
function lib:delete()
  removeFromList(self, self.source.links)
  removeFromList(self, self.target.links)

  if self.view then
    self.view.super:__gc()
    self.view = nil
  end
end