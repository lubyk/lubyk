--[[------------------------------------------------------

  Basic tests for rubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('mimas')

function should.display_widget()
  local app = mimas.Application()
  local win = mimas.Widget()
  win:resize(320, 240)

  win:show()
  app:exec()
end

test.all()