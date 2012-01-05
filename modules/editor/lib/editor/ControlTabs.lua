--[[------------------------------------------------------

  editor.ControlTabs
  ------------------

  Displays all the views in a tabbed interface.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'TabWidget')
editor.ControlTabs = lib

-- constants
function lib:init(zone)
  self.zone = zone
  local add = mimas.Widget()
  self.add_tab = add
  add.lay = mimas.VBoxLayout(add)
  add.dlg = mimas.SimpleDialog {
    'Create a new view',
    {'vbox', box=true, style='background: #222',
      'view name',
      {'input', 'name', 'base'},
    },
    {'hbox',
      {}, -- this adds stretch
      {'btn', 'Cancel'},
      {'btn', 'OK', default=true},
    },
  }
  add.lay:addWidget(add.dlg, 0, mimas.AlignCenter)
  function add.dlg:btn(name)
    printf("add_tab:btn %s / %s", name, self.form.name)
    if name == 'OK' then
      -- create view
      local morph = zone.morph
      if morph then
        morph.push:send(lubyk.update_url, {
          views = {
            [self.form.name] = {},
          }
        })
      end
    else
      -- do nothing
    end
  end
  self:addTab(self.add_tab, '+')
end

function lib:resized(w, h)
  self.width  = w
  self.height = h
end

