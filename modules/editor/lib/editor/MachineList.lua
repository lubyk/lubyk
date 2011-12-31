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
    machine.view:resizeToFit()
    self.vbox:insertWidget(-1, machine.view, 0, mimas.AlignRight)
  end
  self:resizeToFit()
end

function lib:addProcess(process)
  -- TODO: find machine by ip
  machine = self.machine_by_ip['localhost']
  machine:addProcess(process)
  self:resizeToFit()
end

function lib:removeProcess(process_name)
  -- TODO: find machine by ip
  machine = self.machine_by_ip['localhost']
  machine:removeProcess(process_name)
  self:resizeToFit()
end

function lib:resizeToFit()
  local y = PADDING
  local width = 0
  -- find biggest
  for _, elem in pairs(self.machine_by_ip) do
    if elem.view then
      elem = elem.view
      if elem.width > width then
        width = elem.width
      end
      y = y + PADDING + elem.height
    end
  end

  self:setSizeHint(width, y - PADDING)
  self.width = width
  self:resize(width, y - PADDING)
end

function lib:resized(w, h)
  self.width  = w
end

-- function lib:paint(p, w, h)
--   p:fillRect(0, 0, w, h, mimas.colors.Green:colorWithAlpha(0.2))
-- end
