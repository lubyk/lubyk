--[[------------------------------------------------------

  mimas.TableView
  ---------------

  Display tabular data

--]]------------------------------------------------------
require 'worker'
local worker = worker
local constr = mimas_core.TableView
function mimas.TableView(parent)
  local instance
  if parent then
    instance = constr(worker, parent:widget())
  else
    instance = constr(worker)
  end
  return instance
end


