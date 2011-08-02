--[[------------------------------------------------------

  editor.SimpleDialog
  -----------------------

  This dialog widget can be used to easily create a select
  list of input field with buttons.

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
    w.list:setModel(opts.data)
    function w.list.select(...)
      self:select(...)
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
    if opts.ok then
      function w.line.editingFinished(text)
        self:ok()
      end
    end
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

  if opts.ok then
    ---------------------------- 'New' button
    w.ok = mimas.Button(opts.ok)
    w.ok:setStyle[[
      background-color:#EAA844;
    ]]
    function w.ok.click()
      self:ok()
    end
    w.btn_lay:addWidget(w.ok)
  end

  w.lay:addWidget(w.btn_lay)
end

function lib:select()
  error("Please set 'select' callback for SimpleDialog")
end

function lib:ok()
  error("Please set 'ok' callback for SimpleDialog")
end

--- Return the value in the LineEdit.
function lib:text()
  local line = self.widgets.line
  if line then
    return line:text()
  else
    return nil
  end
end

function lib:cancel()
  self:close()
end