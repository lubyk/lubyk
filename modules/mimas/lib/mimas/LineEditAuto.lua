--[[------------------------------------------------------

  mimas.LineEditAuto
  ------------------

  LineEdit with auto-complete

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'LineEdit')
mimas.LineEditAuto = lib

-- contants
local POS = 23

function lib:init(parent, initial_text, pattern)
  self:setText(initial_text)
  self.pattern   = pattern
  self.show_list = true
  -- FIXME: we need this because we cannot find a useable Lua object with
  -- LineEdit:parent(). This could be fixed with minimal bindings for QWidget (not mimas::Widget).
  self.parent    = parent
end


function lib:moved(x, y)
  if self.list then
    self.list:move(x, y + POS)
  end
end

function lib:editingFinished()
  self:autoFinished()
end

-- Cleanup after editing finished.
function lib:autoFinished()
  if self.list then
    if self.list then
      self.list:__gc()
      self.list = nil
    end
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
  local list = mimas.ListView()
  self.list = list

  -- FIXME: we cannot use self:parent():addWidget
  self.parent:addWidget(list)
  list:resize(self:width(), 100)
  list:move(self:x(), self:y() + POS)
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

  function list.data(list, row)
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
    self.list:reset()
    -- deselect
    if self.list_hidden then
      self.list_hidden = false
      self.list:show()
    end
  end
  local data = self:data(nil)
  local ntext = self.prefix .. (data or self.cue)
  self:setText(ntext)
  self:setSelection(string.len(self.old_text), string.len(ntext))
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
        self:setText(self.prefix .. (self:data(i) or self.cue))
      end
    end
    -- do not continue processing
    return
  end
  -- normal op
  return false
end

