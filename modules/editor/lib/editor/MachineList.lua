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
  self.vbox:setContentsMargins(0,40,0,0)
  self.vbox:setSpacing(PADDING)
  self.machines = {}
  self.machine_view_list = {}
  self:resize(10,10)
end

function lib:addProcess(service)
  local machine = self:getMachine(service.ip)
  machine:addProcess(process)
  self:updatePosition()
end

function lib:removeProcess(service)
  local machine = self.machines[service.ip]
  if machine then
    machine:removeProcess(service.name)
    if machine:empty() then
      -- empty machine, no stem cell
      self.machines[service.ip] = nil
      for i, m in ipairs(self.machine_view_list) do
        if m == machine.view then
          table.remove(self.machine_view_list, i)
        end
      end
      if machine.view then
        machine.view:__gc()
      end
    end
    self:updatePosition()
  end
end

function lib:getMachine(name_or_ip)
  local machine = self.machines[name_or_ip]
  if not machine then
    -- Create machine
    machine = editor.Machine(name_or_ip, self)
    self.machines[name_or_ip] = machine
    machine.view = editor.MachineView(machine)
    -- This is needed so that when a process is added, the list size
    -- is updated.
    machine.view.machine_list = self
    self.vbox:insertWidget(-1, machine.view)
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
  self:resize(w, h)
  if self.patch_visible and self.controls_visible then
    -- center in the middle
    local gx, gy = main.control_tabs:globalPosition()
    self:globalMove(gx - w/2, gy)
  elseif self.patch_visible then
    -- align right
    local gx, gy = main.patch_view:globalPosition()
    local pw = main.patch_view.w or main.patch_view:width()
    self:globalMove(gx + pw - w, gy)
  elseif self.controls_visible then
    -- align left
    local gx, gy = main.patch_view:globalPosition()
    self:globalMove(gx, gy)
  else
    -- none visible ?
  end
  private.machineLayoutChanged(self)
end

function private:machineLayoutChanged()
  for k, machine in pairs(self.machines) do
    -- change alignment in vbox
    machine.view:layoutChanged(self.patch_visible, self.controls_visible)
  end
end


