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
  local self = {
    source  = source,
    target  = target,
    target_url = target_url,
  }
  -- register in source and target
  table.insert(target.links, self)
  table.insert(source.links, self)
  source.links_by_target[target_url] = self
  setmetatable(self, lib)
  return self
end})

function lib:updateView()
  if not self.view and self.target.view and self.source.view then
    -- Create link view
    self.view = editor.LinkView(self.source.view, self.target.view)
    if self:isCrossProcess() then
      self.source.node.process.delegate.main_view:addWidget(self.view)
    else
      self.source.node.process.view:addWidget(self.view)
    end
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
  local link = self.source.links_by_target[self.target_url]
  if link == self then
    self.source.links_by_target[self.target_url] = nil
  end
  removeFromList(self, self.target.links)
  self:deleteView()
end

function lib:deleteView()
  if self.view then
    local delegate = self.source.node.delegate
    if delegate.selected_link_view == self.view then
      delegate.selected_link_view = nil
    end
    self.view.super:__gc()
    self.view = nil
  end
end

function lib:isCrossProcess()
  return self.source.node.process ~= self.target.node.process
end