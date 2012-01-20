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

function lib:init(zone)
  self.controls_visible = Lubyk.editor.show.View
  self.patch_visible    = Lubyk.editor.show.Patch
  self.zone = zone
  self.vbox = mimas.VBoxLayout(self)
  self.vbox:setContentsMargins(0,40,0,0)
  self.vbox:setSpacing(10)
  self.vbox:addStretch()
  self.machines = {}
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
    self.vbox:insertWidget(-2, machine.view)
  end
  return machine
end

function lib:patchViewChanged(visible)
  self.patch_visible = visible
  private.machineLayoutChanged(self)
end

function lib:controlViewChanged(visible)
  self.controls_visible = visible
  private.machineLayoutChanged(self)
end

function lib:updatePosition()
  print('================ updatePosition ===============')
  -- execute later so that minimumSize is updated
  local main = self.zone.main_view
  local w, h = self.vbox:minimumSize()
  self:resize(w, h)
  printf("UPDATE SIZE (%i,%i)", w, h)
  if self.patch_visible and self.controls_visible then
    -- center in the middle
    local gx, gy = main.control_tabs:globalPosition()
    printf("MOVE (%i,%i)", gx-w/2, gy)
    self:globalMove(gx - w/2, gy)
  elseif self.patch_visible then
    -- align right
    local gx, gy = main.patch_view:globalPosition()
    local pw = main.patch_view.w or main.patch_view:width()
    printf("MOVE (%i,%i)", gx + pw - w, gy)
    self:globalMove(gx + pw - w, gy)
  elseif self.controls_visible then
    -- align left
    local gx, gy = main.patch_view:globalPosition()
    printf("MOVE (%i,%i)", gx, gy)
    self:globalMove(gx, gy)
  else
  end

end

function private:machineLayoutChanged()
  for k, machine in pairs(self.machines) do
    -- change alignment in vbox
    machine.view:layoutChanged(self.patch_visible, self.controls_visible)
  end
  self:updatePosition()
end
