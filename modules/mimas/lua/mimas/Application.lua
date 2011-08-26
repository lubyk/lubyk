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
require 'worker'
local worker = worker
local constr = mimas_core.Application
function mimas.Application()
  local instance = constr(worker)
  instance:setStyleSheet [[
    .window { background:rgb(38, 38, 38); color: rgb(180,180,180);}
    .table {
      background:rgb(38, 38, 38);
      color: rgb(180,180,180);
      alternate-background-color:rgb(52, 52, 52);
      border:0;
    }
    .list {
      background:rgb(38, 38, 38);
      color: rgb(180,180,180);
      border:0;
      padding:0;
    }
    .list::item {
      padding:2px;
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
  return instance
end
