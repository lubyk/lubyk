--[[------------------------------------------------------

  editor.main
  -----------

  Singleton to hold editor state information (such as
  current selection, etc).

--]]------------------------------------------------------

local lib    = {type='editor.Main'}
lib.__index  = lib
editor.Main  = lib

-- PUBLIC
setmetatable(lib, {
  -- new method
 __call = function(table)
  local instance = {
    selected_node_views = {},
  }
  setmetatable(instance, lib)

  return instance
end})

editor.main = editor.Main() -- singleton

function lib:selectNodeView(node_view, add_to_selection)
  if not add_to_selection then
    for _, view in pairs(self.selected_node_views) do
      view.selected = false
      view:update()
    end
    self.selected_node_views = {}
  end
  if node_view then
    self.selected_node_views[node_view] = node_view
    node_view.selected = true
  end
end

function lib:deselectNodeView(node_view)
  self.selected_node_views[node_view] = nil
end
