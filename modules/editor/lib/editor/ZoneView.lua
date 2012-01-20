--[[------------------------------------------------------

  editor.ZoneView
  ---------------

  Main view holding the different parts of the editor
  (LibraryView, HelpView, PatchView and ControlView).

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'MainWindow')
editor.ZoneView = lib
local private = {dialog = {}}

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
  self.patch_view = editor.PatchView(zone)
  self.layout:addWidget(self.patch_view, 2)
  self.layout:setSpacing(PADDING)
  self.layout:setContentsMargins(0, 0, 0, 0)
  self.machine_list = editor.MachineList(self.zone)
  self:addWidget(self.machine_list)
  --self.layout:addWidget(self.machine_list, 0, mimas.AlignRight)
  self.control_tabs = editor.ControlTabs(self.zone)
  self.layout:addWidget(self.control_tabs, 1)
  self.width  = WIDTH
  self.height = HEIGHT
  self:resize(self.width, self.height)
  private.setupMenus(self)
end

function lib:resized(w, h)
  self.width  = w
  self.height = h
  self.layout_holder:resize(w, h)
  self.machine_list:updatePosition()
end

function lib:addProcessView(view)
  local process = view.process
  self.patch_view:addWidget(view)
  view:move(process.x or 100, process.y or 100)
  -- trigger a full view rebuild once it is positioned
  process:updateView()
end

function lib:addLinkView(view)
  self.patch_view:addWidget(view)
end

--=============================================== Menu setup
function private:setupMenus()
  self.menu_bar = mimas.MenuBar(self)

  --=============================================== File
  local menu = self.menu_bar:addMenu('File')
  local action
  action = menu:addAction('New...', 'Ctrl+N', function()
    -- Choose folder & name
    private.dialog.newProject(self)
  end)
  action = menu:addAction('Open...', 'Ctrl+O', function()
    -- Choose folder & name
    private.dialog.openProject(self)
  end)
  action = menu:addAction('Preferences...', '', function()
    -- Show pref dialog
  end)
  action:setMenuRole(mimas.PreferencesRole)         

  --=============================================== Project
  menu = self.menu_bar:addMenu('Project')
  action = menu:addAction('New view', 'Ctrl+Shift+I', function()
    private.dialog.addView(self)
  end)
  action = menu:addAction('Commit', 'Ctrl+S', function()
  end)

  --=============================================== Show
  local show = Lubyk.editor.show
  menu = self.menu_bar:addMenu('Show')
  private.setupShowAction(menu, 'Library', 'Ctrl+L', show.Library, self.library_view)
  private.setupShowAction(menu, 'Patch',   'Ctrl+E', show.Patch,   self.patch_view)
  private.setupShowAction(menu, 'View',    'Ctrl+I', show.View,    self.control_tabs)

  --=============================================== Special
  local menu = self.menu_bar:addMenu('Special')
  action = menu:addAction('Stop', 'Ctrl+Shift+K', function()
    for k, process in pairs(self.zone.process_watch.processes) do
      if process.online then
        process.push:send(lubyk.quit_url)
      end
    end
  end)
  action = menu:addAction('Start Stem Cell', 'Ctrl+Shift+C', function()
    self.zone:startStemCell()
  end)

  self:setMenuBar(self.menu_bar)
end  

-- self == menu
function private:setupShowAction(title, shortcut, show, view)
  local action = self:addAction(title, shortcut, function(action)
    if view.hidden then
      view:show()
    else
      view:hide()
    end
    view.hidden = not view.hidden
    action:setChecked(not view.hidden)
  end)
  action:setCheckable(true)
  if not show then
    view:hide()
    view.hidden = true
  else
    action:setChecked(true)
  end
end

function private.dialog:newProject()
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

-- function private.dialog:openProject()
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

function private.dialog:addView()
  local dlg = mimas.SimpleDialog {
    'Create a new view',
    {'vbox', box=true, style='background: #222',
      'view name',
      {'input', 'name', 'base'},
    },
    {'hbox',
      {}, -- this adds stretch
      {'btn', 'Cancel'},
      {'btn', 'OK', default=true},
    },
  }
  self.dlg = dlg

  function dlg.btn(dlg, name)
    if name == 'OK' then
      -- create view
      local morph = self.zone.morph
      if morph then
        morph.push:send(lubyk.update_url, {
          views = {
            [dlg.form.name] = {},
          }
        })
      end
    else
      -- do nothing
      dlg:close()
    end
  end

  function dlg.closed()
    self.dlg = nil
  end

  dlg:show()
end

-- Keep machine list centered/left or right depending on Patch and ControlView
-- visibility.
function private:positionMachineList()
  local view = self.machine_list
  local w, h = 
  self.machine_list
end
