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
 __call = function(lib, name, machine_list, remote)
  local self = {
    name         = name,
    machine_list = machine_list,
    zone         = machine_list.zone,
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

function lib:setStem(remote)
  local stem_name = remote.stem_name
  self.stem = remote
  self.name = stem_name
  local machine_by_name = self.machine_list.machines[stem_name]
  if machine_by_name ~= self then
    -- FIXME: merge processes and remove view
  end
  self.machine_list.machines[stem_name] = self
  if self.view then
    self.view:setName(stem_name)
  end
end

--- Ask morph to create a new process.
function lib:createProcess(def)
  local stem = self.stem
  if stem then
    local morph = self.zone.morph
    if morph then
      def.host = stem.stem_name
      morph:createProcess(def)
    end
  end
end

--- Add a new process to the tab view.
function lib:addProcess(service)
  if self.view then
    self.view:addProcess(service)
  end
end

--- Remove a process from the tab view.
function lib:removeProcess(process_name)
  if self.view then
    self.view:removeProcess(process_name)
  end
end

--- Returns true if there is nothing to show and the machine
-- can be removed.
function lib:empty()
  return self.view and #self.view.process_list == 0
end
