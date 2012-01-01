--[[------------------------------------------------------

  editor.Machine
  --------------

  The editor.Machine manages a link with a remote or local
  machine (found with Zeroconf with a name like
  'default:@venus'). The machine is used to create/remove
  processes.

--]]------------------------------------------------------

local lib      = {type='editor.Machine'}
lib.__index    = lib
editor.Machine = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, name, zone, remote)
  local self = {
    name = name,
    zone = zone,
  }

  setmetatable(self, lib)
  if remote then
    self:connect(remote)
  end
  return self
end})

function lib:connect(remote)
  self.remote = remote
end

function lib:createProcess(def)
  -- show dialog for name and tell machine to create
  -- a new process
end

function lib:addProcess(process)
  if self.view then
    self.view:addProcess(process)
  end
end

function lib:removeProcess(process_name)
  if self.view then
    self.view:removeProcess(process_name)
  end
end
