--[[------------------------------------------------------

  mimas.TabWidget test
  --------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.TabWidget')
local withUser = should:testWithUser()

function should.autoload()
  assertTrue(mimas.TabWidget)
end

function should.createTabWidget()
  local win = mimas.TabWidget()
  assertMatch('mimas.TabWidget', win:__tostring())
end

function withUser.should.displayTabs(t)
  t.click = ''
  local win = mimas.TabWidget()
  ------------------------------------ mars
  local mars = mimas.Widget()
  local mlay = mimas.VBoxLayout(mars)
  local mlbl = mimas.Label('We are on Mars.')
  mlay:addWidget(mlbl)
  local mbtn = mimas.Button('Click here first.', function(on)
    if on then
      mlbl:setText('Select Venus now.')
      t.click = t.click .. 'Mars'
    end
  end)
  mlay:addWidget(mbtn)

  ------------------------------------ venus
  local venus = mimas.Widget()
  local vlay = mimas.VBoxLayout(venus)
  local vlbl = mimas.Label('We are on Venus.')
  vlay:addWidget(vlbl)
  local vbtn = mimas.Button('And then here.', function(on)
    if on then
      t.click = t.click .. ' & Venus'
    end
  end)
  vlay:addWidget(vbtn)

  win:addTab(mars, 'Mars')
  win:addTab(venus, 'Venus')

  win:move(100, 100)
  win:resize(200,200)

  win:show()
  t:timeout(function()
    return t.click == 'Mars & Venus'
  end)
  assertEqual('Mars & Venus', t.click)
end
 
function withUser.should.captureKeyboard(t)
  t.win = mimas.TabWidget()
  t.tab = mimas.Window()
  t.lay = mimas.VBoxLayout(t.tab)
  t.lbl = mimas.Label('Pres key "d".')
  t.lay:addWidget(t.lbl)
  t.win:addTab(t.tab, "hello")
  function t.win:keyboard(key, on, chr, mod)
    t.lbl:setText(string.format('key=%i, on=%s, chr=%s, mod=%i', key, (on and 'on') or 'off', chr, mod))
    if chr == 'd' then
      t.continue = true
    end
  end
  t.win:resize(400,100)
  t.win:move(10,10)
  t.win:show()
  t:timeout(function()
    return t.continue
  end)
  t.win:close()
  assertTrue(t.continue)
end                  
test.all()

