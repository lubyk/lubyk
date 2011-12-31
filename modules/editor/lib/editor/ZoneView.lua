--[[------------------------------------------------------

  editor.ZoneView
  ---------------

  Main view holding the different parts of the editor
  (LibraryView, HelpView, PatchView and ControlView).

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'MainWindow')
editor.ZoneView = lib
local private = {}

-- constants
local WIDTH   = 600
local HEIGHT  = 400
local PADDING = 2

function lib:init(zone)
  -- The layout holder is just there so that the main_view does not have
  -- a "master" layout and can therefore hold dragged views and such without
  -- making a mess with widget sizes.
  self.zone = zone
  self.layout_holder = mimas.Widget()
  self:addWidget(self.layout_holder)
  self.layout_holder:move(0,0)
  self.layout = mimas.HBoxLayout(self.layout_holder)
  self.library = zone.library
  self.library_view = editor.LibraryView(zone.library)
  self.layout:addWidget(self.library_view)
  self.patching_view = editor.PatchingView()
  self.layout:addWidget(self.patching_view)
  self.layout:setSpacing(PADDING)
  self.layout:setContentsMargins(0, 0, 0, 0)
  self.machine_list_view = editor.MachineList('local')
  self:addWidget(self.machine_list_view)
  self.width  = WIDTH
  self.height = HEIGHT
  self:resize(self.width, self.height)
  private.setupMenus(self)
end

function lib:resized(w, h)
  self.width  = w
  self.height = h
  self:placeElements()
end

function lib:placeElements()
  local w, h = self.width, self.height
  self.layout_holder:resize(w, h)
  self.machine_list_view:move(w - self.machine_list_view.width, 0)
  self:update()
end

function lib:addProcessView(view)
  local process = view.process
  self.patching_view:addWidget(view)
  view:move(process.x or 100, process.y or 100)
  -- trigger a full view rebuild once it is positioned
  process:updateView()
end

function lib:addLinkView(view)
  self.patching_view:addWidget(view)
end

--=============================================== Menu setup
function private:setupMenus()
  self.menu_bar = mimas.MenuBar(self)
  local file = self.menu_bar:addMenu('File')
  file:addAction('New...', 'Ctrl+N', function()
    -- Choose folder & name
    private.newProjectAction(self)
  end)
  file:addAction('Open...', 'Ctrl+O', function()
    -- Choose folder & name
    private.openProjectAction(self)
  end)
  local pref = file:addAction('Preferences...', '', function()
    -- Show pref dialog
  end)
  pref:setMenuRole(mimas.PreferencesRole)         
  local special = self.menu_bar:addMenu('Special')
  special:addAction('Stop', 'Ctrl+K', function()
    for k, process in pairs(self.zone.process_watch.processes) do
      if process.online then
        -- FIXME: we should push
        process.req:send(lubyk.quit_url)
      end
    end
  end)
  self:setMenuBar(self.menu_bar)
end  

function private:newProjectAction()
  self.doit = lk.Thread(function()
  self.dlg = mimas.SimpleDialog {
    'Who are you ?',
    {'vbox',
      {'hbox', 'First name', {'input', 'first_name', 'Gaspard'}},
      {'hbox', 'Last name',  {'input', 'last_name',  'Bucher'}},
      {'hbox', 'In lubyk',   {'input', 'dir', lfs.currentdir(), folder=true}},
    },
    {'hbox',
      {'btn', 'Cancel'},
      {'btn', 'OK', default = true},
    },
  }
  --self.dlg = mimas.SimpleDialog {
  --  'Create a new lubyk project.',
  --  {'vbox', box = true,
  --    'Project name',
  --    {'input', 'name'},
  --    'Create project in directory',
  --    {'input', 'dir', folder=true},
  --  },
  --  {'hbox', {},
  --    {'btn', 'cancel'},
  --    {'btn', 'OK', default=true},
  --  },
  --}
  function self.dlg:btn(name)
    if name == 'OK' then
      -- Create project
    end
    self.dlg:hide()
    self.dlg = nil
    self.doit = nil
  end
  self.dlg:resize(400,200)
  self.dlg:show()
end)
end

-- function private:openNewDialog()
--   self.dlg = editor.SimpleDialog {
--     message = string.format('Please enter project path on "%s"', self.host_name),
--     line    = 'Zone',
--     line_value = 'default', -- FIXME: use last value (create editor.Settings)
--     line2   = 'Project path',
--     line2_value = '',       -- FIXME: use last value (create editor.Settings)
--     cancel  = 'Back',
--     ok      = 'Start',
--   }
-- 
--   function self.dlg.cancel(dlg)
--     -- Abort choosing path
--     dlg:close()
--     self.host_name = nil
--     makeHostChooser(self)
--   end
-- 
--   function self.dlg.ok(dlg)
--     -- Start morph server on the host with the given path.
--     self.zone = dlg:line()
--     self.project_path = dlg:line2()
--     dlg:close()
--     self.delegate:startZone(self.zone, self.host_name, self.project_path)
--   end
-- 
--   self.lay:addWidget(self.dlg)
--   if is_local then
--     function self.dlg.widgets.line2.click()
--       self.dlg:openProjectDialog()
--     end
-- 
--     -- open file dialog
--     self.dlg:openProjectDialog()
--   end        
-- end

