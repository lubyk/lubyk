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
  self.machine_by_ip = {}
end

function lib:addProcess(service)
  local machine = self:getMachine(service.ip)
  machine:addProcess(process)
end

function lib:removeProcess(service)
  local machine = self.machine_by_ip[service.ip]
  if machine then
    machine:removeProcess(service.name)
    if machine:empty() then
      -- empty machine, no stem cell
      self.machine_by_ip[service.ip] = nil
      if machine.view then
        machine.view:__gc()
      end
    end
  end
end

function lib:getMachine(ip)
  local machine = self.machine_by_ip[ip]
  if not machine then
    machine = editor.Machine(ip, self.zone)
    self.machine_by_ip[ip] = machine

    machine.view = editor.MachineView(machine)
    self.vbox:insertWidget(-2, machine.view)
  end
  return machine
end
