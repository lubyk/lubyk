--[[------------------------------------------------------

  editor.MachineView
  ------------------

  Displays the list of ProcessTabs.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.MachineView = lib

-- constants
local WIDTH       = 120
local PADDING     = 10
-- half pen width
local HPEN_W      = 1

function lib:init(machine)
  self.machine = machine
  self:setSizePolicy(mimas.Fixed, mimas.Minimum)
  self.vbox  = mimas.VBoxLayout(self)
  self.vbox:setContentsMargins(0,0,0,0)
  self.title = mimas.Label()
  self.title:setStyle 'background:hsva(0,0,64); color:#fff; padding:5px'
  self.title:setAlignment(mimas.AlignCenter)
  self.vbox:addWidget(self.title)
  self.process_list = editor.ProcessList(self)
  self.vbox:addWidget(self.process_list)
  self.vbox:addStretch()
  self:setName(machine.name)
end

function lib:createProcess(name)
  -- show dialog for name and tell machine to create
  -- a new process
end

function lib:setName(name)
  self.name = name
  self.title:setText(name)
end

function lib:addProcess(process)
  self.process_list:addProcess(process)
  self:resizeToFit()
end

function lib:resizeAll()
  self.process_list:resizeToFit()
  self:resizeToFit()
end

function lib:removeProcess(process_name)
  self.process_list:removeProcess(process_name)
  self:resizeToFit()
end

function lib:resizeToFit()
  local width  = self.process_list.width  + 2 * HPEN_W 
  local height = self.process_list.height + 30
  self:setSizeHint(width, height)
  self.width  = width
  self.height = height
  self:resize(width, height)
end

function lib:resized(w, h)
  if w > self.width then
    -- place process_list
    self.process_list:move(w - self.process_list.width - 2 * HPEN_W, self.process_list:y())
  end
  self.width = w
end

function lib:paint(p, w, h)
  p:setPen(2*HPEN_W, mimas.Color(0,0,64/256))
  --p:fillRect(0, 0, w, h, mimas.colors.Green:colorWithAlpha(0.2))
  p:drawRect(HPEN_W, HPEN_W, w - 2*HPEN_W, h-2*HPEN_W)
end
