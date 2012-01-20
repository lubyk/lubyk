--[[------------------------------------------------------

  mimas.Menu
  ------------

  A Menu contains actions (added with addAction) and must
  be inserted in a MenuBar (with addMenu).

--]]------------------------------------------------------
local constr  = mimas_core.Menu
local mt      = mimas_core.Menu_
mimas.Menu_   = mt
mimas.Menu    = constr

local addAction = mt.addAction
function mt:addAction(title, shortcut, func)
  local action
  if type(title) == 'table' then
    -- we suppose that we have an action
    action = title
  else
    -- we create a new action
    action = mimas.Action(title, self)
    if shortcut then
      action:setShortcut(shortcut)
    end
    action.trigger = func
  end
  addAction(self, action)
  self[action:text()] = action
  return action
end
