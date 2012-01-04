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
  self:setQuitOnLastWindowClosed(Lubyk.mimas_quit_on_close)
  self:setStyleSheet [[
    .window { background:rgb(38, 38, 38); color: rgb(180,180,180);}
    .main_window { background:rgb(38, 38, 38); color: rgb(180,180,180);}
    .table {
      background:rgb(38, 38, 38);
      color: rgb(180,180,180);
      alternate-background-color:rgb(52, 52, 52);
      border:0;
    }
    .list, .menu {
      background:rgb(38, 38, 38);
      color: rgb(180,180,180);
      border:0;
      padding:0;
    }
    .menu {
      font-size:13px;
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
        color: rgb(180,180,180);
        background: rgb(55, 55, 55);
        padding:0px;
     }
    .line_edit {
      background:rgb(55,55,55);
      color:white;
      selection-color:white;
      selection-background-color: rgb(40,85,140);
    }
    .widget { background:rgb(38, 38, 38); }
    .glwindow { border-style: none;}
    .glwidget { border-style: none;}
    .label { background: transparent; color: rgb(180,180,180); }
    .slider { qproperty-border:2;
      background:transparent;
      color:hsva(80,255,255,75%);
      alternate-background-color:hsv(80,255,255);
      selection-color:rgba(255,255,255,80);
    }
    #test_name { color: hsva(20, 255, 255, 80%); }
    #test_name.slider { color:hsv(30,200,200); }
  ]]

  -- TODO: rewrite 'post' to keep a pool of Callback objects (or accept an existing Callback).
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

