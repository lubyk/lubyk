--[[------------------------------------------------------

  editor.NodeLineEdit
  -------------------

  This is a LineEdit with autocomplete specific for node
  creation (uses editor.Library to find prototypes).

--]]------------------------------------------------------

-- new method
function editor.NodeLineEdit(parent, initial_name, db)
  local self = mimas.LineEditAuto(parent, initial_name, '^(.+)=( *)(.*)')
  self.db = db

  function self:getCue(text)
    local prefix, space, cue = string.match(text, self.pattern)
    if prefix then
      local db_cue = cue
      if cue == '' then
        db_cue = string.match(prefix, '^([^0-9 ]+)[0-9]* *$') or prefix
      end
      self.list_pattern = '%' .. db_cue .. '%'
      self.line_pattern = db_cue .. '%'
      return prefix .. '=' .. space, cue
    else
      return nil
    end
  end

  function self:resultCount()
    return self.db:nodeCount(self.db_pattern)
  end

  function self:data(row)
    local data
    if row then
      -- data for list
      data = self.db:node(self.list_pattern, row)
    else
      -- data for inline
      data = self.db:node(self.line_pattern)
    end
    if data then
      return data.name
    else
      return nil
    end
  end

  return self
end
