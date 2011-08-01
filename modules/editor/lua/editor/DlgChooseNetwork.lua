--[[------------------------------------------------------

  editor.DlgChooseNetwork
  -----------------------

  This is the first widget seen when launching the editor:
  it shows a list of found networks that can be joined or
  proposes to create a new network.

--]]------------------------------------------------------
local lib = mimas.WidgetClass('editor.DlgChooseNetwork')
editor.DlgChooseNetwork = lib

--============================================= PRIVATE
-- constants


--============================================= PUBLIC
function lib:init(data, select_callback)
  -- Compose widget
  local w = {}
  self.widgets = w
  self.data = data
  self.selected = select_callback
  w.lay = mimas.VBoxLayout(self.super)
  w.select = mimas.Label('Choose network')
  w.lay:addWidget(w.select)
  w.list = mimas.ListView()
  function w.list.rowCount() return data:count()   end
  function w.list.data(row)  return data:data(row) end
  function w.list.click(x, y)
    local i, j = w.list:indexAt(x, y)
    select_callback(data[i])
  end

  w.lay:addWidget(w.list)
  w.list:setStyleSheet[[
    .list {background:#333; border:1px solid #666}
    .list::item { border-bottom:1px solid #666; color:white; background:#6E4E24}
  ]]
  w.new = mimas.Button('New')
  w.new:setStyle[[
    background-color:#EAA844;
  ]]
  function w.new.click()
    self:click()
  end
  w.new_lay = mimas.HBoxLayout()
  w.new_lay:addStretch()
  w.new_lay:addWidget(w.new)
  w.lay:addWidget(w.new_lay)
end

function lib:delete()
  self.widget = nil
  -- Delete widget
  app:post(function()
    self.super:__gc()
  end)
end

function lib:click()
  -- noop
end