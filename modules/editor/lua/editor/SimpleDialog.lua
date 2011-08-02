--[[------------------------------------------------------

  editor.SimpleDialog
  -----------------------

  This is the first widget seen when launching the editor:
  it shows a list of found networks that can be joined or
  proposes to create a new network.

--]]------------------------------------------------------
local lib = mimas.WidgetClass('editor.SimpleDialog')
editor.SimpleDialog = lib

--============================================= PRIVATE
-- constants


--============================================= PUBLIC
function lib:init(opts)
  -- Compose widget
  local w = {}
  self.widgets = w
  w.lay = mimas.VBoxLayout(self.super)

  if opts.message then
    w.select = mimas.Label(opts.message)
    w.lay:addWidget(w.select)
  end

  if opts.data then
    ---------------------------- List
    w.list = mimas.ListView()
    function w.list.rowCount() return opts.data:count()   end
    function w.list.data(row)  return opts.data:data(row) end
    function w.list.click(x, y)
      local i, j = w.list:indexAt(x, y)
      self:selected(opts.data[i])
    end

    w.lay:addWidget(w.list)
    w.list:setStyleSheet[[
      .list {background:#333; border:1px solid #666}
      .list::item { border-bottom:1px solid #666; color:white; background:#6E4E24}
    ]]
  end

  if opts.line then
    ---------------------------- LineEdit
    w.edit_lay = mimas.HBoxLayout()
    --- Label
    w.line_lbl = mimas.Label(opts.line)
    w.edit_lay:addWidget(w.line_lbl)
    --- LineEdit
    w.line     = mimas.LineEdit(opts.line_value or '')
    w.edit_lay:addWidget(w.line)
    w.lay:addWidget(w.edit_lay)
  end

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

function lib:selected()
  error("Please set 'selected' callback for SimpleDialog")
end

function lib:new()
  error("Please set 'new' callback for SimpleDialog")
end

function lib:cancel()
  self:close()
end