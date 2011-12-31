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

function lib:init()
  self.vbox  = mimas.VBoxLayout(self)
  self.vbox:setContentsMargins(0,0,0,0)
  self.vbox:setSpacing(10)
  self.vbox:addStretch()
  self.machine_by_ip = {}
  self:addMachine('local', {ip = 'localhost'})
end

function lib:addMachine(name, remote)
  -- Do we have a machine for this ip ?
  local machine = self.machine_by_ip[remote.ip]
  if not machine then
    machine = editor.Machine(name)
    self.machine_by_ip[remote.ip] = machine
  elseif machine.name ~= name then
    machine:setName(name)
  end
  machine:connect(remote)
  if not machine.view then
    machine.view = editor.MachineView(machine)
    self.vbox:insertWidget(-2, machine.view, 0, mimas.AlignRight)
  end
end

function lib:addProcess(process)
  -- TODO: find machine by ip
  machine = self.machine_by_ip['localhost']
  machine:addProcess(process)
end

function lib:removeProcess(process_name)
  -- TODO: find machine by ip
  machine = self.machine_by_ip['localhost']
  machine:removeProcess(process_name)
end

-- function lib:paint(p, w, h)
--   p:fillRect(0, 0, w, h, mimas.colors.Green:colorWithAlpha(0.2))
-- end
