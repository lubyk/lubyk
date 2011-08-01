--[[------------------------------------------------------

  mimas.TableView
  ---------------

  Display tabular data

--]]------------------------------------------------------
require 'worker'
local worker = worker
local constr = mimas_core.TableView
local mt     = mimas_core.TableView_
local close  = mt.cloase

function mimas.TableView(parent)
  local instance
  if parent then
    instance = constr(worker, parent:widget())
  else
    instance = constr(worker)
  end
  return instance
end

function mt:close()
  -- close is like delete: ensure it only runs in GUI thread
  app:post(function()
    close(self)
  end)
end
