--[[------------------------------------------------------

  Basic tests for rubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('mimas')

local app = mimas.Application()
local widgets

function should.insert_label_in_layout()
  local win = mimas.Widget()
  win:move(100, 100)
  local lb = mimas.Label("Label not in layout", win)
  lb:move(180, 0)
  lb:resize(180, 20)

  local lay = mimas.VBoxLayout(win)
  local lbl1 = mimas.Label("Label in layout")
  lay:addWidget(lbl1)

  -- can use rgb(), rgba(), hsv(), hsva() or #00FA88 (caps)
  local tests = {
    'color:hsva(180, 255, 255, 20%)',
    'border: 2px solid hsv(80, 255, 255)',
    'background: hsva(80, 255, 255, 40%)',
    'padding:15px; background: hsva(70, 255, 255, 30%); border: 1px solid blue'
  }

  for _, style_test in ipairs(tests) do
    local lbl = mimas.Label(string.format("setStyle: %s", style_test))
    lay:addWidget(lbl)
    lbl:setStyle(style_test)
    -- avoid GC
    tests[style_test] = lbl
  end

  local lbl1 = mimas.Label("setHue")
  lay:addWidget(lbl1)
  lbl1:setHue(210)

  local lbl2 = mimas.Label("name = test_name")
  lbl2:setName("test_name")
  lay:addWidget(lbl2)

  -- visual check
  assert_true(true)
  win:show()
end

app:post(function()
  test.all()
end)

app:exec()