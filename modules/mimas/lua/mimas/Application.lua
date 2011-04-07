--[[------------------------------------------------------

  mimas.Application
  --------------

  GUI Application.

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
    }
    .list {
      background:rgb(38, 38, 38);
      color: rgb(180,180,180);
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