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

function lib:setStem(remote)
  self.stem = remote
  if self.view then
    if remote then
      -- add [+] btn
      self.view:addProcess {name = '+', hue = 0, add_btn = true, machine = self}
      local stem_name = string.match(remote.name, '^@(.*)$')
      self.name = stem_name
      self.view:setName(stem_name)
    else
      -- remove [+] btn
      self.view:removeProcess('+')
    end
  end
end

function lib:createProcess(def)
  local stem = self.stem
  -- show dialog for name and tell machine to create
  -- a new process
  if stem then
    -- FIXME: First write def to morph
    stem.push:send(lubyk.execute_url, 'spawn', def.name)
  end
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

function lib:empty()
  return (not self.stem) and (self.view and #self.view.process_list == 0)
end
