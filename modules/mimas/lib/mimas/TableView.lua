--[[------------------------------------------------------

  mimas.TableView
  ---------------

  Display tabular data

--]]------------------------------------------------------
local constr     = mimas_core.TableView
local mt         = mimas_core.TableView_
mimas.TableView_ = mt

function mimas.TableView(...)
  return mimas.bootstrap('TableView', constr, ...)
end

local close = mt.close
function mt:close()
  -- close is like delete: ensure it only runs in GUI thread
  if not self:deleted() then
    close(self)
  end
end

-- refresh
function mt:reset()
  self.data_source:reset()
end
