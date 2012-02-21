--[[------------------------------------------------------

  mimas.Application
  --------------

  GUI Application. The object returned by
  mimas.Application() is a table that will contain
  callbacks (stored as references in the numerical index).
  This means that storing things in the application with
  app[4] = 'xxx' is very bad. Doing app.foo = 'yyy' is
  good.

--]]------------------------------------------------------
local constr = mimas_core.Application
local mt     = mimas_core.Application_
mimas.Application_ = mt

function mimas.Application(type)
  local self = constr(type)
  self:setQuitOnLastWindowClosed(mimas.Settings.quit_on_close)
  self:setStyleSheet [[
    .window, .main_window, .widget, .table, .list, .menu, .tab_widget::pane, .label, .control, .checkbox {
      background:#151515;
      color:#B4B4B4;
    }
    .table {
      alternate-background-color:rgb(52, 52, 52);
      border:0;
    }
    .list, .menu {
      border:0;
      padding:0;
    }
    .menu {
      border:1px solid #777;
      font-size:13px;
      background:#222;
    }
    .menu::separator {
      height: 2px;
      background: #444;
    }
    .menu::item {
      padding:2px 5px;
      border-top:1px solid #444;
    }
    .menu::item:first {
      border-top:0;
    }
    .menu::item:selected {
      background: rgb(55,55,55);
    }
    .list::item {
      padding:2px;
    }
    .group_box {
      background: #222;
    }
    .list::item:selected {
        border: 0;
        color: #B4B4B4;
        background:#6E4E24;
        padding:0px;
     }
    .line_edit {
      background:rgb(55,55,55);
      color:white;
      selection-color:white;
      selection-background-color: rgb(40,85,140);
    }
    .glwindow { border-style: none;}
    .glwidget { border-style: none;}
    .label { background: transparent; }

    /* TabWidget */
    .tab_widget { 
      background:#444;
    }
    .tab_widget::pane {
      position:absolute;
      top: -1px;
      border-top:1px solid #999;
    }
    .tab_widget::tab-bar {
      left:15px;
    }
    QTabBar::tab {
      margin-top:8px;
      border: 1px solid #999;
      border-top-left-radius: 4px;
      border-top-right-radius: 4px;
      padding:2px 8px;
      background:#777;
      color:#222;
    }
    QTabBar::tab:first {
      margin-left:0px;
    }
    QTabBar::tab:selected {
      background:#262626;
      border-bottom:0;
      margin-top:5px;
      color:#B4B4B4;
    }
    QScrollBar:horizontal{
      background: #262626;
      height: 8px;
      margin:0;
    }
    QScrollBar:vertical {
      background: #262626;
      width: 8px;
      margin:0;
    }
    QScrollBar::handle:horizontal {
        background: #555;
        min-width: 10px;
    }
    QScrollBar::handle:vertical {
        background: #555;
        min-height: 10px;
    }
    QScrollBar::add-line:horizontal,
    QScrollBar::sub-line:horizontal,
    QScrollBar::sub-line:vertical,
    QScrollBar::add-line:vertical {
        height: 0px;
        width: 0px;
    }
    /* end TabWidget */
    .control {
      font-size:10px;
    }
    QCheckBox::indicator {
      border:1px solid #aaa;
      width: 6px;
      height: 6px;
    }
    QCheckBox::indicator:checked {
      background:#777;
    }


    #test_name { color: hsva(20, 255, 255, 80%); }
    #test_name.slider { color:hsv(30,200,200); }
  ]]

  return self
end

local singleShot = mt.singleShot
function mt.singleShot(app, timeout, func_or_clbk)
  local clbk = func_or_clbk
  if type(func_or_clbk) == 'function' then
    clbk = mimas.Callback(function(self)
      func_or_clbk()
      app[self] = nil
    end)
    -- avoid gc
    app[clbk] = clbk
  end
  singleShot(app, timeout, clbk:object(), '1callback()')
end

