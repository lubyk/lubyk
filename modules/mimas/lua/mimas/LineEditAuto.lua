--[[------------------------------------------------------

  mimas.LineEditAuto
  ------------------

  LineEdit with auto-complete

--]]------------------------------------------------------
local lib = {}
mimas.LineEditAuto = lib
local LineEdit = mimas.LineEdit

local super_mt = mimas_core.LineEdit_

-- contants
local POS = 23

-- Methods not in 'LineEditAuto' are mapped to calls on self.line
function lib:__index(key)
  local m = rawget(lib, key)
  if m then
    return m
  else
    m = rawget(super_mt, key)
    if m then
      local function f(self, ...)
        return m(self.line, ...)
      end
      lib[key] = f
      return f
    end
  end
end

setmetatable(lib, {
  -- new method
 __call = function(lib, parent, initial_text, pattern)
  local line = LineEdit(initial_text)
  local self = {
    line      = line,
    pattern   = pattern,
    show_list = true,
    -- FIXME: we need this because we cannot find a useable Lua object with
    -- LineEdit:parent(). This could be fixed with minimal bindings for QWidget (not mimas::Widget).
    parent    = parent,
  }
  setmetatable(self, lib)

  function line.keyboard(key, on)
    return self:keyboard(key, on)
  end

  function line.textEdited(text)
    self:textEdited(text)
  end

  function line.moved(x, y)
    if self.list then
      self.list:move(x, y + POS)
    end
  end

  function line.editingFinished()
    self:editingFinished()
  end
  return self
end})

function lib:editingFinished()
  self:autoFinished()
end

-- Cleanup after editing finished.
function lib:autoFinished()
  if self.list then
    self.list:__gc()
    self.list = nil
  end
end

-- Methods to overwrite in instance
function lib:getCue(text)
  if self.pattern then
    return string.match(text, self.pattern)
  else
    return '', text
  end
end

function lib:cueChanged(cue)
  -- noop
end

function lib:resultCount()
  error('You need to implement obj:resultCount() returning the number of hints')
end

function lib:data(row)
  error('You need to implement obj:data(row) returning hint for table and inline when row is nil')
end

----------------- LineEditAuto methods
local function createList(self)
  local line = self.line
  local list = mimas.ListView()
  self.list = list

  -- FIXME: we cannot use line:parent():addWidget
  self.parent:addWidget(list)
  list:resize(line:width(), 100)
  list:move(line:x(), line:y() + POS)
  list:setStyleSheet[[
    .list { background:rgb(60,60,60); color:white; padding:0;}
    .list::item:selected { background:rgb(40,85,140); color:white; }
  ]]
  list:show()
  --self.list_hidden = true


  function list.rowCount()
    if self.cue then
      return self.result_count
    else
      return 0
    end
  end

  function list.data(row)
    return self:data(row)
  end

  function list.header()
    return nil
  end
end

function lib:textEdited(text)
  if text == self.old_text then
    -- typed del
    text = string.sub(text, 1, string.len(text) - 1)
  end
  self.old_text = text
  self:setCue(self:getCue(text))
end

function lib:setCue(prefix, cue)
  if not prefix then
    return
  end
  self.cue = cue
  self.prefix = prefix
  self:cueChanged(cue)
  self.result_count = self:resultCount()
  if self.show_list then
    if not self.list then
      createList(self)
    end

    self.selected = 0
    self.list:dataChanged()
    -- deselect
    if self.list_hidden then
      self.list_hidden = false
      self.list:show()
    end
  end
  if self.line then
    local data = self:data(nil)
    local ntext = self.prefix .. (data or self.cue)
    self.line:setText(ntext)
    self.line:setSelection(string.len(self.old_text), string.len(ntext))
  end
end

function lib:keyboard(key, on)
  if key == mimas.Key_Up or key == mimas.Key_Down then
    if on then
      local i = self.selected
      if key == mimas.Key_Up then
        i = i - 1
      else
        i = i + 1
      end
      if i < 1 then
        i = self.result_count
      elseif i > self.result_count then
        if self.result_count > 0 then
          i = 1
        else
          i = 0
        end
      end
      self.selected = i
      if i > 0 then
        if self.list then
          self.list:selectRow(i)
        end
        -- update text
        if self.line then
          self.line:setText(self.prefix .. (self:data(i) or self.cue))
        end
      end
    end
    -- do not continue processing
    return
  end
  -- normal op
  return false
end

