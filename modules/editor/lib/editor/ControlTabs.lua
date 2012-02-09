--[[------------------------------------------------------

  editor.ControlTabs
  ------------------

  Displays all the views in a tabbed interface.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'TabWidget')
editor.ControlTabs = lib

local private = {}

-- constants
function lib:init(zone_view)
  self.zone_view = zone_view
  self.zone = zone_view.zone
end

function lib:addView(name, def)
  local view = editor.ControlView(name, def, self.zone)
  self:insertTab(-2, view, name)
  if self:count() == 2 then
    self:selectTab(0)
  end
  return view
end

function lib:resized(w, h)
  self.width  = w
  self.height = h
end

function lib:hide()
  self.super:hide()
  if self.zone.machine_list then
    self.zone.machine_list:controlViewChanged(false)
  end
end

function lib:show()
  self.super:show()
  if self.zone.machine_list then
    self.zone.machine_list:controlViewChanged(true)
  end
end

function lib:addPlusView()
  local zone = self.zone
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

function lib:removePlusView()
  self.add_tab:hide()
  self.add_tab:__gc()
  self.add_tab = nil
end
