--[[------------------------------------------------------

  editor.MachineList
  ------------------

  Displays the list of Machines

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.MachineList = lib
local private = {}

-- constants
local WIDTH       = 120
local PADDING     = 10

function lib:init(zone_view)
  self.controls_visible = editor.Settings.show.View
  self.patch_visible    = editor.Settings.show.Patch
  self.zone_view = zone_view
  self.zone = zone_view.zone
  self.vbox = mimas.VBoxLayout(self)
  self.vbox:setContentsMargins(0,0,0,20)
  self.vbox:setSpacing(PADDING)
  self.machines = {}
  self.machine_view_list = {}
  self:resize(10,10)
end

function lib:addProcess(ip, process)
  local machine = self:getMachine(process.host, ip)
  if machine == process.machine then
    -- nothing to do
    return
  elseif process.machine and process.machine ~= machine then
    self:removeProcess(process)
  end
  process.machine = machine
  machine:addProcess(process)
  self:updatePosition()
end

function lib:removeProcess(service)
  local machine = service.machine or self.machines[service.ip]
  if machine then
    machine:removeProcess(service.name)
    if machine:empty() then
      -- empty machine, no stem cell
      self.machines[machine.name] = nil
      if machine.ip then
        self.machines[machine.ip] = nil
      end
      private.removeMachine(self, machine)
    end
    self:updatePosition()
  end
end

function lib:getMachine(name, ip)
  -- Try to merge name/ip if we have both or find
  -- any of the two.
  local machine_by_name = name and self.machines[name]
  local machine_by_ip   = ip   and self.machines[ip]

  if name and ip then
    if machine_by_name and machine_by_ip and machine_by_name ~= machine_by_ip then
      -- Reference machine_by_name as machine by IP.
      self.machines[ip] = machine_by_name

      -- transfert processes from machine_by_ip to machine_by_name
      local processes = machine_by_ip:processes()
      machine_by_ip:clear()
      private.removeMachine(self, machine_by_ip)
      for _, process in ipairs(processes) do
        self:addProcess(ip, process)
      end
    elseif machine_by_ip then
      machine_by_ip:setName(name)
      self.machines[name] = machine_by_ip
    elseif machine_by_name then
      machine_by_name.ip = ip
      self.machines[ip] = machine_by_name
    end
  end

  local machine = machine_by_name or machine_by_ip
  if not machine then
    local ref = name or ip or '-'
    -- Create machine
    machine = editor.Machine(ref, self)
    self.machines[ref] = machine
    machine.ip = ip
    machine.view = editor.MachineView(machine)
    -- This is needed so that when a process is added, the list size
    -- is updated.
    machine.view.machine_list = self
    self.vbox:insertWidget(-1, machine.view)
    machine.view:show()
    table.insert(self.machine_view_list, machine.view)
  end
  return machine
end

function lib:patchViewChanged(visible)
  self.patch_visible = visible
  self:updatePosition()
end

function lib:controlViewChanged(visible)
  self.controls_visible = visible
  self:updatePosition()
end

function lib:updatePosition()
  local main = self.zone_view
  local w, h = self.vbox:minimumSize()
  local _, gy = main:globalPosition()
  local gy = gy + main:height() - h
  self:resize(w, h)
  if self.patch_visible and self.controls_visible then
    -- center in the middle
    local gx, _ = main.control_tabs:globalPosition()
    self:globalMove(gx - w/2, gy)
  elseif self.patch_visible then
    -- align right
    local gx, _ = main.patch_view:globalPosition()
    local pw = main.patch_view.w or main.patch_view:width()
    self:globalMove(gx + pw - w, gy)
  elseif self.controls_visible then
    -- align left
    local gx, _ = main.patch_view:globalPosition()
    self:globalMove(gx, gy)
  else
    -- none visible ?
  end
  private.machineLayoutChanged(self)
end

function private:machineLayoutChanged()
  for _, view in ipairs(self.machine_view_list) do
    -- change alignment in vbox
    view:layoutChanged(self.patch_visible, self.controls_visible)
  end
end

function private:removeMachine(machine)
  for i, m in ipairs(self.machine_view_list) do
    if m == machine.view then
      table.remove(self.machine_view_list, i)
    end
  end
  if machine.view then
    machine.view:__gc()
    machine.view = nil
  end
end
