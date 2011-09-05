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
  path = lk.absolutizePath(path)
  if not self.paths[path] then
    addPath(self, path)
    self.paths[path] = true
  end
end

local removePath = mt.removePath
function mt:removePath(path)
  path = lk.absolutizePath(path)
  if self.paths[path] then
    removePath(self, lk.absolutizePath(path))
    self.paths[path] = nil
  end
end

function mimas.FileObserver(file_or_files)
  local self = constr()
  self.paths = {}
  if file_or_files then
    if type(file_or_files) == 'table' then
      for file in ipairs(file_or_files) do
        self:addPath(file)
      end
    else
      self:addPath(file_or_files)
    end
  end
  return self
end

