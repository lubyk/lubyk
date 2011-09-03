--[[------------------------------------------------------

  editor.SplashScreen
  -------------------

  This is the splash screen seen when launching the editor.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget', mimas.SplashScreenFlag)
editor.SplashScreen = lib

--============================================= PRIVATE
-- constants
local WIDTH  = 400
local HEIGHT = 400
local CHOOSER_W = 250
local CHOOSER_H = 200
local PADDING = 20

--============================================= PUBLIC
function lib:init(delegate)
  self.delegate = delegate
  self.title = mimas.Label('Lubyk')
  self.title:setStyle 'font-size:48px; color:white'
  self:addWidget(self.title)
  self.title:move(PADDING, PADDING)

  self.copy = mimas.Label('Copyright (c) Gaspard Bucher 2011.')
  self.copy:setStyle 'font-size:12px; color:white'
  self:addWidget(self.copy)
  self.copy:move(PADDING, self.title:height() + 2*PADDING)

  self.chooser = editor.ZoneChooser(delegate)
  self:addWidget(self.chooser)
  self.chooser:resize(CHOOSER_W, CHOOSER_H)
  self.chooser:move(WIDTH - PADDING - CHOOSER_W, HEIGHT - PADDING - CHOOSER_H)
  self:resize(WIDTH, HEIGHT)
  self:setStyle 'background:transparent;'
  self:show()
end

function lib:paint(p, w, h)
  local bp = 2 -- full box padding
  local arc_radius = 15
  local bg = mimas.Color(0.6, 0.5, 0.5)

  local pen = mimas.EmptyPen
  p:setBrush(bg)
  p:setPen(pen)
  p:drawRoundedRect(bp, bp, w - 2*bp, h - 2*bp, arc_radius)
end
