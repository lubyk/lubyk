--[[------------------------------------------------------

  editor.DlgChooseInList
  -----------------------

  This is the first widget seen when launching the editor:
  it shows a list of found networks that can be joined or
  proposes to create a new network.

--]]------------------------------------------------------
local lib = mimas.WidgetClass('editor.DlgChooseInList')
editor.DlgChooseInList = lib

--============================================= PRIVATE
-- constants


--============================================= PUBLIC
function lib:init(opts)
  -- Compose widget
  local w = {}
  self.widgets = w
  self.data = opts.data
  w.lay = mimas.VBoxLayout(self.super)

  if opts.message then
    w.select = mimas.Label(opts.message)
    w.lay:addWidget(w.select)
  end

  ---------------------------- List
  w.list = mimas.ListView()
  function w.list.rowCount() return self.data:count()   end
  function w.list.data(row)  return self.data:data(row) end
  function w.list.click(x, y)
    local i, j = w.list:indexAt(x, y)
    self:selected(opts.data[i])
  end

  w.lay:addWidget(w.list)
  w.list:setStyleSheet[[
    .list {background:#333; border:1px solid #666}
    .list::item { border-bottom:1px solid #666; color:white; background:#6E4E24}
  ]]

  w.btn_lay = mimas.HBoxLayout()
  w.btn_lay:addStretch()

  if opts.cancel then
    ---------------------------- 'Cancel' button
    w.cancel = mimas.Button(opts.cancel)
    function w.cancel.click()
      self:cancel()
    end
    w.btn_lay:addWidget(w.cancel)
  end

  if opts.new then
    ---------------------------- 'New' button
    w.new = mimas.Button(opts.new)
    w.new:setStyle[[
      background-color:#EAA844;
    ]]
    function w.new.click()
      self:new()
    end
    w.btn_lay:addWidget(w.new)
  end

  w.lay:addWidget(w.btn_lay)
end

function lib:delete()
  self.widget = nil
  -- Delete widget
  app:post(function()
    self.super:__gc()
  end)
end

function lib:selected()
  error("Please set 'selected' callback for DlgChooseInList")
end

function lib:new()
  error("Please set 'new' callback for DlgChooseInList")
end

function lib:cancel()
  self:close()
end