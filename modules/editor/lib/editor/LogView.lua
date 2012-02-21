--[[------------------------------------------------------

  editor.LogView
  --------------

  The LogView is a top floating window that displays
  messages and errors from remote processes and nodes.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget', mimas.ToolFlag)
editor.LogView = lib
lib.MaxLogSize = 50
local private = {}
local settings = editor.Settings
local log_show = settings.log_show

function lib:init(zone)
  self:setAttribute(mimas.WA_MacAlwaysShowToolWindow, true)
  self.zone  = zone
  local lv = settings.log_view
  self:move(lv.x, lv.y)
  self:resize(lv.w, lv.h)
  self:setFocusPolicy(mimas.ClickFocus)
  self:setFocus(false)
  self.lay = mimas.VBoxLayout(self)
  self.data = {sz = 0}
  self.view_data = self.data
  private.makeButtons(self)
  private.makeList(self)
  private.filterChanged(self)
end

local fmt = string.format
local last = 'three'
function lib:addMessage(url, typ, msg)
  -- If we scroll to row x: keep this row in view.
  msg = string.gsub(msg, '^%s*(.-)%s*$', '%1')
  local data = self.data
  local entry = {
    url = url,
    msg = msg,
    html = fmt([[<html><body class='%s'><p class='%s'><span class='url'>%s</span> %s</p></body></html>]], last, typ, url, msg),
    typ = typ,
  }
  table.insert(data, entry)
  if data.sz >= self.MaxLogSize then
    table.remove(data, 1)
  else
    data.sz = data.sz + 1
  end

  if not self.locked then
    local data = self.view_data
    if log_show[typ] then
      table.insert(data, entry)
      if data.sz >= self.MaxLogSize then
        table.remove(data, 1)
      else
        data.sz = data.sz + 1
      end
    end

    self.list:reset()
    self.list:scrollToBottom()
  end

  -- 3 stage color changes to avoid blinking effect.
  if last == 'one' then
    last = 'two'
  elseif last == 'two' then
    last = 'three'
  else
    last = 'one'
  end

end

local Vertical = mimas.Vertical

function lib:closed()
  self.zone.view.log_view = nil
  if sched.should_run then
    -- This is not a user close operation.
    editor.Settings.show.Log = false
    editor.Settings:save()
  end
end

function lib:moved(x, y)
  local v = settings.log_view
  v.x = x
  v.y = y
  settings:save(true)
end

function lib:resized(w, h)
  local v = settings.log_view
  v.w = w
  v.h = h
  settings:save(true)
  self.w = w
  self.h = h
end

--=============================================== PRIVATE

function private:makeButtons()
  local unlock_btn = mimas.Button('unlock')
  self.unlock_btn = unlock_btn
  self.btn_lay = mimas.HBoxLayout()
  self.lay:addWidget(self.btn_lay)
  -- Filters
  self.btns = {}
  local function makeBtn(typ)
    local btn = mimas.CheckBox(typ, function(b, state)
      log_show[typ] = state
      editor.Settings:save(true)
      private.filterChanged(self)
    end)
    -- FIXME: this does not work and it's ugly.
    btn:setAttribute(mimas.WA_MacShowFocusRect, false)
    btn:setChecked(log_show[typ])
    self.btns[typ] = btn
    self.btn_lay:addWidget(btn)
    return btn
  end
  -- info
  local btn = makeBtn('info')
  
  -- warn
  btn = makeBtn('warn')
  --btn:setStyle('color:orange;')

  -- error
  btn = makeBtn('error')
  --btn:setStyle('color:red;')


  self.btn_lay:addStretch()
  self.btn_lay:addWidget(unlock_btn)
  unlock_btn:hide()
  function unlock_btn.click()
    self:unlock()
  end
end


function private:makeList()
  local list = mimas.ListView()
  list:setAttribute(mimas.WA_MacShowFocusRect, false)
  list:enableHtml(true,
  -- CSS rules
  [[
   body {
     margin:0;
     padding:0;
     font-size:11px;
     color:#B4B4B4;
   }
   body.two {
     background:#222;
   }
   body.three {
     background:#292929;
   }
   body.selected {
     background:#6E4E24;
   }
   p {
    margin:0;padding:0;
    white-space:pre;
    margin-left:20px;
    text-indent:-20px;
    border:2px solid yellow;
   }
   p.error .url { color:red; }
   p.warn .url { color:orange;}
   p span { font-size:12px; color:white;}
  ]]
  )
  self.lay:addWidget(list)
  self.list = list
  function list.rowCount(l)
    return #self.view_data
  end

  function list.data(l, row_i)
    local n = self.view_data[row_i]
    if n then
      return n.html
    end
  end

  function list.select(l, i)
    local log = self.view_data[i]
    if log then
      local url = log.url
      local line = string.match(log.msg, ':([0-9]+):')
      if self.zone then
        self.zone:editNode(url, line)
      end
      private.showUnlock(self)
      private.select(self, self.view_data[i])
      list:reset()
    end
    return true
  end

  function list.slider(s, dir, action)
    -- When the user changes the vertical slider, we
    -- stop changing log data.
    if dir == Vertical then
      private.showUnlock(self)
    end
  end

end

function private:showUnlock()
  if not self.locked then
    self.locked = true
    self.unlock_btn:show()
    self.view_data = yaml.load(yaml.dump(self.data))
    self.view_bak  = self.view_data
  end
end

function lib:unlock()
  self.unlock_btn:hide()
  self.locked = false
  private.filterChanged(self)
end

function lib:click(x, y, op)
  if op == mimas.DoubleClick then
    private.clear(self)
  end
end

function private:filterChanged()
  -- rebuild view data
  local data = self.data
  if self.locked then
    data = self.view_bak
  end
  local vdata = {}
  local sz = 0
  for _, entry in ipairs(data) do
    if log_show[entry.typ] then
      sz = sz + 1
      table.insert(vdata, entry)
    end
  end
  vdata.sz = sz
  self.view_data = vdata
  self.list:reset()
  if not self.locked then
    self.list:scrollToBottom()
  end
end

function private:select(entry)
  local selected = self.selected
  if selected then
    -- deselect
    selected.html = selected.html_bak
    selected.html_bak = nil
  end
  entry.html_bak = entry.html
  entry.html = fmt([[<html><body class='selected'><p class='%s'><span class='url'>%s</span> %s</p></body></html>]], entry.typ, entry.url, entry.msg)
  self.selected = entry
end
  
function private:clear()
  self.selected = nil
  self.data = {sz = 0}
  self.view_data = data
  self:unlock()
end
