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
 __call = function(base, source, target)
  local instance = {
    source  = source,
    target  = target,
  }
  table.insert(target.links, instance)
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
