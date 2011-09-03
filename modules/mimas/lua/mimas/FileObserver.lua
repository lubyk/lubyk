--[[------------------------------------------------------

  mimas.Label
  -----------

  ...

--]]------------------------------------------------------
local constr        = mimas_core.FileObserver
local mt            = mimas_core.FileObserver_
mimas.FileObserver_ = mt

local addPath = mt.addPath
function mt:addPath(path)
  addPath(self, lk.absolutizePath(path))
end

local removePath = mt.removePath
function mt:removePath(path)
  removePath(self, lk.absolutizePath(path))
end

function mimas.FileObserver(file_or_files)
  local instance = constr()
  if file_or_files then
    if type(file_or_files) == 'table' then
      for file in ipairs(file_or_files) do
        instance:addPath(file)
      end
    else
      instance:addPath(file_or_files)
    end
  end
  return instance
end

