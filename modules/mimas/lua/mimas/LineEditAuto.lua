--[[------------------------------------------------------

  mimas.LineEditAuto
  ------------------

  LineEdit with auto-complete

--]]------------------------------------------------------
local lib = {}
mimas.LineEditAuto = lib
local LineEdit = mimas.LineEdit

local super_mt = mimas_core.LineEdit_
lib.__index = lib

--function lib.__index(lib, key)
--  local m = rawget(lib, key)
--  if m then
--    return m
--  else
--    m = rawget(super_mt, key)
--    if m then
--      return function(self, ...)
--        return m(self.line, ...)
--      end
--    end
--  end
--end

setmetatable(lib, {
  -- new method
 __call = function(lib, initial_text, pattern)
  local line = LineEdit(initial_text)
  local self = {
    line      = line,
    pattern   = pattern,
    show_list = true,
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
      self.list:move(x, y + 20)
    end
  end
  return self
end})

function lib:widget()
  return self.line:widget()
end

function lib:selectAll()
  self.line:selectAll()
end

function lib:move(...)
  self.line:move(...)
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
  local list = mimas.TableView()
  self.list = list

  --list:setAlternatingRowColors(true)
  list:setVisibleHeaders(mimas.Vertical + mimas.Horizontal, false)
  list:setGridStyle(mimas.NoPen)
  -- FIXME: we cannot use line:parent():addWidget
  self.parent:addWidget(list)
  list:resize(line:width(), 100)
  list:move(line:x(), line:y() + 20)
  list:show()
  --self.list_hidden = true

  function list.columnCount()
    return 1
  end

  function list.rowCount()
    if self.cue then
      return self.result_count
    else
      return 0
    end
  end

  function list.data(row, column)
    if column == 1 then
      return self:data(row)
    else
      return nil
    end
  end

  function list.header()
    return nil
  end
end

local function removeList(self)
  if self.list then
    self.list:delete()
    self.list = nil
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
  elseif key == mimas.Key_Return then
    self.list:hide()
    self.line:hide()
    return
  end
  -- normal op
  return false
end

