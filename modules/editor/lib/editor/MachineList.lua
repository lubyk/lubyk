--[[------------------------------------------------------

  editor.MachineList
  ------------------

  Displays the list of Machines

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.MachineList = lib

-- constants
local WIDTH       = 120
local PADDING     = 10

function lib:init(zone)
  self.zone = zone
  self.vbox = mimas.VBoxLayout(self)
  self.vbox:setContentsMargins(0,0,0,0)
  self.vbox:setSpacing(10)
  self.vbox:addStretch()
  self.machines = {}
end

function lib:addProcess(service)
  local machine = self:getMachine(service.ip)
  machine:addProcess(process)
end

function lib:removeProcess(service)
  local machine = self.machines[service.ip]
  if machine then
    machine:removeProcess(service.name)
    if machine:empty() then
      -- empty machine, no stem cell
      self.machines[service.ip] = nil
      if machine.view then
        machine.view:__gc()
      end
    end
  end
end

function lib:getMachine(name_or_ip)
  local machine = self.machines[name_or_ip]
  if not machine then
    -- Create machine
    machine = editor.Machine(name_or_ip, self)
    self.machines[name_or_ip] = machine
    machine.view = editor.MachineView(machine)
    self.vbox:insertWidget(-2, machine.view)
  end
  return machine
end
