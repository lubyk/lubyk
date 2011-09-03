--[[------------------------------------------------------

  mimas.TableView
  ---------------

  Display tabular data

--]]------------------------------------------------------
local constr     = mimas_core.TableView
local mt         = mimas_core.TableView_
mimas.TableView_ = mt
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
  if not self:deleted() then
    app:post(function()
      close(self)
    end)
  end
end

-- refresh
function mt:reset()
  self.data_source:reset()
end
