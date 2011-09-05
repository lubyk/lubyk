--[[------------------------------------------------------

  mimas.MenuBar
  ------------

  A MenuBar contains menus. The first menu bar created
  will be used as default for widgets without a specific
  menu bar (MainWindow::setMenuBar).

--]]------------------------------------------------------
local constr  = mimas_core.MenuBar
local mt      = mimas_core.MenuBar_
mimas.MenuBar_ = mt
mimas.MenuBar    = constr

function mimas.MenuBar(win)
  local self = constr()
  if win then
    win:setMenuBar(self)
  end
  return self
end

local addMenu = mt.addMenu
function mt:addMenu(title)
  local menu
  if type(title) == 'table' then
    -- we suppose we have a menu
    menu = title
  else
    menu = mimas.Menu(title)
  end
  addMenu(self, menu)
  self[title] = menu
  return menu
end
