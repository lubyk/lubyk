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
    function w.list.select(list, ...)
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
    w.line_lay = mimas.HBoxLayout()
    --- Label
    w.line_lbl = mimas.Label(opts.line)
    w.line_lay:addWidget(w.line_lbl)
    --- LineEdit
    w.line     = mimas.LineEdit(opts.line_value or '')
    if opts.ok then
      function w.line.keyboard(line, key, on)
        if key == mimas.Key_Enter and on then
          self:ok()
        else
          -- pass to LineEdit
          return false
        end
      end
    end
    w.line_lay:addWidget(w.line)
    w.lay:addWidget(w.line_lay)
  end

  if opts.line2 then
    ---------------------------- LineEdit
    w.line2_lay = mimas.HBoxLayout()
    --- Label
    w.line2_lbl = mimas.Label(opts.line2)
    w.line2_lay:addWidget(w.line2_lbl)
    --- LineEdit
    w.line2     = mimas.LineEdit(opts.line2_value or '')
    if opts.ok then
      function w.line2.keyboard(line2, key, on)
        if key == mimas.Key_Enter and on then
          self:ok()
        else
          -- pass to LineEdit
          return false
        end
      end
    end
    w.line2_lay:addWidget(w.line2)

    if opts.line2_file_dialog then
      w.line2_btn = mimas.Button('Open...')
      function w.line2_btn.click()
        self:openFileDialog()
      end
      w.line2_lay:addWidget(w.line2_btn)
    end
    w.lay:addWidget(w.line2_lay)
  end

  w.btn_lay = mimas.HBoxLayout()
  w.btn_lay:addStretch()

  if opts.cancel then
    ---------------------------- 'Cancel' button
    w.cancel = mimas.Button(opts.cancel)
    function w.cancel.click()
      print('cancel')
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
      print('ok...')
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
function lib:line()
  local line = self.widgets.line
  if line then
    return line:text()
  else
    return nil
  end
end

--- Return the value in the LineEdit.
function lib:line2()
  local line2 = self.widgets.line2
  if line2 then
    return line2:text()
  else
    return nil
  end
end

function lib:cancel()
  self:close()
end

function lib:openFileDialog()
  local line2 = self.widgets.line2
  if line2 then
    local path = self:getOpenFileName('Select lua script', nil, 'Lua files (*.lua)')
    if path then
      line2:setText(path)
    end
  end
end

function lib:openProjectDialog()
  local line2 = self.widgets.line2
  if line2 then
    local path = self:getExistingDirectory('Select project directory')
    if path then
      line2:setText(path)
    end
  end
end
