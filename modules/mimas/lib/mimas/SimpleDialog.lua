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
function lib:init(def)
  self.widgets = {lay = mimas.VBoxLayout(self)}
  self.form    = {}
  for _,elem in ipairs(def) do
    private.makeWidget(self, self.widgets.lay, elem)
  end
  -- Can we suspend execution until dialog ends ?
  self:resize(self:minimumSize())
  return self
end

function lib:btn(name)
  -- noop
end

function lib:minimumSize()
  return self.widgets.lay:minimumSize()
end

--=============================================== PRIVATE
local function makeWidget(main, parent, def)
  if type(def) == 'string' then
    def = {'label', def}
  end

  local t = def[1]
  local elem
  if t == 'label' then
    elem = mimas.Label(def[2])
  elseif t == 'vbox' or t == 'hbox' then
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
    if def.folder or def.file then
      makeWidget(main, parent, {'hbox',
        {'input', def[2], def[3]},
        {'btn', '...', def[2]..'btn'},
      })
      local btn = main.widgets[def[2]..'btn']
      function btn:click(x, y, op)
        if op == mimas.MousePress then
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
      end
    else
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
            default_btn:click(0,0,mimas.MousePress)
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
    elem:setModel(def[2])
    function elem:select(...)
      main:list(...)
    end
    elem:setStyleSheet[[
      .list {background:#333; border:1px solid #666}
      .list::item { border-bottom:1px solid #666; color:white; background:#6E4E24}
    ]]
  elseif t == nil then
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

