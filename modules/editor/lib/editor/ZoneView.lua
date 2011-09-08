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
  self.process_list_view = editor.ProcessList()
  self:addWidget(self.process_list_view)
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
  self.process_list_view:move(w - self.process_list_view.width, 0)
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
function private.setupMenus(self)
  self.menu_bar = mimas.MenuBar(self)
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
