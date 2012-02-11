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

  --=============================================== Prototypes library
  self.library_view = editor.LibraryView(zone.library, zone)
  self.layout:addWidget(self.library_view)

  --=============================================== Patch view
  self.patch_view = editor.PatchView(self)
  self.layout:addWidget(self.patch_view, 2)
  self.layout:setSpacing(PADDING)
  self.layout:setContentsMargins(0, 0, 0, 0)

  --=============================================== Machine list
  self.machine_list = editor.MachineList(self)
  self:addWidget(self.machine_list)

  --=============================================== Control views
  self.control_tabs = editor.ControlTabs(self)
  self.layout:addWidget(self.control_tabs, 1)

  --=============================================== Controls library
  self.ctrl_library_view = editor.LibraryView(zone.ctrl_library, zone)
  self.layout:addWidget(self.ctrl_library_view)

  self.w = settings.main_view.w
  self.h = settings.main_view.h
  self:resize(self.w, self.h)
  self:move(settings.main_view.x, settings.main_view.y)
  private.setupMenus(self)
  -- Display open recent / create new dialog until something appears on the
  -- network.
  private.dialog.start(self)
end

function lib:moved(x, y)
  local v = settings.main_view
  v.x = x
  v.y = y
  settings:save(true)
end

function lib:resized(w, h)
  local v = settings.main_view
  v.w = w
  v.h = h
  settings:save(true)
  self.w = w
  self.h = h

  self.layout_holder:resize(w, h)
  self.machine_list:updatePosition()
  private.centerDlg(self)
end

function lib:addProcessView(view)
  local process = view.process
  self.patch_view:addWidget(view)
  view:move(process.x or 100, process.y or 100)
  -- trigger a full view rebuild once it is positioned
  process:updateView()
  view:show()
end

function lib:addLinkView(view)
  self.patch_view:addWidget(view)
  view:show()
end

function lib:hideDialog()
  local dlg = self.dlg
  if dlg then
    dlg:hide()
    dlg:__gc()
    self.dlg = nil
  else
    self.dlg_hidden = true
  end
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
  private.setupShowAction(self, menu, 'Library', 'Ctrl+L', show.Library,     self.library_view)
  private.setupShowAction(self, menu, 'Patch',   'Ctrl+E', show.Patch,       self.patch_view)
  private.setupShowAction(self, menu, 'View',    'Ctrl+I', show.View,        self.control_tabs)
  private.setupShowAction(self, menu, 'Controls', 'Ctrl+O', show.Controls, self.ctrl_library_view)

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

function private:setupShowAction(menu, title, shortcut, show, view)
  local action = menu:addAction(title, shortcut, function(action)
    if view.hidden then
      view:show()
    else
      view:hide()
    end
    view.hidden = not view.hidden
    settings.show[title] = not view.hidden
    settings:save()
    action:setChecked(not view.hidden)
    private.centerDlg(self)
  end)
  action:setCheckable(true)
  if not show then
    view:hide()
    view.hidden = true
  else
    view:show()
    view.hidden = false
    action:setChecked(true)
  end
end

function private.dialog:newProject()
  local base = (settings.open_recent or {})[1]
  if base then
    -- Go from lkp file to directory containing project
    -- folder.
    base = lk.directory(base)
    base = lk.directory(base)
  else
    base = os.getenv('HOME')
  end
  --self.doit = lk.Thread(function()
  self.dlg = mimas.SimpleDialog {
    parent = self,
    'Create a new lubyk project.',
    { 
      'vbox', box = true,
      'Project name',
      {'input', 'name'},
      'Create project in directory',
      {'input', 'dir', base, folder=true},
    },
    {
      'hbox', {},
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
    --self.doit = nil
  end
  self.dlg:resize(400,200)
  self.dlg:show()
  --end)
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

function private.dialog:start()
  if self.dlg_hidden then
    -- Do not show
    self.dlg_hidden = nil
    return
  end

  local dlg = mimas.SimpleDialog {
    flag = mimas.WidgetFlag,
    'Start or create a project',
    {'list', settings.open_recent},
    {'hbox',
      {}, -- stretch
      {'space', 120},
      {'btn', 'New...'},
      {'btn', 'Open...'},
      {'btn', 'Open Recent', default = true},
    },
  }
  self.dlg = dlg
  self:addWidget(dlg)

  function dlg.btn(dlg, btn_name)
    if btn_name == 'New...' then
      private.dialog.newProject(self)
    elseif btn_name == 'Open...' then
      private.dialog.openProject(self)
    elseif #settings.open_recent > 0 then
      app:openFile(settings.open_recent[1])
      lk.Thread(function()
        -- delete later
        self:hideDialog()
      end)
    end
  end

  dlg.max_list_len = 30
  function dlg.list(dlg, path)
    if path then
      app:openFile(path)
      lk.Thread(function()
        -- delete later
        self:hideDialog()
      end)
    end
  end
  dlg.widgets.lay:setContentsMargins(15,15,15,15)
  function dlg:paint(p, w, h)
    p:setBrush(mimas.Color(0, 0, 0.2))
    p:setPen(3, mimas.Color(0.2, 0.8, 0.3))
    p:drawRoundedRect(4, 4, w-8, h-8, 10)
  end
  private.centerDlg(self)
end


function private:centerDlg()
  local dlg = self.dlg
  if dlg then
    local w, h = dlg:width(), dlg:height()
    local pw, ph = self.w, self.h
    -- left and right padding
    local ll, lr = 0, 0
    if not self.library_view.hidden then
      ll = self.library_view:width()
    end
    if not self.ctrl_library_view.hidden then
      lr = self.ctrl_library_view:width()
    end
    dlg:move(ll + (pw-w-ll-lr)/2, (ph-h)/2)
  end
end
