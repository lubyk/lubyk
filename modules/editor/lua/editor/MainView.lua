--[[------------------------------------------------------

  editor.MainView
  ---------------

  Main view holding the different parts of the editor
  (LibraryView, HelpView, PatchView and ControlView).

--]]------------------------------------------------------
local lib = mimas.WidgetClass()
editor.MainView = lib

-- constants
local WIDTH   = 600
local HEIGHT  = 400
local PADDING = 2

function lib:init(main)
  self.layout = mimas.HBoxLayout(self.super)
  self.library_view = editor.LibraryView(main.library)
  self.layout:addWidget(self.library_view)
  self.patching_view   = editor.PatchingView()
  self.layout:addWidget(self.patching_view)
  self.layout:setSpacing(PADDING)
  self.layout:setContentsMargins(0, 0, 0, 0)
  self.process_list_view = editor.ProcessList()
  self:addWidget(self.process_list_view)
  self.width  = WIDTH
  self.height = HEIGHT
  self:resize(self.width, self.height)
end

function lib:resized(w, h)
  self.width  = w
  self.height = h
  self:placeElements()
end

function lib:placeElements()
  local w, h = self.width, self.height
  self.process_list_view:move(w - self.process_list_view.width, 0)
  self:update()
end

function lib:addProcessView(view)
  local process = view.process
  self.patching_view:addWidget(view)
  view:move(process.x or 100, process.y or 100)
end
