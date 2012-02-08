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
local PADDING = 2
local settings = editor.Settings

function lib:init(zone)
  -- The layout holder is just there so that the main_view does not have
  -- a "master" layout and can therefore hold dragged views and such without
  -- making a mess with widget sizes.
  self:setWindowTitle(zone.name)
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
  self.width  = settings.main_view.w
  self.height = settings.main_view.h
  self:resize(self.width, self.height)
  self:move(settings.main_view.x, settings.main_view.y)
  private.setupMenus(self)
end

function lib:moved(x, y)
  local v = settings.main_view
  v.x = x
  v.y = y
  settings:save()
end

function lib:resized(w, h)
  local v = settings.main_view
  v.w = w
  v.h = h
  settings:save()

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
  local show = settings.show
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
    settings.show[title] = not view.hidden
    settings:save()
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
      parent = self,
      'Create a new lubyk project.',
      {'vbox', box = true,
        'Project name',
        {'input', 'name'},
        'Create project in directory',
        {'input', 'dir', os.getenv('HOME'), folder=true},
      },
      {'hbox', {},
        {'btn', 'cancel'},
        {'btn', 'OK', default=true},
      },
    }

    function self.dlg.btn(dlg, btn_name)
      if btn_name == 'OK' then
        -- Create project
        local name = dlg.form.name
        local path = dlg.form.dir .. '/' .. name .. '/' .. name .. '.lkp'
        if not lk.exist(path) then
          lk.writeall(path, '')
        end
        app:openFile(path)
      end
      dlg:hide()
      self.dlg = nil
      self.doit = nil
    end
    self.dlg:resize(400,200)
    self.dlg:show()
  end)
end

function private.dialog:openProject()
  local base = (settings.open_recent or {})[1] or os.getenv('HOME')
  local path = self:getOpenFileName(
    'Choose project file',
    base,
    'Lubyk project (*.lkp)'
  )
  if path then
    app:openFile(path)
  end
end

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
  --local w, h = 
  --self.machine_list
end
