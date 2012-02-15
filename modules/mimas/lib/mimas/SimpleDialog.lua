--[[------------------------------------------------------

  mimas.SimpleDialog
  -----------------------

  This dialog widget can be used to easily create a select
  list of input field with buttons.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
mimas.SimpleDialog = lib

local private = {}
--============================================= PUBLIC
function lib.new(def)
  local self
  if def.parent then
    self = mimas.Widget(def.parent, def.flag or mimas.SheetFlag)  
  else
    self = mimas.Widget()
  end
  setmetatable(self, lib)
  self:set(def)
  return self
end

function lib:btn(name)
  -- noop
end

function lib:minimumSize()
  return self.widgets.lay:minimumSize()
end

function lib:show()
  self.super:show()
  self:resize(self:minimumSize())
end

-- FIXME: Set does not resize correctly...
function lib:set(def)
  if self.widgets then
    -- Delete layout
    self.widgets.lay:__gc()
  end
  self.widgets = {lay = mimas.VBoxLayout(self)}
  self.form    = {}
  for _,elem in ipairs(def) do
    private.makeWidget(self, self.widgets.lay, elem)
  end
  if def.flag == mimas.WidgetFlag then
    -- FIXME: do this properly...
    -- fix missing background
    function self:paint(p, w, h)
      p:setPen(1, mimas.Color(0, 0, 0.5, 0.5))
      p:setBrush(mimas.Color(0, 0, 0.2))
      p:drawRect(0, 0, w, h)
    end
  end
end


--=============================================== PRIVATE
local function makeWidget(main, parent, def)
  if type(def) == 'string' then
    def = {'label', def}
  end

  local t = def[1]
  local elem
  if t == 'label' then
    --=============================================== label
    elem = mimas.Label(def[2])


  elseif t == 'vbox' or t == 'hbox' then
    --=============================================== vbox/hbox layout
    local constr
    if t == 'vbox' then
      constr = mimas.VBoxLayout
    else
      constr = mimas.HBoxLayout
    end
    local vb
    if def.box then
      if type(def.box) == 'string' then
        elem = mimas.GroupBox(def.box)
      else
        elem = mimas.GroupBox()
      end
      vb = constr(elem)
      elem.vbox = vb
    else
      vb = constr()
      elem = vb
    end
    for i,sub in ipairs(def) do
      if i > 1 then
        makeWidget(main, vb, sub)
      end
    end


  elseif t == 'input' then
    --=============================================== file/folder selection
    if def.folder or def.file then
      makeWidget(main, parent, {'hbox',
        {'input', def[2], def[3]},
        {'btn', '...', def[2]..'btn'},
      })
      local btn = main.widgets[def[2]..'btn']
      function btn:click()
        local path = main.form[def[2]]
        if def.folder then
          path = main:getExistingDirectory(def.message or 'Select directory', path)
        else
          local base
          if path then
            base = lk.directory(path)
          end
          path = main:getOpenFileName(def.message or 'Select lua script', base)
        end
        if path then
          main.form[def[2]] = path
          main.widgets[def[2]]:setText(path)
        end
      end


    else
      --=============================================== input field
      elem = mimas.LineEdit(def[3])
      main.widgets[def[2]] = elem
      main.form[def[2]] = def[3]
      function elem:editingFinished(text)
        main.form[def[2]] = text
      end
      function elem:keyboard(key, on)
        if on and (key == mimas.Key_Enter or key == mimas.Key_Return) then
          local default_btn = main.widgets.default_btn
          if default_btn then
            self:editingFinished(self:text())
            default_btn:click()
          else
            -- pass to LineEdit
            return true
          end
        else
          return true
        end
      end
    end


  elseif t == 'btn' then
    elem = mimas.Button(def[2], function()
      main:btn(def[3] or def[2])
    end)
    -- access to personnalize button behavior
    main.widgets[def[3] or def[2]] = elem
    if def.default then
      main.widgets.default_btn = elem
      elem:setStyle[[
        background-color:#EAA844;
      ]]
    end


  elseif t == 'list' then
    ---------------------------- List
    elem = mimas.ListView()
    main.widgets.list = elem
    local data = def[2]
    if data.type ~= 'mimas.DataSource' then
      function elem:rowCount()
        return #data
      end

      function elem:data(row_i)
        local n = data[row_i]
        if main.max_list_len and n and string.len(n) > main.max_list_len then
          return '...' .. string.sub(n, -main.max_list_len, -1)
        else
          return n
        end
      end

      function elem:select(i)
        local r = main:list(data[i])
        if r == nil then
          -- continue list selection
          return true
        else
          return r
        end
      end

      function elem:keyboard(key, on)
        if on and (key == mimas.Key_Enter or key == mimas.Key_Return) then
          local selected = elem:selectedIndexes()
          if selected[1] then
            main:list(data[selected[1][1]])
          else
            local default_btn = main.widgets.default_btn
            if default_btn then
              default_btn:click()
            end
          end
        end
        -- continue processing with QListView's keyboard handling
        return true
      end

    else
      error('DataSource not supported in SimpleDialog yet...')
      elem:setModel(data)
    end


  elseif t == 'space' then
    ---------------------------- Space
    parent:addSpacing(def[2])


  elseif t == nil then
    ---------------------------- Stretch
    parent:addStretch()
  end

      
  if elem then
    if def.style_sheet then
      elem:setStyleSheet(def.style_sheet)
    elseif def.style then
      elem:setStyle(def.style)
    end
    parent:addWidget(elem)
    -- protect from gc
    table.insert(main.widgets, elem)
  end
  return elem
end
private.makeWidget = makeWidget

