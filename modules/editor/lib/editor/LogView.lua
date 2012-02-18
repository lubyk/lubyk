--[[------------------------------------------------------

  editor.LogView
  --------------

  The LogView is a top floating window that displays
  messages and errors from remote processes and nodes.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget', mimas.ToolFlag)
editor.LogView = lib
lib.MaxLogSize = 30
local private = {}
local settings = editor.Settings

function lib:init(zone)
  self:setAttribute(mimas.WA_MacAlwaysShowToolWindow, true)
  self.zone  = zone
  local lv = settings.log_view
  self:move(lv.x, lv.y)
  self:resize(lv.w, lv.h)
  self:setFocusPolicy(mimas.ClickFocus)
  self:setFocus(false)
  self.lay = mimas.VBoxLayout(self)
  self.data = {
    -- empty message for padding (kept last)
    {
      url = '',
      typ = 'info',
      msg = '',
    }, 
    sz=0
  }
  private.makeList(self)
end

local fmt = string.format
function lib:addMessage(url, typ, msg)
  msg = string.gsub(msg, '^%s*(.-)%s*$', '%1')
  local data = self.data
  table.insert(data, data.sz, {
    url = url,
    msg = fmt([[<html><body><p class='%s'><span class='url'>%s</span> %s</p></body></html>]], typ, url, msg),
    typ = typ,
  })
  if data.sz >= self.MaxLogSize then
    table.remove(data, 1)
  else
    data.sz = data.sz + 1
  end
  self.list:scrollToBottom()
  self.list:reset()
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

function private:makeList()
  local list = mimas.ListView()
  list:enableHtml(true,
  -- CSS rules
  [[
   body {
     margin:0;
     padding:0;
     font-size:11px;
     color:#B4B4B4;
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
    return #self.data
  end

  function list.data(l, row_i)
    local n = self.data[row_i]
    if n then
      return n.msg
    end
  end

  function list:select(i)
    return true
  end
end

